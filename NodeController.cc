//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include <set>
#include "inet/common/queue/DropTailQueue.h"
#include "inet/networklayer/spring/control/NodeController.h"
#include "inet/linklayer/ppp/PPP.h"


namespace inet {

namespace spring {

Define_Module(NodeController);

void NodeController::initialize(int stage) {
    int k = getParentModule()->getParentModule()->par("k");

    cSimpleModule::initialize(stage);
    if (stage == INITSTAGE_LOCAL) {
        minNodalSID = 1;
        maxNodalSID = par("maxNodalSID");

        minAdjacencySID = maxNodalSID + 1;
        maxAdjacencySID = par("maxAdjacencySID");

        //ASSERT(minNodalSID <= nodeSID && nodeSID <=maxNodalSID);
        ASSERT(0 <= minNodalSID && minNodalSID <= maxNodalSID);
        ASSERT(minAdjacencySID <= maxAdjacencySID);

        ift = getModuleFromPar<IInterfaceTable>(par("interfaceTableModule"), this);
        rt = getModuleFromPar<IIPv4RoutingTable>(par("routingTableModule"), this);
        fib = getModuleFromPar<FibTable>(par("springFibTableModule"), this);
        fec = getModuleFromPar<FecTable>(par("springFecTableModule"), this);
        classif = getModuleFromPar<Classifier>(par("springClassifierModule"), this);

        reclassif = getModuleFromPar<Reclassifier>(par("springReclassifierModule"), this);

        ospf = check_and_cast<OSPFRouting *>(getModuleByPath("^.ospf"));

        lsaDatabase = std::unique_ptr < OpaqueLsaDatabase> (new OpaqueLsaDatabase());
        srgb = std::unique_ptr < SRGBDatabase > (new SRGBDatabase());
        srgb->addRange(minNodalSID, maxNodalSID - minNodalSID + 1);
        bool valid = srgb->getIndexOfSID(par("nodeId"), nodeSIDidx);
        ASSERT(valid);

        classif->setSid(getNodalSID());
        reclassif->setSid(getNodalSID());
        firstSelfMsg = true;
        scheduleAt(simTime() + 70, new cMessage);
    }

    //wait for interfaceTable of the controller
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        controllerIP = L3AddressResolver().resolve(par("controllerIP"));
        localPort = par("localPort");
        controllerPort = par("controllerPort");

        if (controllerIP.isUnspecified()) {
            ASSERT(0);
        }
        else {
            controllerSocket.setOutputGate(gate("udpOut"));
            controllerSocket.bind(localPort);
            controllerSocket.connect(controllerIP, controllerPort);
            EV << "Socket created" << endl;
        }

        netGraph = std::unique_ptr < NetGraph> (new NetGraph(rt->getRouterId(), srgb.get(), true));

        ospf->registerOpaqueListener(this, OSPFOpaqueType::EXTENDED_LINK_TYPE,0);
        ospf->registerOpaqueListener(this, OSPFOpaqueType::EXTENDED_PREFIX_TYPE, 0);
        ospf->registerOpaqueListener(this, OSPFOpaqueType::PRIVATE_TYPE_TURN_OFF, 0);
        floodLocalLinksDescription();
    }
    int num_of_connections_for_each_core_without_clients = getParentModule()->getParentModule()->par("num_of_connections_for_each_core_without_clients");

    for(int i=0; i<10; i++) channelUtilizationArr[i]=-1;
    for(int i=0; i<num_of_connections_for_each_core_without_clients; i++) delayPerLink[i]=-1;



}

//IOpaqueListener
void NodeController::notify(OSPFOpaqueLSA *lsa) {
    Enter_Method_Silent();
    EV_DETAIL << "Received LSA update: " << *lsa;
    EV_DETAIL << "Adding to LSA map: "<<endl;
    lsaDatabase->updateLSAinMap(std::unique_ptr<OSPFOpaqueLSA>(lsa));
}

void NodeController::updateExplicitRoutes(EROs &eros){
    for (ERO &ero : eros ) {

        if (ero.srcIp.isUnspecified()) {
            continue;
        }

        SegmentId egresSid = ero.explicitRoute.back();

        //extract without first element
        //FIXME, don't copy data
        auto it = ero.explicitRoute.begin();
        SIDVector explicitRoute(++it, ero.explicitRoute.end());

        fec->updateExplicitRoute(egresSid, explicitRoute);
    }
}

void NodeController::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleTimer(msg);
    } else if (msg->getArrivalGateId() == gate("udpIn")->getId()
            && msg->getKind() == UDP_I_DATA) {
        EV_DETAIL << "Received EROs" << endl;

        UDPEros * packet = dynamic_cast<UDPEros*>(msg);
        ASSERT(packet);

        updateExplicitRoutes(packet->getEros());
        delete msg;
    }
    else {
        throw cRuntimeError("Unexpected message");
    }

}

