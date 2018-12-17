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

#include "inet/networklayer/spring/control/NetworkController.h"
#include "inet/networklayer/spring/control/SpringControlPackets_m.h"
#include "inet/networklayer/spring/control/NodeController.h"
#include <iostream>

using namespace std;
///#include inet.node.spring.SPRING_LSR;
namespace inet {

namespace spring {

Define_Module(NetworkController);

bool edge::operator<(const edge &right) const {
    if ((int) this->leftEdge < (int) right.leftEdge) {
        return true;
    } else if (this->leftEdge == right.leftEdge) {
        if ((int) this->rightEdge < (int) right.rightEdge) {
            return true;
        } else
            return false;
    } else
        return false;

}
bool edge::operator==(const edge &right) const {

    return (this->leftEdge == right.leftEdge
            && this->rightEdge == right.rightEdge);
}

edge::edge() {
    leftEdge = -1;
    rightEdge = -1;
    load = 0;
    del = -1;
}
edge::edge(SegmentId leftEdge, SegmentId rightEdge, double load, double del) {

    this->leftEdge = leftEdge;
    this->rightEdge = rightEdge;
    this->load = load;
    this->del = del;
}

Flow::Flow() {
    this->entry = ElephentEntry();
    //  this->window=new MovingWindow();
    this->window = new MovingWindow();
    this->pathForEntry = SIDVector();
    ;
    this->first = 0;
    this->TTL = 0;
    this->TTL = 0;
}

Flow::Flow(ElephentEntry entry, MovingWindow window, SIDVector pathForEntry,
        SegmentId first) {
    this->entry = entry;
    this->window = new MovingWindow(window);

    // this->window=MovingWindow();
    this->pathForEntry = pathForEntry;
    this->first = first;
    this->TTL = window.getLastSeen();

}
Flow::Flow(const Flow &obj) {

    this->entry = obj.entry;
    this->window = new MovingWindow(*obj.window);
    this->pathForEntry = obj.pathForEntry;
    this->first = obj.first;
    this->TTL = this->window->getLastSeen();
}

bool Flow::operator<(const Flow &right) const {
    return this->TTL < right.TTL;
}

void NetworkController::initialize(int stage) {

    int i;
    edge e;

    cSimpleModule::initialize(stage);
    if (stage == 0) {

        /*
         for(i=0; i<PATHTABLESIZE; i++) {
         fullbPathTable[i].src=-1;
         fullbPathTable[i].dst=-1;
         }*/
        minNodalSID = 1;
        maxNodalSID = par("maxNodalSID");

        minAdjacencySID = maxNodalSID + 1;
        maxAdjacencySID = par("maxAdjacencySID");

        ASSERT(0 <= minNodalSID && minNodalSID <= maxNodalSID);
        ASSERT(minAdjacencySID <= maxAdjacencySID);

        ospf = check_and_cast<OSPFRouting *>(getModuleByPath("^.ospf"));

        srgb = std::unique_ptr < SRGBDatabase > (new SRGBDatabase());
        srgb->addRange(minNodalSID, maxNodalSID - minNodalSID + 1);

        lsaDatabase = std::unique_ptr < OpaqueLsaDatabase
                > (new OpaqueLsaDatabase());

        network = std::unique_ptr < ControllerNetGraph
                > (new ControllerNetGraph(srgb.get()));

        localPort = par("localPort");
        udpSocket.setOutputGate(gate("udpOut"));
        udpSocket.bind(localPort);

        //insertTotable(15,20,vec3);
        readCSV();
        //readEdgesAggCSV();
        //readEdgesCoresCSV();
        readEdgesEdgCSV();

        int k = getParentModule()->getParentModule()->par("k");

//        SegmentId seg=1;
//        for(int i=0; i<(k+1); i++) {
//            SIDVector vec;
//            for(int j=0; j<k; j++) {
//                vec.push_back(seg);
//                seg++;
//            }
//            pathsInPods.push_back(vec);
//        }

        //                  //edges_set.insert(std::pair<edge,edge>(testEdge,testEdge)  );
        //                  edges_set.insert(std::pair<edge,edge>(testEdge,testEdge)  );
        //                //  edge testEdge5=++testEdge3;
        //             auto myit=edges_set.find(testEdge);
        //             if(myit!=edges_set.end())
        //                          {
        //                          myit->second.setLoad(5);
        //                          }
        //
        //
        //
        //
        //
        //
        //                 // edge testEdge(2,6);
        //                //  edge testEdge2(6,14);
        //
        //             auto myit2=edges_set.find(testEdge2);
        //                  if(myit2!=edges_set.end())
        //                               {
        //                               myit->second.setLoad(5);
        //                               }
        //
        init_edges_set();
        scheduleAt(simTime() + 71, new cMessage);
    }

    //    wait for ospf
    if (stage == INITSTAGE_APPLICATION_LAYER) {
        ospf->registerOpaqueListener(this, OSPFOpaqueType::EXTENDED_LINK_TYPE,
                0);
        ospf->registerOpaqueListener(this, OSPFOpaqueType::EXTENDED_PREFIX_TYPE,
                0);
        msgTimeInsysStats.setName("Whyyyyyyyyyyyyyyyyyyyyyy 0");

    }
    int k = getParentModule()->getParentModule()->par("k");
    sigmaAggtoCore = 1;
    sigmaEdgetoAgg = k;
    sigmaEdgetoAgg = sigmaEdgetoAgg / 2;
    sigmaStoT = k * k / 4;

    pFile = fopen("/home/chen/Desktop/loadFiles/loadFile.txt", "w+");
}

//IOpaqueListener
void NetworkController::notify(OSPFOpaqueLSA *lsa) {
    Enter_Method_Silent();
    EV << "Received LSA update: " << *lsa;
    lsaDatabase->updateLSAinMap(std::unique_ptr<OSPFOpaqueLSA>(lsa));
}
void NetworkController::init_edges_set() {
    int k = getParentModule()->getParentModule()->par("k");
    int i;

    //yodars: numofcores
//    for(i=1; i <= k*(k+1); i++)
//    {
//
//        vector <edge> vec;
//        vec=edgeCores_Location[i];
//        for (unsigned int j=0; j<vec.size();j++)
//        {
//            edge testEdge=vec[j];
//            edges_set.insert(std::pair< const edge,edge>(testEdge,testEdge)  );
//            edges_set_est.insert(std::pair< const edge,edge>(testEdge,testEdge)  );
//        }
//
//    }
//    numOfAggs
//    for(i=(k*k)/4+2; i<=3*(k*k)/4+1; i++)
//    {
//
//        vector <edge> vec;
//        vec=edgeAgg_Location[i];
//        for (unsigned int j=0; j<vec.size();j++)
//        {
//            edge testEdge=vec[j];
//            edges_set.insert(std::pair< const edge,edge>(testEdge,testEdge)  );
//            edges_set_est.insert(std::pair< const edge,edge>(testEdge,testEdge)  );
//
//        }
//    }
    // numOfEdges
    int num_of_edges = getParentModule()->getParentModule()->par(
            "num_of_edges");

    for (i = 1; i <= num_of_edges; i++) {
        vector<edge> vec;
        vec = edgeEdg_Location[i];
        for (unsigned int j = 0; j < vec.size(); j++) {
            edge testEdge = vec[j];
            edges_set.insert(std::pair<const edge, edge>(testEdge, testEdge));
            edges_set_est.insert(
                    std::pair<const edge, edge>(testEdge, testEdge));
        }
    }
}

void NetworkController::handleTimer(cMessage *msg) {
    EV << "HEY!" << endl;
    NodeController *mizrahi;
    double *load;
    int i, j, l, k, num;
    edge e;
    char edgesStr[] = "^.^.Edges[*].springNodeController";
    char aggStr[] = "^.^.Aggregation[*].springNodeController";
    char coresStr[] = "^.^.Cores[*].springNodeController";
    k = getParentModule()->getParentModule()->par("k");
    msgTimeInsysStats.record(k + simTime());/**********************/
    if (firstSelfmsg == false) {
        firstSelfmsg = true;
        network->construct(*lsaDatabase);
        network->initializeShortestPaths();
    } else if (simTime() > SIMTIMESTART) {

        //        /*
        //         * get here and take out all the data from all the nodes
        //         */
        //
        //
        //        for(i=0; i<(k*k)/4; i++) {
        //            //  coresStr[10]=i+48;
        //            sprintf(coresStr,"^.^.Cores[%d].springNodeController",i);
        //            mizrahi= check_and_cast<NodeController *>(getModuleByPath(coresStr));
        //            load =mizrahi->getChanels();
        //            int loc=0;
        //            for (int kk=0; kk< 10; kk++){
        //                if (load[kk]!=-1)
        //                {
        //                    edge testEdge;
        //                    edgeCores_Location[mizrahi->getNodalSID()][loc].setLoad(load[kk]);
        //                    testEdge=edgeCores_Location[mizrahi->getNodalSID()][loc];
        //                    auto myit=edges_set.find(testEdge);
        //                    if(myit!=edges_set.end())
        //                    {
        //                        myit->second.setLoad(load[kk]);
        //
        //                    }
        //                    //                    edges_set[edgeCores_Location[mizrahi->getNodalSID()][loc]].setLoad(load[kk]);
        //                    //                edges_set[edgeCores_Location[mizrahi->getNodalSID()][loc]]=edgeCores_Location[mizrahi->getNodalSID()][loc];
        //                    loc++;
        //                }
        //            }
        //        }
        //
        //        for(i=0; i<(k*k)/2; i++) {
        //            // aggStr[16]=i+48;
        //            sprintf(aggStr,"^.^.Aggregation[*].springNodeController",i);
        //            mizrahi= check_and_cast<NodeController *>(getModuleByPath(aggStr));
        //            load =mizrahi->getChanels();
        //
        //            int loc=0;
        //            for (int kk=0; kk< 10; kk++){
        //                if (load[kk]!=-1)
        //                {
        //                    edgeAgg_Location[mizrahi->getNodalSID()][loc].setLoad(load[kk]);
        //                    edge testEdge;
        //                    edgeAgg_Location[mizrahi->getNodalSID()][loc].setLoad(load[kk]);
        //                    testEdge=edgeAgg_Location[mizrahi->getNodalSID()][loc];
        //                    auto myit=edges_set.find(testEdge);
        //                    if(myit!=edges_set.end())
        //                    {
        //                        myit->second.setLoad(load[kk]);
        //
        //                    }
        //                    //  edges_set[edgeAgg_Location[mizrahi->getNodalSID()][loc]]=edgeAgg_Location[mizrahi->getNodalSID()][loc];
        //                    //      edges_set[edgeAgg_Location[mizrahi->getNodalSID()][loc]].setLoad(load[kk]);
        //
        //                    loc++;
        //                }
        //            }
        //        }
        //
        //        for(i=0; i<(k*k)/2; i++) {
        //            //           edgesStr[10]=i+48;
        //            sprintf(edgesStr,"^.^.Edges[*].springNodeController",i);
        //            mizrahi= check_and_cast<NodeController *>(getModuleByPath(edgesStr));
        //            load =mizrahi->getChanels();
        //
        //            int loc=0;
        //            for (int kk=0; kk< 10; kk++){
        //                if (load[kk]!=-1)
        //                {
        //                    edgeEdg_Location[mizrahi->getNodalSID()][loc].setLoad(load[kk]);
        //                    edge testEdge;
        //                    //    edgeEdg_Location[mizrahi->getNodalSID()][loc].setLoad(load[kk]);
        //                    testEdge=edgeEdg_Location[mizrahi->getNodalSID()][loc];
        //                    auto myit=edges_set.find(testEdge);
        //                    if(myit!=edges_set.end())
        //                    {
        //                        myit->second.setLoad(load[kk]);
        //                    }
        //                    // myit.setDelay(load[kk]);
        //                    loc++;
        //                }
        //            }
        //        }
        //        //also find best path.

        //         mizrahi= check_and_cast<NodeController *>(getModuleByPath(edgesStr));
        //        double *load =mizrahi->getChanels();
        //        setDelay2();

//      setDelay3();
////
//     loadEstEdge();
//
//        // if(simTime()>83) loadFile();
//        clacDelaysOnAllPaths();
    }
    //TODO where to put this??
    // loadEstEdge() ;
    if (simTime() > 84) {    //TODO change start time;
        loadEstEdge();
        setDelay3();
        clacDelaysOnAllPaths();
        if ( RUNALGFLAG)
            Congestion_Detection();
        if (simTime() > 83)
            loadFile();

    }

    // if(simTime()>83) loadFile();

    //
    scheduleAt(simTime() + SAMPTIME, msg);

}

void NetworkController::setDelay2() {
    int k = getParentModule()->getParentModule()->par("k");
    // char edgesStr[]="^.^.Edges[*].springNodeController";
    // char aggStr[]="^.^.Aggregation[*].springNodeController";
    // char coresStr[]="^.^.Cores[*].springNodeController";
    char edgesStr[50];    //="^.^.Edges[*].springNodeController";
    char aggStr[50];    //="^.^.Aggregation[*].springNodeController";
    char coresStr[50];    //="^.^.Cores[*].springNodeController";
    k = getParentModule()->getParentModule()->par("k");
    int i;
    NodeController *node;

    double *delArr;
//    for(i=0; i<(k*k)/4; i++) {
//        //coresStr[10]=i+48;
//        sprintf(coresStr,"^.^.Cores[%d].springNodeController",i);
//        node= check_and_cast<NodeController *>(getModuleByPath(coresStr));
//        delArr =node->getDelays();
//        int loc=0;
//        for (int kk=0; kk< 4; kk++){
//            if (delArr[kk]!=-1)
//            {
//                edgeCores_Location[node->getNodalSID()][loc].setDelay(delArr[kk]);
//                // edges_set[edgeCores_Location[node->getNodalSID()][loc]]=edgeCores_Location[node->getNodalSID()][loc];
//                //  edges_set[ edgeCores_Location[node->getNodalSID()][loc]].setDelay(delArr[kk]);
//                loc++;
//            }
//        }
//    }
//
//    for(i=0; i<(k*k)/2; i++) {
//        // aggStr[16]=i+48;
//        sprintf(aggStr,"^.^.Aggregation[*].springNodeController",i);
//        node= check_and_cast<NodeController *>(getModuleByPath(aggStr));
//        delArr =node->getDelays();
//
//        int loc=0;
//        for (int kk=0; kk< 4; kk++){
//            if (delArr[kk]!=-1)
//            {
//                edgeAgg_Location[node->getNodalSID()][loc].setDelay(delArr[kk]);
//                //  edges_set[edgeAgg_Location[node->getNodalSID()][loc]]=edgeAgg_Location[node->getNodalSID()][loc];
//                // edges_set[edgeAgg_Location[node->getNodalSID()][loc]].setDelay(delArr[kk]);
//                loc++;
//            }
//        }
//    }
    int num_of_edges = getParentModule()->getParentModule()->par(
            "num_of_edges");

    for (i = 0; i < num_of_edges; i++) {
        sprintf(edgesStr, "^.^.Edges[*].springNodeController", i);
        // edgesStr[10]=i+48;
        node = check_and_cast<NodeController *>(getModuleByPath(edgesStr));
        delArr = node->getDelays();

        int loc = 0;
        for (int kk = 0; kk < 4; kk++) {
            if (delArr[kk] != -1) {
                edgeEdg_Location[node->getNodalSID()][loc].setDelay(delArr[kk]);
                //  edges_set[edgeEdg_Location[node->getNodalSID()][loc]]=edgeEdg_Location[node->getNodalSID()][loc];
                // edges_set[edgeEdg_Location[node->getNodalSID()][loc]].setDelay(delArr[kk]);
                loc++;
            }
        }
    }

}

void NetworkController::setDelay3() {
    int k = getParentModule()->getParentModule()->par("k");
    // char edgesStr[]="^.^.Edges[*].springNodeController";
    // char aggStr[]="^.^.Aggregation[*].springNodeController";
    // char coresStr[]="^.^.Cores[*].springNodeController";
    char edgesStr[50];    //="^.^.Edges[*].springNodeController";
    char aggStr[50];    //="^.^.Aggregation[*].springNodeController";
    char coresStr[50];    //="^.^.Cores[*].springNodeController";
    k = getParentModule()->getParentModule()->par("k");
    int num_of_edges = getParentModule()->getParentModule()->par(
            "num_of_edges");
    int i, count1 = 0, count2 = 0, count3 = 0;
    NodeController *node;
    //    edges_set.clear();
    //yodars
//    for(i=0; i<(k*k)/4; i++) {
//        //coresStr[10]=i+48;
//        sprintf(coresStr,"^.^.Cores[%d].springNodeController",i);
//        node= check_and_cast<NodeController *>(getModuleByPath(coresStr));
//        auto delArr =node->getAllLoadsAndDelays();
//        std::map<edge,edge>::iterator it;
//        for (it=delArr.begin(); it!=delArr.end(); ++it){
//            edges_set[it->first].setLoad(it->second.getLoad());
//            edges_set[it->first].setDelay(it->second.getDelay());
//            count1++;
//        }
//    }
//
//    for(i=0; i<(k*k)/2; i++) {
//        // aggStr[16]=i+48;
//        sprintf(aggStr,"^.^.Aggregation[%d].springNodeController",i);
//        node= check_and_cast<NodeController *>(getModuleByPath(aggStr));
//        auto delArr =node->getAllLoadsAndDelays();
//        std::map<edge,edge>::iterator it;
//        for (it=delArr.begin(); it!=delArr.end(); ++it){
//            edges_set[it->first].setLoad(it->second.getLoad());
//            edges_set[it->first].setDelay(it->second.getDelay());
//            count2++;
//        }
//    }

    for (i = 0; i < num_of_edges; i++) {
        sprintf(edgesStr, "^.^.Edges[%d].springNodeController", i);
        // edgesStr[10]=i+48;
        node = check_and_cast<NodeController *>(getModuleByPath(edgesStr));
        auto delArr = node->getAllLoadsAndDelays();
        std::map<edge, edge>::iterator it;
        for (it = delArr.begin(); it != delArr.end(); ++it) {
            edges_set[it->first].setLoad(it->second.getLoad());
            edges_set[it->first].setDelay(it->second.getDelay());
            count3++;
        }
    }

    if (simTime() > 82.3) {
        int ggg = 'p' * 'o';
    }
}

void NetworkController::sendEROS(SegmentId srcSid, L3Address dstAddr,
        int dstPort) {
    UDPEros * udpEros = new UDPEros;
    network->getAllEROsFrom(srcSid, udpEros->getEros());
    udpSocket.sendTo(udpEros, dstAddr, dstPort);
}

void NetworkController::updateStats(SegmentId srcSid, StatsVector& stats) {
    for (auto &it : stats) {
        SegmentId dstSid;

        bool valid = srgb->getByIndex(it.first, dstSid);
        ASSERT(valid);
        network->updateTM(srcSid, dstSid, it.second);

    }
}

void NetworkController::handleMessage(cMessage *msg) {

    if (msg->isSelfMessage()) {
        handleTimer(msg);
    } else if (msg->getArrivalGateId() == gate("udpIn")->getId()
            && msg->getKind() == UDP_I_DATA) {

        UDPStats * packet = dynamic_cast<UDPStats*>(msg);
        ASSERT(packet);

        SegmentId srcSid;
        bool valid = srgb->getByIndex(packet->getOrigSid(), srcSid);
        ASSERT(valid);

        //        StatsVector &stats = packet->getStats();
        //        updateStats(srcSid, stats); TODO fix this if we want to collect stats CHEN AND ITAY AND CHLE AND EYAL

        UDPDataIndication * udpData =
                dynamic_cast<UDPDataIndication*>(packet->getControlInfo());
        ASSERT(udpData!=NULL);
        sendEROS(srcSid, udpData->getSrcAddr(), udpData->getSrcPort());

        delete msg;
    } else {
        throw cRuntimeError("Unexpected message");
    }
}

/*
 void NetworkController::insertTotable(SegmentId dst,SegmentId src, std::vector<std::vector<SegmentId>> vec) {
 int i, flag=0;
 for(i=0; i<PATHTABLESIZE && flag==0; i++) {
 if(oriZohar[i].src==-1 && oriZohar[i].dst==-1) {
 oriZohar[i].src=src;
 oriZohar[i].dst=dst;
 oriZohar[i].vec=vec;
 flag=1;
 }
 }
 }
 */

/*
 * Giving us the initial route which the flow passes through.
 */
SIDVector NetworkController::getFirstPath(cGate* gate, SPRINGPacket* packet,
        SegmentId dest, SegmentId src) {
    SIDVector firstPath;
    SegmentId currentSeg = 0;
    cGate* currentGate = gate;
    //    firstPath.push_back(src);

    while (currentSeg != dest) {
        auto springModule =
                currentGate->getPathEndGate()->getOwnerModule()->getParentModule()->getSubmodule(
                        "ppp")->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule()->getParentModule()->getSubmodule(
                        "spring");
        currentSeg = ((SPRING*) springModule)->reclassif->getSelfSID();
        firstPath.push_back(currentSeg);
        if (currentSeg == dest)
            break;
        SPRINGPacket* packet2 = packet->dup();
        currentGate = ((SPRING*) springModule)->getOutputGate(packet2);
        delete packet2;

    }
    return firstPath;
}

/*
 * Putting the best route in the variable route.
 */
bool NetworkController::getBestPath(SegmentId src, SegmentId dst,
        SIDVector & route) {

    int i;
    double min;
    SIDVector noPath;
    int numberOfPathsFromEdgesToEdges = getParentModule()->getParentModule()->par(
            "numberOfPathsFromEdgesToEdges");
    for (i = 0; i < numberOfPathsFromEdgesToEdges; i++) { // find the path between the 2
        if (fullbPathTable[i].src == src && fullbPathTable[i].dst == dst)
            break;
    }
    //    if(simTime()>86) {
    //        int p=1;
    //        p++;
    //    }

    /*
     * Looking for the best path between all paths (short and long).
     */
    if (i < numberOfPathsFromEdgesToEdges) { // if found a path between the 2
        min = fullbPathTable[i].longpaths[0].second;
        route = fullbPathTable[i].longpaths[0].first;
        for (unsigned int j = 0; j < fullbPathTable[i].shortPaths.size(); j++) { // find the shortest 'short path'
            if (min > fullbPathTable[i].shortPaths[j].second) {
                min = fullbPathTable[i].shortPaths[j].second;
                route = fullbPathTable[i].shortPaths[j].first;
            }
        }
        for (unsigned int j = 1; j < fullbPathTable[i].longpaths.size(); j++) { // find the shortest 'long path'
            if (min > fullbPathTable[i].longpaths[j].second) {
                min = fullbPathTable[i].longpaths[j].second;
                route = fullbPathTable[i].longpaths[j].first;
            }
        }

        return true;
    } else
        return false;
}

void NetworkController::readEdgesAggCSV() {
    /*
     * CSV parsers.
     * Only for k=4
     * Assumes that each pair of source destination have 4 short paths and 4 long paths
     * TODO generalize for any k
     */

    vector<vector<string> > data;
    SegmentId head;
    ifstream infile("/home/chen/Desktop/AggregationList.csv");
    int i = 0, lines = 0;
    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            record.push_back(s);
        }

