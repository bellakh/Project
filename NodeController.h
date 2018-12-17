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

#ifndef SPRINGNODECONTROLLER_H_
#define SPRINGNODECONTROLLER_H_
//yodars num_of_connections_without_clients
#define DELAYARR 7
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <memory>

#include "inet/common/ModuleAccess.h"
#include "inet/networklayer/contract/IRoutingTable.h"
#include "inet/networklayer/ipv4/IPv4InterfaceData.h"
#include "inet/networklayer/contract/IInterfaceTable.h"
#include "inet/transportlayer/udp/UDP.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/contract/ipv4/IPv4Address.h"
//#include "inet/networklayer/spring/forwarding/Classifier.h"
//#include "inet/networklayer/spring/forwarding/Reclassifier.h"

#include "inet/routing/ospfv2/OSPFRouting.h"
#include "inet/routing/ospfv2/router/OSPFcommon.h"
#include "inet/routing/ospfv2/OSPFPacket_m.h"
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"
#include "inet/routing/ospfv2/router/LSA.h"
#include "inet/routing/ospfv2/opaque/ExtendedPrefixLinkAttr_m.h"
#include "inet/networklayer/spring/control/SpringOspf_m.h"

//#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/base/ConfigurableThruputMeteringChannel.h"
#include "inet/networklayer/spring/forwarding/SPRING.h"
//#include "inet/networklayer/spring/forwarding/FibTable.h"
//#include "inet/networklayer/spring/forwarding/RoutingAlg.h"
//#include "inet/networklayer/spring/control/NetGraph.h"
//#include "inet/networklayer/spring/control/SpringControlPackets_m.h"




namespace inet {

using namespace ospf;

namespace spring {

class NodeController: public cSimpleModule, public IOpaqueListener {
protected:
    SegmentId nodeSIDidx;
    SegmentId minNodalSID;
    SegmentId maxNodalSID;
    SegmentId minAdjacencySID;
    SegmentId maxAdjacencySID;
    SPRING * spring;
    IInterfaceTable *ift;           // interface table of the host
    IIPv4RoutingTable *rt;
    FibTable *fib;
    FecTable *fec;
    Classifier *classif;
    Reclassifier *reclassif;
    L3Address controllerIP;
    int controllerPort;
    UDPSocket controllerSocket;

    std::map<edge,edge> allLoads;

    int localPort;

    OSPFRouting *ospf;

    std::unique_ptr<NetGraph> netGraph;
    std::unique_ptr<OpaqueLsaDatabase> lsaDatabase;
    std::unique_ptr<SRGBDatabase> srgb;

    std::vector<int> linksOff;

    bool firstSelfMsg ;
public:
    NodeController() {}
    virtual ~NodeController() {}

    SegmentId getNodalSID();
    void rebuildFibTable() ;
    void rebuildFibTableECMP();
   bool getFirst(){return firstSelfMsg;}
   double *getChanels(){return channelUtilizationArr;}
   double *getDelays(){return delayPerLink;}
   std::map<edge,edge> getAllLoadsAndDelays(){ return allLoads;}
protected:
    double channelUtilizationArr[10];
    double delayPerLink[DELAYARR]; //TODO make dynamic
    double timerFlag=0;
    virtual int numInitStages() const { return NUM_INIT_STAGES; }
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    void updateChannels();
    void updateChannels2();
    void handleTimer(cMessage *msg);
    void LoadAndDelay();
    //IOpaqueListener (receiving the ospf opaque lsa)
    virtual void notify(OSPFOpaqueLSA *lsa);

    //flood the node description via ospf
    void updateExplicitRoutes(EROs &eros);
    void floodLocalLinksDescription();
};

} //namespace spring

} //namespace inet


#endif /* SPRINGNODECONTROLLER_H_ */