void NodeController::handleTimer(cMessage *msg) {
    LoadAndDelay();

    if (timerFlag>0.9)
    {
        scheduleAt(simTime() + 0.1 + dblrand() / 5, msg);
        timerFlag=0;
    fib->refreshTable();


   // LoadAndDelay();

    //**********//

    for (int i = 0; i < ift->getNumInterfaces(); i++) {
        InterfaceEntry *ie = ift->getInterface(i);

        if (ie->getNodeOutputGateId() < 0) {
            continue;
        }

        cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
        ConfigurableThruputMeteringChannel * ch =
                dynamic_cast<ConfigurableThruputMeteringChannel *>(g->getChannel());

        if (ch != nullptr) {
            ch->updateDisplay();
        }

    }



    /****************/

    if (firstSelfMsg) {
        netGraph->constructGraph(*lsaDatabase);
        netGraph->computeRoutes();
        rebuildFibTableECMP();
        firstSelfMsg = false;
    }

    EV << "Sending stats to controller" << endl;
    UDPStats * packet = new UDPStats;
    packet->setOrigSid(nodeSIDidx);
    for (auto &it : classif->getStats()) {
        unsigned int sidIdx;
        bool valid = srgb->getIndexOfSID(it.first, sidIdx);
        ASSERT(valid);
        packet->getStats().emplace_back(sidIdx,
                it.second.getTotal(simTime()) / it.second.getInterval(simTime()));
    }
    controllerSocket.send(packet);


    //Update the colors of links in the GUI
    for (int i = 0; i < ift->getNumInterfaces(); i++) {
        InterfaceEntry *ie = ift->getInterface(i);

        if (ie->getNodeOutputGateId() < 0) {
            continue;
        }

        cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
        ConfigurableThruputMeteringChannel * ch =
                dynamic_cast<ConfigurableThruputMeteringChannel *>(g->getChannel());

        if (ch != nullptr) {
            ch->updateDisplay();
        }

    }

    }
    else
    {
        timerFlag+=0.1;
        updateChannels();
//        updateChannels2();
        scheduleAt(simTime() + 0.1 , msg);
    }

}
void NodeController::updateChannels() {

    int k = getParentModule()->getParentModule()->par("k");

    for (int i = 0; i < ift->getNumInterfaces(); i++) {
           InterfaceEntry *ie = ift->getInterface(i);

           if (ie->getNodeOutputGateId() < 0) {
               continue;
           }

        // PPP *pp=check_and_cast<PPP *>(getModuleByPath("^.^.Cores[0].ppp[0].queue"));

           //SPRING_LSR sr* = check_and_cast< SPRING_LSR> (getParentModule());
     //  DropTailQueue *qu=dynamic_cast<DropTailQueue *>(getParentModule()->getModuleByPath("ppp[0].queue"));
          // if (qu != nullptr) {
          //   int banana=15;
          // }
          // PPP qu=getParentModule()->getModuleByPath("ppp[0]");


         //if (simTime()>83 && num>1)
         //num=ift->getNumInterfaces();

           cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
           ConfigurableThruputMeteringChannel * ch =
                   dynamic_cast<ConfigurableThruputMeteringChannel *>(g->getChannel());
           if (ch != nullptr) {

               double util= ch->getUtilization();

               channelUtilizationArr[i]=util;
           }
    }

    DropTailQueue *qu;
    char buff[50];
    int numOfPacket; //TODO taking into account the procesing time
    double delay=getParentModule()->getParentModule()->par("del");
    int num_of_clients = getParentModule()->getParentModule()->par("num_of_clients");
    int num_of_connections_for_each_core_without_clients = getParentModule()->getParentModule()->par("num_of_connections_for_each_core_without_clients");
    int num_of_connections = num_of_clients + num_of_connections_for_each_core_without_clients;


//    if(fib->isThisAnEdge()){
        //yodars
       for(int i=num_of_clients; i<num_of_connections; i++) {
            sprintf(buff,"^.ppp[%d].queue",i);
            qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
            numOfPacket=qu->queueGetLen();
            delayPerLink[i-num_of_clients]= numOfPacket*delay;
        }
//    }
//    else {
//        for(int i=0; i<k; i++) {
//            sprintf(buff,"^.ppp[%d].queue",i);
//            qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
//            numOfPacket=qu->queueGetLen();
//            delayPerLink[i]= numOfPacket*delay;
//       }
//    }
  //  sprintf(buff,"^.ppp[%d]",3);
    //PPP *pp=check_and_cast<PPP *>(getModuleByPath(buff));
 //  auto dd=  pp-> gate(3)->findTransmissionChannel()->getFullPath();
  // int h=0;

}