        data.push_back(record);
        lines++;
    }

    unsigned int j;

    for (i = 0; i < lines;) {
        vector<edge> edgeslist22;
        for (j = 0; j < 5; j++) {
            if (j == 0) {
                head = std::stoi(data[i][0]);
            } else {
                vector<string> pathstring = data[i];

                SegmentId right = std::stoi(pathstring[0]);
                SegmentId left = std::stoi(pathstring[1]);
                edge temp1(right, left);
                edgeslist22.push_back(temp1);
            }
            i++;
        }
        edgeAgg_Location.insert(
                std::pair<SegmentId, std::vector<edge>>(head, edgeslist22));
    }

    /*
     if (!infile.eof())
     {
     cerr << "Fooey!\n";
     }*/

}

void NetworkController::readEdgesCoresCSV() {
    //csv parsers
    //only for k=4
    //Assumes that each pair of src dst have 4 short paths and 4 long paths
    //TODO generalize for any k
    vector<vector<string> > data;
    SegmentId head;
    ifstream infile("/home/chen/Desktop/CoresList2.csv");
    int i = 0, lines = 0;
    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            record.push_back(s);
        }

        data.push_back(record);
        lines++;
    }

    unsigned int j, k = 0, m = 0;

    for (i = 0; i < lines;) {
        vector<edge> coreslist22;
        for (j = 0; j < 5; j++) {
            if (j == 0) {
                head = std::stoi(data[i][0]);
            } else {
                vector<string> pathstring = data[i];

                SegmentId right = std::stoi(pathstring[0]);
                SegmentId left = std::stoi(pathstring[1]);
                edge temp1(right, left);
                coreslist22.push_back(temp1);
            }
            i++;
        }
        edgeCores_Location.insert(
                std::pair<SegmentId, std::vector<edge>>(head, coreslist22));
    }

    /*
     if (!infile.eof())
     {
     cerr << "Fooey!\n";
     }*/

}

void NetworkController::readEdgesEdgCSV() {
    //csv parsers
    //only for k=4
    //Assumes that each pair of src dst have 4 short paths and 4 long paths
    //TODO generalize for any k
    vector<vector<string> > data;
    SegmentId head;
    string path_to_edges_list = getParentModule()->getParentModule()->par(
            "path_to_edges_list");
    ifstream infile(path_to_edges_list);
    int i = 0, lines = 0;
    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            record.push_back(s);
        }

        data.push_back(record);
        lines++;
    }

    unsigned int j, k = 0, m = 0;
    int num_of_connections_per_edge_without_clients =
            getParentModule()->getParentModule()->par(
                    "num_of_connections_for_each_core_without_clients");

    for (i = 0; i < lines;) {
        vector<edge> edgeslist22;
        for (j = 0; j < num_of_connections_per_edge_without_clients + 1; j++) {
            if (j == 0) {
                head = std::stoi(data[i][0]);
            } else {
                vector<string> pathstring = data[i];

                SegmentId right = std::stoi(pathstring[0]);
                SegmentId left = std::stoi(pathstring[1]);
                edge temp1(right, left);
                edgeslist22.push_back(temp1);
            }
            i++;
        }
        edgeEdg_Location.insert(
                std::pair<SegmentId, std::vector<edge>>(head, edgeslist22));
    }


     if (!infile.eof())
     {
     cerr << "Fooey!\n";
     }

}

edge NetworkController::findEdge(edge e) {
    std::map<SegmentId, vector<edge>>::iterator it;
    edge null;

    for (it = edgeAgg_Location.begin(); it != edgeAgg_Location.end(); ++it) {
        vector<edge> vec = it->second;

        for (unsigned int j = 0; j < vec.size(); j++) {

            if (e == vec[j]) {
                return vec[j];
            }
        }
    }

    for (it = edgeCores_Location.begin(); it != edgeCores_Location.end();
            ++it) {
        vector<edge> vec = it->second;

        for (unsigned int j = 0; j < vec.size(); j++) {

            if (e == vec[j]) {
                return vec[j];
            }
        }
    }

    for (it = edgeEdg_Location.begin(); it != edgeEdg_Location.end(); ++it) {
        vector<edge> vec = it->second;

        for (unsigned int j = 0; j < vec.size(); j++) {

            if (e == vec[j]) {
                return vec[j];
            }
        }
    }

    return null;
}