void NodeController::updateChannels2() {

    int k = getParentModule()->getParentModule()->par("k");
    DropTailQueue *qu;
    char buff[50];
    int numOfPacket; //TODO taking into account the procesing time
    double delay=getParentModule()->getParentModule()->par("del");
    SegmentId currentSeg=0;

    for (int i = 0; i < ift->getNumInterfaces(); i++) {
       InterfaceEntry *ie = ift->getInterface(i);
       if (ie->getNodeOutputGateId() < 0) {
           continue;
       }
//         PPP *pp=check_and_cast<PPP *>(getModuleByPath("^.^.Cores[0].ppp[0].queue"));
//
//           SPRING_LSR sr* = check_and_cast< SPRING_LSR> (getParentModule());
//       DropTailQueue *qu=dynamic_cast<DropTailQueue *>(getParentModule()->getModuleByPath("ppp[0].queue"));
//           if (qu != nullptr) {
//             int banana=15;
//           }
//           PPP qu=getParentModule()->getModuleByPath("ppp[0]");
//
//
//         if (simTime()>83 && num>1)
//         num=ift->getNumInterfaces();

       cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
       ConfigurableThruputMeteringChannel * ch =
               dynamic_cast<ConfigurableThruputMeteringChannel *>(g->getChannel());
       if (ch != nullptr) {
           double util= ch->getUtilization();
           channelUtilizationArr[i]=util;
       }

//       if(fib->isThisAnEdge()){
           //yodars
          for(int i=2; i<k+1; i++) {
              auto springModule= g->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule("ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule("spring");
              currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
              sprintf(buff,"^.ppp[%d].queue",i);
              qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
              numOfPacket=qu->queueGetLen();
              edge ewEdge(reclassif->getSelfSID(),currentSeg,0,0);
          }
//       }
//       else {
//          for(int i=0; i<k; i++) {
//              int selfSID=reclassif->getSelfSID();
//              auto springModule= g->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule("ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule("spring");
//              currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
//              sprintf(buff,"^.ppp[%d].queue",i);
//              qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
//              numOfPacket=qu->queueGetLen();
//              edge newEdge(selfSID,currentSeg,0,0);
//
//          }
//       }

    }




//    sprintf(buff,"^.ppp[%d]",3);
//    PPP *pp=check_and_cast<PPP *>(getModuleByPath(buff));
//   auto dd=  pp-> gate(3)->findTransmissionChannel()->getFullPath();
//   int h=0;

}


void NodeController::rebuildFibTableECMP() {
    //find, in the ECMP dijkstra structure, all next hops for the given node
    auto findAllLinksToNextHops =
            [this](NetGraph::Node * dstNode ) {
                std::set<NetGraph::Link *> linksToNH;

                std::vector<bool> visited(netGraph->getNumNodes(), false);
                std::queue<NetGraph::Node *> toVisit;

                toVisit.push(dstNode);
                while( !toVisit.empty() ) {
                    NetGraph::Node * node = toVisit.front();
                    toVisit.pop();
                    visited[node->getIndex()] = true;

                    for(NetGraph::LinkIn *linkIn : netGraph->linkFromParent.getAllLinks(node->getIndex())) {
                        int remoteIdx = static_cast<NetGraph::Node*>(linkIn->getRemoteNode())->getIndex();

                        if(netGraph->linkFromParent[remoteIdx] == NULL) {
                            linksToNH.insert( (NetGraph::Link *) linkIn );
                        } else if (!visited[remoteIdx]) {
                            toVisit.push( static_cast<NetGraph::Node *> (netGraph->getNode(remoteIdx)) );
                        }
                    }

                }

                ASSERT(linksToNH.size() > 0);
                return linksToNH;
            };


    fib->clear();
    for (int i = 0; i < (int) netGraph->getNumNodes(); i++) {
        NetGraph::Node * node = (NetGraph::Node *) netGraph->getNode(i);

        if (rt->isLocalAddress(node->getRouterID())) {
            fib->addEntry(node->getSid(), FibEntry::makeNext());
            continue;
        }

        std::set<NetGraph::Link *> linksToNextHops  = findAllLinksToNextHops(node);

        for(auto nh : linksToNextHops){
            InterfaceEntry *ie = ift->getInterfaceById(nh->getSid());
            ASSERT(ie);
            int iftIndex = ie->getNetworkLayerGateIndex();
            ASSERT(iftIndex > 0);

            fib->addECMPEntry(node->getSid(), FibEntry::makeContinue(iftIndex));
        }
    }

    EV_DETAIL << *fib << endl;
}


void NodeController::rebuildFibTable() {

    fib->clear();
    for (int i = 0; i < (int) netGraph->getNumNodes(); i++) {
        NetGraph::Node * node = (NetGraph::Node *) netGraph->getNode(i);

        if (rt->isLocalAddress(node->getRouterID())) {
            fib->addEntry(node->getSid(), FibEntry::makeNext());
        } else {
            NetGraph::LinkIn *linkIn = netGraph->linkFromParent[node->getIndex()];
            NetGraph::Link *link = (NetGraph::Link*) linkIn;
            ASSERT(linkIn);
            while ( netGraph->linkFromParent[((NetGraph::Node*) linkIn->getRemoteNode())->getIndex()]) {
                linkIn =  netGraph->linkFromParent[((NetGraph::Node*) linkIn->getRemoteNode())->getIndex()];
                link = (NetGraph::Link*) linkIn;
            }
            InterfaceEntry *ie = ift->getInterfaceById(link->getSid());
            ASSERT(ie);
            int iftIndex = ie->getNetworkLayerGateIndex();
            ASSERT(iftIndex > 0);
            fib->addEntry(node->getSid(), FibEntry::makeContinue(iftIndex));
        }
    }

    EV_DETAIL << *fib << endl;
}

void NodeController::floodLocalLinksDescription() {
    ASSERT(ift);
    ASSERT(rt);

    unsigned int opaqueInfoLen = 0;

    //set the sid/la belsub-tlv
    //SIDLabelValue *sidVal = new SIDLabelValue();
    //sidVal->setSid(nodeSID);

    PrefixSIDValue *prefSID = new PrefixSIDValue();
    PrefixSIDFlags prefSIDFlags;
    prefSIDFlags.L_LocalGlobal = 0; //global
    prefSIDFlags.V_ValueIndex = 0; //index
    prefSIDFlags.N_NodeSID = 1;
    prefSID->setFlags(prefSIDFlags);
    prefSID->setMultiTopoID(0);
    prefSID->setAlgorithm(0);
    prefSID->setLabelOrIdx(nodeSIDidx);

    ExtendedPrefixVal * extPrefVal = new ExtendedPrefixVal();
    extPrefVal->setAddressFamily(ExtendedPrefixAF::IPV4_UNICAST);
    extPrefVal->setPrefix(rt->getRouterId());
    extPrefVal->setPrefixLength(32);
    extPrefVal->setRouteType(ExtendedPrefixRouteType::INTER_AREA);
    extPrefVal->setFlags(ExtendedPrefixFlags());
    extPrefVal->getSubTLVs().setTlvsArraySize(1);
    extPrefVal->getSubTLVs().getTlvs(0).setValue(prefSID);
    extPrefVal->getSubTLVs().getTlvs(0).setType(
            ExtendedPrefixSubTLVTypes::SPRING_PREFIX_SID);
    //extPrefVal->getSubTLVs().getTlvs(0).setLength(0);//TODO

    OSPFOpaqueTLVs *opaquePrefixInfo = new OSPFOpaqueTLVs();
    opaquePrefixInfo->setTlvsArraySize(1);
    opaquePrefixInfo->getTlvs(0).setValue(extPrefVal);
    opaquePrefixInfo->getTlvs(0).setType(
            ExtendedPrefixTLVTypes::EXTENDED_PREFIX);
    ospf->floodOpaqueArea(opaquePrefixInfo, opaqueInfoLen,
            BACKBONE_AREAID, OSPFOpaqueType::EXTENDED_PREFIX_TYPE, 0);

    OSPFOpaqueTLVs *opaqueLinksInfo = new OSPFOpaqueTLVs();
    opaqueInfoLen = 0;
    //inspired by ted
    for (int i = 0; i < ift->getNumInterfaces(); i++) {
        InterfaceEntry *ie = ift->getInterface(i);

        if (ie->getNodeOutputGateId() == -1) // ignore if it's not a physical interface
            continue;

        cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
        ASSERT(g);
        double linkBandwidth = g->getChannel()->getNominalDatarate();
        double linkDelay =
                check_and_cast<cDatarateChannel *>(g->getChannel())->getDelay().dbl();

        g->getDisplayString().setTagArg("t", 0, "");
        // find destination node for current interface
        cModule *destNode = nullptr;
        g = g->getNextGate();
        while (g) {
            cModule *mod = g->getOwnerModule();
            cProperties* props = mod->getProperties();
            if (props && props->getAsBool("springNode")) {
                destNode = mod;
                break;
            }
            g = g->getNextGate();
        }

        if (!g)     // not connected
            continue;
        IIPv4RoutingTable *destRt = L3AddressResolver().findIPv4RoutingTableOf(destNode);
        if (!destRt)    // switch, hub, bus, accesspoint, etc
            continue;
        IPv4Address destRouterId = destRt->getRouterId();
        IInterfaceTable* destIft = L3AddressResolver().findInterfaceTableOf(destNode);
        ASSERT(destIft);
        InterfaceEntry *destIe = destIft->getInterfaceByNodeInputGateId(g->getId());
        ASSERT(destIe);

        ASSERT(ie->ipv4Data());
        ASSERT(destIe->ipv4Data());

        AdjSIDValue *adjSID = new AdjSIDValue();
        AdjSIDFlags adjSIDFlags;
        adjSIDFlags.V_ValueIndex = 1; //value
        adjSIDFlags.L_LocalGlobal = 1; //local
        adjSID->setFlags(adjSIDFlags);
        adjSID->setWeight(1);
        adjSID->setMultiTopoID(0);
        adjSID->setLabelOrIdx(ie->getInterfaceId());
        adjSID->setCost(linkDelay);
        adjSID->setDatarate(linkBandwidth);

        ExtendedLinkVal * extLinkVal = new ExtendedLinkVal();
        extLinkVal->setLinkType(LinkType::POINTTOPOINT_LINK);
        extLinkVal->setLinkID(destRouterId);
        extLinkVal->getSubTLVs().setTlvsArraySize(1);
        extLinkVal->getSubTLVs().getTlvs(0).setValue(adjSID);
        extLinkVal->getSubTLVs().getTlvs(0).setType(
                ExtendedLinkSubTLVTypes::SPRING_ADJ_SID);
        //extPrefVal->getSubTLVs().getTlvs(0).setLength(0);//TODO

        int arraySize = opaqueLinksInfo->getTlvsArraySize();
        opaqueLinksInfo->setTlvsArraySize(arraySize + 1);
        opaqueLinksInfo->getTlvs(arraySize).setValue(extLinkVal);
        opaqueLinksInfo->getTlvs(arraySize).setType(
                ExtendedLinkTLVTypes::EXTENDED_LINK);
    }
    ospf->floodOpaqueArea(opaqueLinksInfo, opaqueInfoLen, BACKBONE_AREAID,
            OSPFOpaqueType::EXTENDED_LINK_TYPE, 0);
}

SegmentId NodeController::getNodalSID() {
    SegmentId sid = 0;
    srgb->getByIndex(nodeSIDidx, sid);
    return sid;
}

void  NodeController::LoadAndDelay() {
    DropTailQueue *qu;
    char buff[50];
    int numOfPacket; //TODO taking into account the procesing time
    double delay=getParentModule()->getParentModule()->par("del");
    int num_of_clients = getParentModule()->getParentModule()->par("num_of_clients");
    int num_of_connections_for_each_core_without_clients = getParentModule()->getParentModule()->par("num_of_connections_for_each_core_without_clients");
    int num_of_connections = num_of_clients + num_of_connections_for_each_core_without_clients;

    SegmentId currentSeg =0;
    allLoads.clear();
//    for (int i = 0; i < ift->getNumInterfaces(); i++) {
//       InterfaceEntry *ie = ift->getInterface(i);
//       if (ie->getNodeOutputGateId() < 0) {
//           continue;
//       }
//         PPP *pp=check_and_cast<PPP *>(getModuleByPath("^.^.Cores[0].ppp[0].queue"));
//
//           SPRING_LSR sr* = check_and_cast< SPRING_LSR> (getParentModule());
//       DropTailQueue *qu=dynamic_cast<DropTailQueue *>(getParentModule()->getModuleByPath("ppp[0].queue"));
//           if (qu != nullptr) {
//             int banana=15;
//           }
//           PPP qu=getParentModule()->getModuleByPath("ppp[0]");
//
//
//         if (simTime()>83 && num>1)
//         num=ift->getNumInterfaces();

//       cGate *g = getParentModule()->gate(ie->getNodeOutputGateId());
//       ConfigurableThruputMeteringChannel * ch =
//               dynamic_cast<ConfigurableThruputMeteringChannel *>(g->getChannel());
//       if (ch != nullptr) {
//           double util= ch->getUtilization();
//           channelUtilizationArr[i]=util;
//       }

//       if(fib->isThisAnEdge()){ // yodars: NOTE!! in the fat tree not everyone are edges, in dragonfly everyone is
         if(1) {
//            for(int i=k/2; i<k; i++) { // yodars: this line was chen's line
             // yodars, i=1 because number of clients

             for(int i=num_of_clients; i< num_of_connections; i++) {
//              auto springModule= g->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule("ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule("spring");
//              SPRING*    springModule
//              currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
                  sprintf(buff,"^.ppp[%d].queue",i);

                  qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
                  numOfPacket=qu->queueGetLen();
                  sprintf(buff,"^.ppp[%d]",i);


                  auto springModule=  getModuleByPath(buff)->getSubmodule("ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule("spring");

                  currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
                  edge ewEdge(reclassif->getSelfSID(),currentSeg,0,numOfPacket*delay+delay);
                  edge edgesId(reclassif->getSelfSID(),currentSeg);
                  allLoads[edgesId]=ewEdge;

             }
       }
       else { // yodars: not relevant for dragonfly topology
          for(int i=0; i<num_of_connections_for_each_core_without_clients; i++) {
              int selfSID=reclassif->getSelfSID();
//              auto springModule= g->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule("ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule("spring");
//              currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
              sprintf(buff,"^.ppp[%d].queue",i);
              qu=check_and_cast<DropTailQueue *>(getModuleByPath(buff));
              numOfPacket=qu->queueGetLen();
//              edge newEdge(selfSID,currentSeg,0,0);
              sprintf(buff,"^.ppp[%d]",i);
              cModule * pm = getModuleByPath(buff);
              cModule * smpm = pm->getSubmodule("ppp");
              cGate* g = smpm->gate("phys$o");
              cGate* pathEndGate = g->getPathEndGate();
              cModule* om = pathEndGate->getOwnerModule();
              cModule* pmom = om->getParentModule();
              cModule* pmpmom = pmom->getParentModule();
              auto springModule = pmpmom->getSubmodule("spring");
              currentSeg=((SPRING*)springModule)->reclassif->getSelfSID();
              edge ewEdge(reclassif->getSelfSID(),currentSeg,0,numOfPacket*delay+delay);
              allLoads[ewEdge]=ewEdge;

          }
       }

       sprintf(buff,"^.spring");
       SPRING* mySpring = check_and_cast<SPRING *> (getModuleByPath(buff));
       std:: map<SegmentId, MovingWindow>::iterator it;
       std:: map<SegmentId, MovingWindow> loadOnEdges=mySpring->getLoadOnEdges();
       for(it=loadOnEdges.begin(); it!=loadOnEdges.end(); ++it) {
           edge newEdge(reclassif->getSelfSID(),it->first);
           allLoads[newEdge].setLoad(it->second.getTotal());
       }
//       if(simTime()>84.3 && reclassif->getSelfSID()==7) {
//           int kampes=3;
//       }
  }


} //namespace spring

} //namespace inet