void NetworkController::readCSV() {
    //csv parsers
    //only for k=4
    //Assumes that each pair of src dst have 4 short paths and 4 long paths
    //TODO generalize for any k

    //yodars: this function read the neighbors from the file
    vector<vector<string> > data;
    string topologyName =
            getParentModule()->getParentModule()->par(
                    "topologyName");
    if (topologyName.find("DragonflyTopology") == string::npos) {
        readVariousCSV();
        return;
    }
    string path_paths_between_edges_list =
                getParentModule()->getParentModule()->par(
                        "path_paths_between_edges_list");
    ifstream infile(path_paths_between_edges_list);
    int i = 0, lines = 0;
    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            if (s != "" && s != "\r")
                record.push_back(s);
        }

        data.push_back(record);
        lines++;
    }
    unsigned int j, k = 0, m = 0;

    // yodars: i indicates the line in the file,
    // j indicates the part of the file, each source
    // and dest have 8 paths, and 2 other for source and dest
    // k indicates the couple's number
    for (i = 0; i < lines;)

    {
        pathTable tmp;
        for (j = 0; j < 24; j++) {
            if (j == 0) { // source
                fullbPathTable[k].src = std::stoi(data[i][0]);
                tmp.src = std::stoi(data[i][0]);
            } else if (j == 1) // dest
                    {
                vector<string> test = data[i];
                fullbPathTable[k].dst = std::stoi(data[i][0]);
                tmp.dst = std::stoi(data[i][0]);
            } else if (j < 4) // there are 2 short paths for each couple
                    {
                vector<string> pathstring = data[i];
                vector<SegmentId> path;
                for (m = 0; m < pathstring.size(); m++)
                    path.push_back(std::stoi(pathstring[m]));

                // fullbPathTable[k].shortPaths.push_back(path);
                fullbPathTable[k].shortPaths.push_back(pathAnddelay(path, -1));
                tmp.shortPaths.push_back(pathAnddelay(path, -1));

            } else { // there are 20 long paths for each couple
                vector<string> pathstring = data[i];
                vector<SegmentId> path;
                for (m = 0; m < pathstring.size(); m++)
                    path.push_back(std::stoi(pathstring[m]));
                fullbPathTable[k].longpaths.push_back(pathAnddelay(path, -1));
                tmp.longpaths.push_back(pathAnddelay(path, -1));
            }
            i++;
        }
        k++;
        fullbPathTable2.push_back(tmp);
    }

    if (!infile.eof()) {
        cerr << "Fooey!\n";
    }

}

void NetworkController::readVariousCSV() {
    //csv parsers
    //only for k=4
    //Assumes that each pair of src dst have 4 short paths and 4 long paths
    //TODO generalize for any k

    //yodars: this function read the neighbors from the file
    vector<vector<string> > data;
    string path_paths_between_edges_list =
            getParentModule()->getParentModule()->par(
                    "path_paths_between_edges_list");
    ifstream infile(path_paths_between_edges_list);
    int i = 0, lines = 0;
    while (infile) {
        string s;
        if (!getline(infile, s))
            break;

        istringstream ss(s);
        vector<string> record;

        while (ss) {
            string s;
            if (!getline(ss, s, ','))
                break;
            if (s != "" && s != "\r" && s.find("\"") == string::npos)
                record.push_back(s);
        }

        data.push_back(record);
        lines++;
    }
    unsigned int j, k = 0, m = 0;

    // yodars: i indicates the line in the file,
    // j indicates the part of the file, each source
    // and dest have 8 paths, and 2 other for source and dest
    // k indicates the couple's number
    for (i = 0; i < lines;)

    {
        pathTable tmp;
        int numOfShortPath = std::stoi(data[i + 2][0]);
        int numOfLongPath = std::stoi(data[i + 3][0]);
        int vecLength = 4 + numOfShortPath + numOfLongPath;
        for (j = 0; j < vecLength; j++) {
            if (j == 0) { // source
                fullbPathTable[k].src = std::stoi(data[i][0]);
                tmp.src = std::stoi(data[i][0]);
            } else if (j == 1) // dest
                    {
                vector<string> test = data[i];
                fullbPathTable[k].dst = std::stoi(data[i][0]);
                tmp.dst = std::stoi(data[i][0]);
            } else if (j < 4) // dest
                    {
                //do nothing
            } else if (j < 4 + numOfShortPath) // there are 2 short paths for each couple
                    {
                vector<string> pathstring = data[i];
                vector<SegmentId> path;
                for (m = 0; m < pathstring.size(); m++)
                    path.push_back(std::stoi(pathstring[m]));

                // fullbPathTable[k].shortPaths.push_back(path);
                fullbPathTable[k].shortPaths.push_back(pathAnddelay(path, -1));
                tmp.shortPaths.push_back(pathAnddelay(path, -1));

            } else { // there are 20 long paths for each couple
                vector<string> pathstring = data[i];
                vector<SegmentId> path;
                for (m = 0; m < pathstring.size(); m++)
                    path.push_back(std::stoi(pathstring[m]));
                fullbPathTable[k].longpaths.push_back(pathAnddelay(path, -1));
                tmp.longpaths.push_back(pathAnddelay(path, -1));
            }
            i++;
        }
        k++;
        fullbPathTable2.push_back(tmp);
    }

    if (!infile.eof()) {
        cerr << "Fooey!\n";
    }

}

double NetworkController::findDelayOnPath(vector<SegmentId> path) {
    double total = 0;

    for (uint32 i = 0; i < path.size() - 1; i++) {
        edge currE(path[i], path[i + 1]);
        total +=  edges_set[currE].getDelay();
    }
    return total;
}

double NetworkController::findLoadOnPath(vector<SegmentId> path) {
    double total = 0;

    for (uint32 i = 0; i < path.size() - 1; i++) {
        edge currE(path[i], path[i + 1]);
        total += findEdge(currE).getLoad();
    }
    return total;
}

void NetworkController::clacDelaysOnAllPaths() {
    int i;
    //    if(simTime()>85) {
    //        int t=10;
    //    }
    int numberOfPathsFromEdgesToEdges = getParentModule()->getParentModule()->par(
            "numberOfPathsFromEdgesToEdges");
    for (i = 0; i < numberOfPathsFromEdgesToEdges; i++) {

        vector<SegmentId> path;
        for (unsigned int j = 0; j < fullbPathTable[i].longpaths.size(); j++) {
            path = fullbPathTable[i].longpaths[j].first;
            fullbPathTable[i].longpaths[j].second = findDelayOnPath(path);
            if (findDelayOnPath(path) > 0) { //yodars: seems useless
                double tt = findDelayOnPath(path);
                tt++;
            }
        }
        for (unsigned int j = 0; j < fullbPathTable[i].shortPaths.size(); j++) {
            path = fullbPathTable[i].shortPaths[j].first;
            fullbPathTable[i].shortPaths[j].second = findDelayOnPath(path);
        }

    }
}
/*
 * this function is used to find the divsor for small flows
 * @params int i this is the stage of the short path, ie hop number
 * TODO :since this an ad-hoc formula we need to test these values for K!=4,
 */
int NetworkController::howToDivaide(int i) {
    string topologyName = getParentModule()->getParentModule()->par(
            "topologyName");
    if (topologyName == "DragonflyTopologyH2") {
        return 2;
    }
    return 1;
    //yodars: may need to change
//    int k=getParentModule()->getParentModule()->par("k");
//
//    if(i%4==0 || i%4==2){
//        return k*k/8;
//    }
//    else
//        return k*k/4;
}

/**
 * this function estimates the load I.E. the number of bytes on each edge in the topology
 */
void NetworkController::loadEstEdge() {

    SPRING *edgeNode;
    SegmentId seg;
    int k = getParentModule()->getParentModule()->par("k");
    int num_of_edges = getParentModule()->getParentModule()->par(
            "num_of_edges");
    char edgesStr[30];
    int count = 0;
    /*
     * first we clear all the values in the edge table
     */
    for (std::map<edge, edge>::iterator it = edges_set_est.begin();
            it != edges_set_est.end(); ++it) {
        it->second.setLoad(0);
        count++;
    }

    //    if(simTime()>83) {
    //        int yu=10;
    //    }

    /*
     * for each large flow we have this part.
     * just take the size of the flow and add it to each of the edges int its path
     */for (int i = 0; i < num_of_edges; i++) {
        sprintf(edgesStr, "^.^.Edges[%d].spring", i);
        edgeNode = check_and_cast<SPRING *>(getModuleByPath(edgesStr));
        RoutingAlg *alg = edgeNode->getAlg();
        Reclassifier *rec = edgeNode->getReclassifier();

        std::map<ElephentEntry, MovingWindow> elephantTable =
                alg->getElephantTable();
        for (std::map<ElephentEntry, MovingWindow>::iterator it =
                elephantTable.begin(); it != elephantTable.end(); ++it) {
            ElephentEntry entry = it->first;
            MovingWindow window = it->second;
            long unsigned int totalOfBytes = window.getTotal();
            SIDVector pathForEntry = rec->pathForFlow(entry);
            SegmentId v1 = rec->getSelfSID();
            for (unsigned int j = 0; j < pathForEntry.size(); j++) {
                SegmentId v2 = pathForEntry[j];
                edge curEdge(v1, v2);

                auto res = edges_set_est.find(curEdge);
                if (res != edges_set_est.end()) {
                    double curLoad = res->second.getLoad() + totalOfBytes;
                    curEdge.setLoad(curLoad);
                    edges_set_est.erase(res);
                    edges_set_est.insert(
                            std::pair<edge, edge>(curEdge, curEdge));
                }
                v1 = v2;
            }
        }

    }
    int tester2 = 0;

    /*
     * this next part will be used to applay the wieghts of small flows in the network to the "graph" or our edge container
     * two parts are used, first part is for when the src and dst of the flow are the same pod and the other is for when they are in the same
     * in part 1:  for each edge node we take its mice flow table, each index of this arry represents a dst.
     *   than we take our paths container and get for each pair of src dst its short paths, for each short path we take all of its edges,
     *   and for each edge we add up the number of bits for that small flow, takeing care the divde each value by  the number we ger from howToDivaide()
     *
     * in part 2: we take each flow that is the same pod. if a flow is in the same pod this means it's path lengt will aleays be 2 for any value of K
     *   therefor we make a path for each of the egdes on the pod and update them with the currect number of byts
     *
     */
    //yodars
    //for(int i=0; i<(k*k)/2-1; i++) { //for each src node
    for (int i = 0; i < num_of_edges; i++) { //for each src node
        sprintf(edgesStr, "^.^.Edges[%d].spring", i); //this gets the curr node ie the the current src node
        edgeNode = check_and_cast<SPRING *>(getModuleByPath(edgesStr));
//         int k=getParentModule()->getParentModule()->par("k");
        RoutingAlg *alg = edgeNode->getAlg();
        Reclassifier *rec = edgeNode->getReclassifier();
        SegmentId selfSID = rec->getSelfSID(); //selfSID of src
        //  MovingWindow* miceTable=alg->getMiceFlows();
        for (int j = 0; j < num_of_edges; j++) { //for each dst node
            if (j != i) {
                int dst = j + 1;
                int src = selfSID;

                sprintf(edgesStr, "^.^.Edges[%d].spring", j);
                SPRING * edgeNodeDst = check_and_cast<SPRING *>(
                        getModuleByPath(edgesStr));
                MovingWindow* window = edgeNode->getAlg()->getMiceFlows();
                int numberOfPathsFromEdgesToEdges = getParentModule()->getParentModule()->par(
                        "numberOfPathsFromEdgesToEdges");
                for (int l = 0; l < numberOfPathsFromEdgesToEdges; l++) { //serach for paths in the table
                    string topologyName = getParentModule()->getParentModule()->par(
                            "topologyName");
                    if ((topologyName != "DragonflyTopologyH2") || (edgeNodeDst->getPodIndex() != edgeNode->getPodIndex() )) {
                        if (fullbPathTable[l].src == src
                                && fullbPathTable[l].dst == dst) {
                            for (unsigned int m = 0;m < fullbPathTable[l].shortPaths.size();m++) { //for each short path
                                std::vector<SegmentId> shortPath =
                                        fullbPathTable[l].shortPaths[m].first;
                                SegmentId v1 = rec->getSelfSID();
                                //yodars
                                //long unsigned int totalOfBytes=window[edgeNodeDst->getReclassifier()->getSelfSID()-((k*k / 4) + (k*k / 2) + 2)].getTotal();
                                long unsigned int totalOfBytes =
                                        window[edgeNodeDst->getReclassifier()->getSelfSID()
                                                - (1)].getTotal();
                                if (totalOfBytes > 0) {
                                    int yy = 5;
                                }
                                for (unsigned int n = 1; n < shortPath.size();
                                        n++) { //for each edge in a short path //starts with n=1 since the first SID in path is it self
                                    SegmentId v2 = shortPath[n];
                                    edge curEdge(v1, v2);
                                    auto edgeEst = edges_set_est.find(curEdge);
                                    if (edgeEst != edges_set_est.end()) {
                                        double curLoad = floor(
                                                edgeEst->second.getLoad()
                                                        + totalOfBytes
                                                                / fullbPathTable[l].shortPaths.size()); //TODO:yodars add it if needed in other topologies /howToDivaide(n-1)
                                        curEdge.setLoad(curLoad);
                                        edges_set_est.erase(edgeEst);
                                        edges_set_est.insert(
                                                std::pair<edge, edge>(curEdge,
                                                        curEdge));
                                    }
                                    v1 = v2;
                                }
                            }
                        }
                    }
                    // yodars: there are no aggregations on our file.
//                     else {
//
//                         if (simTime()>83.5 &&rec->getSelfSID()==16 ){
//                             int gg=5;
//                             gg++;
//                         }
//                         long unsigned int totalOfBytes=window[j].getTotal();
//                         SegmentId v1=rec->getSelfSID();
//                         int numOfAggs=pathsInPods[edgeNode->getPodIndex()].size();//divide by this number... this should be 2 for k=4 and 4 for k=8
//                         for (int b=0;b<pathsInPods[edgeNode->getPodIndex()].size();b++){
//
//
//                             SegmentId v2=pathsInPods[edgeNode->getPodIndex()][b];
//                             edge Edge1(v1,v2);
//                             edge Edge2(v2,edgeNodeDst->getReclassifier()->getSelfSID());
//                             auto res=edges_set_est.find( Edge1);
//
//                             if(res!=edges_set_est.end()){
//                                 double curLoad=floor((res->second.getLoad()+totalOfBytes)/numOfAggs);
//
//                                 Edge1.setLoad(curLoad);
//                                 if (curLoad>0){
//                                     tester2++;
//                                 }
//                                 edges_set_est.erase(res);
//                                 edges_set_est.insert(std::pair<edge,edge>(Edge1,Edge1));
//                             }
//                             res=edges_set_est.find( Edge2);
//                             if(res!=edges_set_est.end()){
//                                 double curLoad=floor((res->second.getLoad()+totalOfBytes)/numOfAggs);
//                                 Edge2.setLoad(curLoad);
//                                 edges_set_est.erase(res);
//                                 edges_set_est.insert(std::pair<edge,edge>(Edge2,Edge2));
//
//
//                             }
//                             //src and dst in the same pod.
//                         }
//                     }
                }
            }

        }
    }
    if (simTime() > 86.0) {

        std::vector<edge> tester;
        for (std::map<edge, edge>::iterator it = edges_set_est.begin();
                it != edges_set_est.end(); ++it) {
            if (it->second.getLoad() > 0) {
                tester.push_back(it->second);
            }

        }
        tester2 = tester2;
        int pp = 1;
        pp++;

    }
}

/*
 * tests to see if an edge is a part of this vector of edges
 */
bool NetworkController::IstheEdgeInSet(vector<edge> Edges, edge theEdge) {
    bool is = false;
    for (uint32 i = 0; i < Edges.size(); i++) {
        if (Edges[i] == theEdge) {
            is = true;
            break;
        }
    }
    return is;

}

void NetworkController::Congestion_Detection() {
    std::vector<edge> estEdges;
    std::vector<edge> realEdges;
    pathChange = 0;
    std::multiset<Flow> flowsWhoChangedPathAndNotFinished;

    // yodars: definition of loaded edge
    double MaxLoad = MAXLOAD * WINDOWTIME;
    double MaxLoadEst = MAXLOADEST * WINDOWTIME;
//    cout << "test" << endl;

    for (std::map<edge, edge>::iterator it = edges_set_est.begin();
            it != edges_set_est.end(); ++it) {
//        cout << "debug: it->second.getLoad(): " << it->second.getLoad() <<
//                " MaxLoadEst:"<< MaxLoadEst<<endl;

        if (it->second.getLoad() > MaxLoadEst) {
//            cout << "Load is bigger than MAX load1" << endl;

            estEdges.push_back(it->second);
        }

    }

    for (std::map<edge, edge>::iterator it = edges_set.begin();
            it != edges_set.end(); ++it) {
        if (it->second.getLoad() > MaxLoad) {
//            cout << "Load is bigger than MAX load2" << endl;
            int i=0;
            i++;
            realEdges.push_back(it->second);
        }

    }
    /*
     * get all the flows
     */
    std::multiset<Flow> allFlowsNotRerouted;
    std::multiset<Flow> allFlowsOfAllElephants;

    SPRING *edgeNode;
    SegmentId seg;
    int k = getParentModule()->getParentModule()->par("k");
    int num_of_edges = getParentModule()->getParentModule()->par(
            "num_of_edges");

    char edgesStr[30]; //buffer
    //yodars
    for (int i = 0; i < num_of_edges; i++) {

        //    for(int i=0; i<(k*k)/2-1; i++) {
        sprintf(edgesStr, "^.^.Edges[%d].spring", i);
        edgeNode = check_and_cast<SPRING *>(getModuleByPath(edgesStr));
        RoutingAlg *alg = edgeNode->getAlg();
        Reclassifier *rec = edgeNode->getReclassifier();
        std::map<ElephentEntry, MovingWindow> tmpelephantTable =
                alg->getElephantTable();
        std::map<ElephentEntry, MovingWindow> elephantTable;
        std::vector<ElephentEntry> vecE;
        std::vector<ElephentEntry> elphantWithChangedPath;

        std::vector<ElephentEntry> allEdgesOfAllElephant;

        for (std::map<ElephentEntry, MovingWindow>::iterator it =
                tmpelephantTable.begin(); it != tmpelephantTable.end(); ++it) {
            if (alg->IsnewRoute[it->first] == false) {
//                cout << "was in for hommie" << endl;

                //    elephantTable.insert(std::pair<ElephentEntry, MovingWindow> (it->first, MovingWindow (it->second) ));
                vecE.push_back(it->first);
            } else if(segal_alg){
                elphantWithChangedPath.push_back(it->first);
            }
            if (segal_alg){
                allEdgesOfAllElephant.push_back(it->first);
            }

        }

        // for (std::map<ElephentEntry,MovingWindow>::iterator it=elephantTable.begin(); it!=elephantTable.end(); ++it) {
        for (int i = 0; i < vecE.size(); i++) {
            MovingWindow tmpW = tmpelephantTable.find(vecE[i])->second;
            Flow tmp(vecE[i], tmpW, rec->pathForFlow(vecE[i]),
                    rec->getSelfSID());
            //Flow(ElephentEntry entry, MovingWindow window, SIDVector pathForEntry, SegmentId first);
            //    Flow tmp(it->first,tmpW,rec->pathForFlow(it->first),rec->getSelfSID());
            allFlowsNotRerouted.insert(tmp);
        }

        if (segal_alg){
            for (int i = 0; i < elphantWithChangedPath.size(); i++) {
                MovingWindow tmpW = tmpelephantTable.find(elphantWithChangedPath[i])->second;
                Flow tmp(elphantWithChangedPath[i], tmpW, rec->pathForFlow(elphantWithChangedPath[i]),
                        rec->getSelfSID());
                flowsWhoChangedPathAndNotFinished.insert(tmp);
            }


            for (int i = 0; i < allEdgesOfAllElephant.size(); i++) {
                MovingWindow tmpW = tmpelephantTable.find(allEdgesOfAllElephant[i])->second;
                Flow tmp(allEdgesOfAllElephant[i], tmpW, rec->pathForFlow(allEdgesOfAllElephant[i]),
                        rec->getSelfSID());
                allFlowsOfAllElephants.insert(tmp);
            }


        }

    }


    /*
     * end//get all the flows
     */

    //    /***test bed***/
    //    double arr[5];
    //    std::multiset<Flow>::iterator it;
    //    int i=0;
    //    for (it=allFlows.begin(); it!=allFlows.end(); ++it){
    //        Flow tmp2=*it;
    //        arr[i]=tmp2.getTTLDBL();
    //        i++;
    //    }

    std::vector<edge> edgesWithChangedPathNotOver = updateEdgesWithChangedPathNotOver(allFlowsOfAllElephants, flowsWhoChangedPathAndNotFinished);

    if (estEdges.size() > 0 && !pathChange) {
        CheckAndRepair(realEdges, estEdges, allFlowsNotRerouted, 2, edgesWithChangedPathNotOver);
    }
    if (realEdges.size() > 0 && !pathChange) {
        CheckAndRepair(realEdges, estEdges, allFlowsNotRerouted, 1, edgesWithChangedPathNotOver);
    }
}

vector<edge> NetworkController::updateEdgesWithChangedPathNotOver(const std::multiset<Flow> &allFlows, std::multiset<Flow> flowsWhoChangedPathAndNotFinished) {
    std::vector<edge> edgesWithChangedPathNotOver;
    if (!segal_alg) {
        return edgesWithChangedPathNotOver;
    }

    std::multiset<Flow>::iterator it;
    for (it = flowsWhoChangedPathAndNotFinished.begin(); it != flowsWhoChangedPathAndNotFinished.end(); ++it) {
        auto finder=allFlows.find(*it);
        if (finder == flowsWhoChangedPathAndNotFinished.end()) {
            flowsWhoChangedPathAndNotFinished.erase(*it);
        }
    }

    // add all edges to the vector
    for (it = flowsWhoChangedPathAndNotFinished.begin(); it != flowsWhoChangedPathAndNotFinished.end() && !pathChange; ++it) {
           Flow tmp = *it;
           SIDVector pathForEntry = tmp.pathForEntry;
           int curr = tmp.first; //gets first node on path, it is not part of the path in the SIDvector
           for (unsigned int p = 0; p < pathForEntry.size(); p++) {
               edge currE(curr, pathForEntry[p]);
               edgesWithChangedPathNotOver.push_back(currE);

               curr = pathForEntry[p];
           }
    }
    return edgesWithChangedPathNotOver;
}

void NetworkController::CheckAndRepair(vector<edge> &realEdges,
        vector<edge> &estimatedEdges, std::multiset<Flow> &allFlows, int which, vector<edge> &edgesWithChangedPathNotOver) {
    std::multiset<Flow>::iterator it;
    std::vector<Flow> delVec; //this vector will have all flows that would need to be deleted
    vector<edge> Edges = which == 1 ? realEdges : estimatedEdges;
    for (it = allFlows.begin(); it != allFlows.end() && !pathChange; ++it) {
        Flow tmp = *it;
        int EdgeOverLap = 0;
        SIDVector pathForEntry = tmp.pathForEntry;
        int curr = tmp.first; //gets first node on path, it is not part of the path in the SIDvector
        for (unsigned int p = 0; p < pathForEntry.size() && !pathChange; p++) {
            edge currE(curr, pathForEntry[p]);
            if (IstheEdgeInSet(Edges, currE)) {
                EdgeOverLap++;
            }
            curr = pathForEntry[p];
        }

        if (EdgeOverLap > 0) {
            //findReRoutePath
            SIDVector reRoutePath;
//            yodars
//            if (tmp.pathForEntry.size()>3)  {/////////path in same pod it has a very short path of 2--->dont send it;
//            if (1)  {/////////path in same pod it has a very short path of 2--->dont send it;

            string topologyName = getParentModule()->getParentModule()->par(
                    "topologyName");
            if (topologyName != "DragonflyTopologyH2"
                    || !isWithinTheSamePod(tmp)) {
                flowCandidate chosenCandidate = findReRoutePath(Edges, tmp,
                        EdgeOverLap, which == 2, edgesWithChangedPathNotOver); // which == 2 is estimated

                //              yodars:  all paths can be longer than 1
                if (isValidCandidate(chosenCandidate)) {
                    reRoutePath = chosenCandidate.path;
//                if (reRoutePath[0]!=0){
                    reRoute(tmp, reRoutePath);
                    //allFlows.erase(it);
                    delVec.push_back(*it);
                    if (!chosenCandidate.isShortPath) { //test if any flow is a long flow
                        replacePath++;
                        //pathChange=1;
//                        yodars
//                        if( reRoutePath.size()>5){ //test if any flow is a long flow
//                        if( reRoutePath.size()>2){ //test if any flow is a long flow
//
//
//                            EV << "Path Replaced From: " <<
//                                    tmp.pathForEntry[0]<<"->" <<
//                                    tmp.pathForEntry[1]<<"->" <<
//                                    tmp.pathForEntry[2]<<"->" <<
//                                    tmp.pathForEntry[3]<< "->" <<
//                                    tmp.pathForEntry[4] <<"TO:" <<  tmp.pathForEntry[0]<< "->" <<reRoutePath[0] <<endl;
//
//
//                                          }

                    }

                    updateEdges(tmp, reRoutePath, estimatedEdges, realEdges, edgesWithChangedPathNotOver);
                }
            }
        }
        if (Edges.size() == 0)
            break;

    }
    for (int j = 0; j < delVec.size(); j++) {
        allFlows.erase(delVec[j]);

    }
}
bool NetworkController::isWithinTheSamePod(Flow theFlow) {
    int k = getParentModule()->getParentModule()->par("k");
    return ((theFlow.first - 1) / k
            == ((theFlow.pathForEntry[theFlow.pathForEntry.size() - 1]) - 1) / k);
}

flowCandidate NetworkController::findReRoutePath(vector<edge> Edges,
        Flow theFlow, int EdgeOverLap, bool isEstimatedMode, vector<edge> &edgesWithChangedPathNotOver) {
    flowCandidate candidate = findReRoutePathPotential(Edges, theFlow,
            EdgeOverLap, isEstimatedMode, edgesWithChangedPathNotOver);

    if (!RUN_YODAR_NITZAN_FEATURE_FLAG) {
        return candidate;
    }
// yodars and nitzan's awesome alg
    if (isValidCandidate(candidate) && isCandidateBetterThanCurrentPath(theFlow, candidate, isEstimatedMode)) {
        return candidate;
    }
    flowCandidate returnNothing;
    SIDVector vec;
    vec.push_back(0);
    returnNothing.path = vec;
    return returnNothing;
}

bool NetworkController::isValidCandidate(const flowCandidate candidate) {
    SIDVector reRoutePath = candidate.path;
    return (reRoutePath[0] != 0 && reRoutePath.size() > 1);
}

bool NetworkController::isCandidateBetterThanCurrentPath(const Flow theFlow,
        const flowCandidate candidate, bool isEstimatedMode) {
    double maxLoadEst = 0;
    double SumLoadEst = 0;
    SIDVector tmpSID = theFlow.pathForEntry;
    SIDVector path = theFlow.pathForEntry;
    double loadInc = 0;
    path.insert(path.begin(), theFlow.first);
    for (int j = 0; j < path.size() - 1; j++) {

        edge currEdge(path[j], path[j + 1]);
        if(LOAD_INCLUDE_NEIGHBORS_LOAD){
            loadInc += calcNeighborLoad(path[j], path[j + 1]);
        }
        if (!isEstimatedMode && RUN_YODAR_NITZAN_FEATURE_FLAG) {
            SumLoadEst += (edges_set[currEdge].getLoad() + loadInc);
            if ((edges_set[currEdge].getLoad() + loadInc) > maxLoadEst) {
                maxLoadEst = edges_set[currEdge].getLoad() + loadInc;
            }
        } else {
            SumLoadEst += (edges_set_est[currEdge].getLoad() + loadInc);
            if ((edges_set_est[currEdge].getLoad() + loadInc) > maxLoadEst) {
                maxLoadEst = edges_set_est[currEdge].getLoad() + loadInc;
            }
        }
    }
    double currentLoad = alpha * maxLoadEst + (1 - alpha) * SumLoadEst;
    if (currentLoad < candidate.load) {
        return false;
    }
    return true;

}

bool NetworkController::checkIfEdgeWillBeLoaded(edge currEdge,
        Flow theFlow, bool isEstimatedMode) {
    double loadInc;
    double currentLoadOnEdge = 0;
    if (!isEdgePartOfPath(currEdge, theFlow.pathForEntry)) {
        loadInc = theFlow.window->getTotal();
    }

//    if (isEstimatedMode) {
//        auto res = edges_set_est.find(currEdge);
//        if (res != edges_set_est.end()) {
//            currentLoadOnEdge = res->second.getLoad();
//        }
//        return (currentLoadOnEdge + loadInc) > MAXLOADEST;
//    } else { // normal load
        auto res = edges_set.find(currEdge);
        if (res != edges_set.end()) {
            currentLoadOnEdge = res->second.getLoad();
        }
        return (currentLoadOnEdge + loadInc) > MAXLOAD;
//    }


}

bool NetworkController::checkIsEdgeWithMaxReroute(edge currEdge,
        vector<edge> &edgesWithChangedPathNotOver) {
    if (!segal_alg) {
        return false;
    }
    int numberOfTimesInSet = 0;
    for (uint32 i = 0; i < edgesWithChangedPathNotOver.size(); i++) {
        if (edgesWithChangedPathNotOver[i] == currEdge) {
            numberOfTimesInSet ++;
        }
    }
    return numberOfTimesInSet >= MAX_NUMBER_REROUTE_PER_EDGE;

}



flowCandidate NetworkController::findReRoutePathPotential(vector<edge> Edges,
        Flow theFlow, int EdgeOverLap, bool isEstimatedMode, vector<edge> &edgesWithChangedPathNotOver) {

    pathTable Pathentry;
    double delayDiff = (simTime() - theFlow.getTTL()).dbl(); //might be not so acuurate
    Pathentry = FindAllPathsS2T(theFlow.first,
            theFlow.pathForEntry[theFlow.pathForEntry.size() - 1]);
    cout << "src:" << Pathentry.src << "dst:" << Pathentry.dst << "path entry"
            << endl;
    cout << "src:" << theFlow.first << "dst:"
            << theFlow.pathForEntry[theFlow.pathForEntry.size() - 1]
            << "the flow" << endl;
    cout << endl;

    std::vector<flowCandidate> CandidateVec;
    int i, j, eOverlapCurr = 0;
    for (i = 0; i < Pathentry.shortPaths.size(); i++) {
        bool isThereAnEdgeWithMaxRerouteInPath = false;
        eOverlapCurr = 0;
        SIDVector pathForEntry = Pathentry.shortPaths[i].first;
        int curr = pathForEntry.front(); //gets first node on path, it is not part of the path in the SIDvector
        for (unsigned int p = 1; p < pathForEntry.size(); p++) {
            edge currE(curr, pathForEntry[p]);
            bool willEdgeBeLoaded = checkIfEdgeWillBeLoaded(currE, theFlow, isEstimatedMode);
            bool isEdgeWithMaxReroute = checkIsEdgeWithMaxReroute(currE, edgesWithChangedPathNotOver);
            if (isEdgeWithMaxReroute) {
                isThereAnEdgeWithMaxRerouteInPath = true;
                break;
            }
            if (IstheEdgeInSet(Edges, currE) || (willEdgeBeLoaded && RUN_YODAR_NITZAN_FEATURE_FLAG)) {
                eOverlapCurr++;
            }
            curr = pathForEntry[p];

        }
        SIDVector tmpSID = theFlow.pathForEntry;
        tmpSID.insert(tmpSID.begin(), theFlow.first);
        double flowDel = findDelayOnPath(tmpSID);
        double pathDel = findDelayOnPath(pathForEntry);
        if (isThereAnEdgeWithMaxRerouteInPath || (EdgeOverLap > eOverlapCurr && flowDel - pathDel > delayDiff)) {
            flowCandidate tmpC;
            tmpC.delayDiff = flowDel - pathDel;
            tmpC.load = -1;
            tmpC.path = pathForEntry;
            tmpC.eOverLap = eOverlapCurr;
            tmpC.isShortPath = true;
            CandidateVec.push_back(tmpC);
        }
    } //short paths
    for (i = 0; i < Pathentry.longpaths.size(); i++) {
        bool isThereAnEdgeWithMaxRerouteInPath = false;
        eOverlapCurr = 0;
        SIDVector pathForEntry = Pathentry.longpaths[i].first;
        int curr = pathForEntry.front(); //gets first node on path, it is not part of the path in the SIDvector
        for (unsigned int p = 1; p < pathForEntry.size(); p++) {
            edge currE(curr, pathForEntry[p]);
            bool willEdgeBeLoaded = checkIfEdgeWillBeLoaded(currE, theFlow, isEstimatedMode);
            bool isEdgeWithMaxReroute = checkIsEdgeWithMaxReroute(currE, edgesWithChangedPathNotOver);
            if (isEdgeWithMaxReroute) {
                isThereAnEdgeWithMaxRerouteInPath = true;
                break;
            }
            if (IstheEdgeInSet(Edges, currE) || (willEdgeBeLoaded && RUN_YODAR_NITZAN_FEATURE_FLAG) || (isEdgeWithMaxReroute && segal_alg)) {
                eOverlapCurr++;
            }
            curr = pathForEntry[p];
        }
        SIDVector tmpSID = theFlow.pathForEntry;
        tmpSID.insert(tmpSID.begin(), theFlow.first);
        double flowDel = findDelayOnPath(tmpSID);
        double pathDel = findDelayOnPath(pathForEntry);

        // Check if the number of loaded edges is smaller than the current path, and the delay is smaller on the new path.
//        if (EdgeOverLap > eOverlapCurr && flowDel - pathDel > delayDiff) // yodars: was
        if (isThereAnEdgeWithMaxRerouteInPath||(EdgeOverLap > eOverlapCurr && flowDel - pathDel > delayDiff)) // yodars: was
        {
            flowCandidate tmpC;
            tmpC.delayDiff = flowDel - pathDel;
            tmpC.load = -1;
            tmpC.path = pathForEntry;
            tmpC.eOverLap = eOverlapCurr;
            tmpC.isShortPath = false;
            CandidateVec.push_back(tmpC);
        }
    } //long paths

    for (auto it = CandidateVec.begin(); it != CandidateVec.end(); ++it) {
        double maxLoadEst = 0;
        double SumLoadEst = 0;
        SIDVector tmpSID = theFlow.pathForEntry;
        double loadInc = 0;
        tmpSID.insert(tmpSID.begin(), theFlow.first);
        for (j = 0; j < it->path.size() - 1; j++) {

            edge currEdge(it->path[j], it->path[j + 1]);
            if (!isEdgePartOfPath(currEdge, tmpSID)) {
                loadInc = theFlow.window->getTotal();
            }

//            if (!isEstimatedMode && RUN_YODAR_NITZAN_FEATURE_FLAG) {
//                SumLoadEst += (edges_set[currEdge].getLoad() + loadInc);
//                if ((edges_set[currEdge].getLoad() + loadInc) > maxLoadEst) {
//                    maxLoadEst = edges_set[currEdge].getLoad() + loadInc;
//                }
//            } else {
            if(LOAD_INCLUDE_NEIGHBORS_LOAD){
                loadInc += calcNeighborLoad(it->path[j], it->path[j + 1]);
            }

            SumLoadEst += (edges_set_est[currEdge].getLoad() + loadInc);
            if ((edges_set_est[currEdge].getLoad() + loadInc) > maxLoadEst) {
                maxLoadEst = edges_set_est[currEdge].getLoad() + loadInc;
            }
//            }

        }
        it->load = alpha * maxLoadEst + (1 - alpha) * SumLoadEst;

    }
    vector<flowCandidate> tempVec;
    double minload = 9999999999999999;
    double mindellDiff = 9999999999999999;
    double minOverLap = 9999999999999999;

    vector<flowCandidate> tempVecdell;
    vector<flowCandidate> tempVecoveLap;

    if (CandidateVec.size() > 1) {
        int index_load_est = 0;
        for (j = 0; j < CandidateVec.size(); j++) {
            if (CandidateVec[j].load < minload) {
                index_load_est = j;
                minload = CandidateVec[j].load;
            }
        }
        for (j = 0; j < CandidateVec.size(); j++) {
            if (CandidateVec[j].load == minload)
                tempVec.push_back(CandidateVec[j]);
        }
        if (tempVec.size() <= 1)
            return CandidateVec[index_load_est];
        else { //if more than one cadidate with minlaod

            int index_delay = 0;
            for (j = 0; j < tempVec.size(); j++) {
                if (tempVec[j].delayDiff < mindellDiff) {
                    index_delay = j;
                    mindellDiff = tempVec[j].delayDiff;
                }
            }
            for (j = 0; j < tempVec.size(); j++) {
                if (tempVec[j].delayDiff == mindellDiff)
                    tempVecdell.push_back(tempVec[j]);
            }
            if (tempVecdell.size() <= 1)
                return tempVecdell[index_delay];
            else {

                int index_edges_overlap = 0;
                for (j = 0; j < tempVecdell.size(); j++) {
                    if (tempVecdell[j].eOverLap < minOverLap) {
                        index_edges_overlap = j;
                        minOverLap = tempVecdell[j].eOverLap;
                    }
                }
                for (j = 0; j < tempVecdell.size(); j++) {
                    if (tempVecdell[j].delayDiff == mindellDiff)
                        tempVecoveLap.push_back(tempVec[j]);
                }
                return tempVecoveLap[index_edges_overlap];

            }

        }
    } else if (CandidateVec.size() == 1) {
        return CandidateVec[0];
    } else {
        flowCandidate returnNothing;
        SIDVector vec;
        vec.push_back(0);
        returnNothing.path = vec;
        return returnNothing;
    }
}

double NetworkController::calcNeighborLoad(SegmentId firstNode, SegmentId secondNode){
    
    //for all edges that related to first node - take their load
    double sumLoad  = 0;
    std::vector<edge> edgeOfNode = edgeEdg_Location[firstNode];
    for (int i = 0; i < edgeOfNode.size(); ++i) {
        sumLoad += edges_set_est[edgeOfNode[i]].getLoad();
    }
    edge currEdge(firstNode, secondNode);
    sumLoad -= edges_set_est[currEdge].getLoad();
    double avgLoad = sumLoad / (edgeOfNode.size()-1);
    return avgLoad;
}

pathTable NetworkController::FindAllPathsS2T(SegmentId src, SegmentId dst) {
    int i = 0;
    int numberOfPathsFromEdgesToEdges = getParentModule()->getParentModule()->par(
            "numberOfPathsFromEdgesToEdges");
    for (; i < numberOfPathsFromEdgesToEdges; i++) {
        if (fullbPathTable[i].src == src && fullbPathTable[i].dst == dst) {
            if (simTime() > 87.2) {
                int l = 0;
                l++;
            }
//            cout << "Im not your toy: src:" << src << "dest:" << dst <<endl;

            break;
        }
    }
    pathTable entry; //false entry
    entry.dst = 0;
    entry.src = 0;
    return numberOfPathsFromEdgesToEdges == i ? entry : fullbPathTable[i];
}

bool NetworkController::isEdgePartOfPath(edge e, SIDVector p) {
    for (auto i = 0; i < p.size() - 1; i++) {
        edge currEdge(p[i], p[i + 1]);
        if (currEdge == e) {
            return true;
        }
    }
    return false;

}

void NetworkController::loadFile() {
    double time = simTime().dbl();
    long unsigned int load, estLoad;
    int left, right;

    for (std::map<edge, edge>::iterator it = edges_set_est.begin();
            it != edges_set_est.end(); ++it) {
        left = it->second.getLeft();
        right = it->second.getRight();
        load = edges_set[it->first].getLoad();
        estLoad = it->second.getLoad();

        //       if (load<0 || estLoad<0){
        //           int fuck='t';
        //       }
        //       if(left==21 && right==13) {
        //           int tupu=98;
        //       }

        fprintf(pFile, "%lf,%d,%d,%lu,%lu,", time, left, right, load, estLoad);
        fflush(pFile);
    }
    fprintf(pFile, "\n");
}
void NetworkController::reRoute(Flow f, SIDVector path) {
    //get a call the the reclassiffier and change the path to this;
    //TODO test what we need to send? the vector with out the head;
    char edgesStr[30];
//    int k=getParentModule()->getParentModule()->par("k");
//    int dst=(k*k / 4) + (k*k / 2) + 2;
    int dstOffset = 1;

    if (simTime() > 87.2) {
        int l = 0;
        l++;
    }

    sprintf(edgesStr, "^.^.Edges[%d].spring", f.first - dstOffset);
    SPRING * edgeNode = check_and_cast<SPRING *>(getModuleByPath(edgesStr));
    Reclassifier *rec = edgeNode->getReclassifier();
    SIDVector withoutFirst;
    for (int k = 1; k < path.size(); k++) {
        withoutFirst.push_back(path[k]);
    }
    rec->ChangePathForFLow(f.entry, withoutFirst);

}

/*
 * this function updates all the edges in our sim with the new load, we assume that we the flow's load fprm one path to the other
 *  just update all of edges that are part of both paths
 */
void NetworkController::updateEdges(Flow f, SIDVector path,
        vector<edge> &estimatedEdges, vector<edge> &realEdges, vector<edge> &edgesWithChangedPathNotOver) {
    if (1 == 1) {    //ALGFLAG
        SegmentId v1 = f.first;
        /*
         * first we remove all the load from the current path of the flow
         */
        for (unsigned int n = 0; n < f.pathForEntry.size(); n++) { //for each edge in a short path //starts with n=1 since the first SID in path is it self
            SegmentId v2 = f.pathForEntry[n];
            edge curEdge(v1, v2);
            if (simTime() > 86.2) {
                int tp = 9;
            }
            auto edgeEst = edges_set_est.find(curEdge);
            auto edgeReal = edges_set.find(curEdge);
            if (edgeEst != edges_set_est.end()) {
                double curLoad = floor(
                        edgeEst->second.getLoad() - f.window->getTotal());
                if (curLoad < 0 || curLoad > 100000000)    // Sanity check
                        {
                    curLoad = edgeReal->second.getLoad();
                }
                curEdge.setLoad(curLoad);
                edges_set_est.erase(edgeEst);
                edges_set_est.insert(std::pair<edge, edge>(curEdge, curEdge));
            }
            //        if(res2!=edges_set.end()){
            //            double curLoad=floor(res2->second.getLoad()-f.window->getTotal());
            //            curEdge.setLoad(curLoad);
            //            edges_set.erase(res2);
            //            edges_set.insert(std::pair<edge,edge>(curEdge,curEdge));
            //        }
            v1 = v2;
        }

        /*
         * and now to re add the load
         */
        v1 = path[0];
        for (unsigned int n = 1; n < path.size(); n++) { //for each edge in a short path //starts with n=1 since the first SID in path is it self
            SegmentId v2 = path[n];
            edge curEdge(v1, v2);
            auto edgeEst = edges_set_est.find(curEdge);
            auto edgeReal = edges_set.find(curEdge);
            if (edgeEst != edges_set_est.end()) {
                double curLoad = floor(
                        edgeEst->second.getLoad() + f.window->getTotal()); //TODO FIX THIS PART!!
                curEdge.setLoad(curLoad);
                if (curLoad < 0 || curLoad > 100000000) // Sanity check
                        {
                    curLoad = edgeReal->second.getLoad();
                }
                edges_set_est.erase(edgeEst);
                edges_set_est.insert(std::pair<edge, edge>(curEdge, curEdge));
                edgesWithChangedPathNotOver.push_back(curEdge);
            }
            //           if(res2!=edges_set.end()){
            //               double curLoad=floor(res2->second.getLoad()+f.window->getTotal());//TODO FIX THIS PART!!
            //               curEdge.setLoad(curLoad);
            //               edges_set.erase(edgeReal);
            //               edges_set.insert(std::pair<edge,edge>(curEdge,curEdge));
            //           }
            v1 = v2;
        }
        std::vector<edge> newEdges;
        double MaxLoad = MAXLOAD * WINDOWTIME, MaxLoadEst = MAXLOADEST
                * WINDOWTIME;
        for (std::map<edge, edge>::iterator it = edges_set_est.begin();
                it != edges_set_est.end(); ++it) {
            if (it->second.getLoad() > MaxLoadEst) {
                newEdges.push_back(it->second);
            }

        }

        estimatedEdges = newEdges;
        newEdges.clear();

        for (std::map<edge, edge>::iterator it = edges_set.begin();
                it != edges_set.end(); ++it) {
            if (it->second.getLoad() > MaxLoad) {
                newEdges.push_back(it->second);
            }

        }
        realEdges = newEdges;

    }

}

} //namespace inet
}
