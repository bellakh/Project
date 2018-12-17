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

#ifndef SPRINGNETWORKCONTROLLER_H_
#define SPRINGNETWORKCONTROLLER_H_
#define SIMTIMESTART 80
//#define ALGFLAG 1//run updated edges 1 means active
#define PATHTABLESIZE 1722

//#define MAXLOAD 625000
//#define MAXLOADEST 625000

//#define MAXLOAD 680000
//#define MAXLOADEST 680000

//movie values
//#define MAXLOAD 680000
//#define MAXLOADEST 680000

//good values
//#define MAXLOAD 87500
//#define MAXLOADEST 87500
#define MAX_NUMBER_REROUTE_PER_EDGE 10
#define MAXLOAD 150000
#define MAXLOADEST 300000
#define SAMPTIME 0.1
#define RUNALGFLAG 1  /// run alg flag 1 means active -- yodars : this var is to show if the alg of hen is On, 1 is On
#define RUN_YODAR_NITZAN_FEATURE_FLAG 1 // has not affect when RUNALGFLAG is 0
#define LOAD_INCLUDE_NEIGHBORS_LOAD 1 // indicate if edge load depends on neighbors edges load
#define segal_alg 1
#define ALPHA 0.4
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "inet/transportlayer/udp/UDP.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"

#include "inet/routing/ospfv2/OSPFRouting.h"
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"
#include "../forwarding/RoutingAlg.h"
#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/base/FlowGraph.h"
#include "inet/networklayer/spring/control/SpringControlPackets_m.h"
#include "inet/networklayer/spring/control/ControllerNetGraph.h"
#include "inet/networklayer/spring/forwarding/SPRING.h"
#include "inet/networklayer/spring/forwarding/SPRINGPacket.h"

namespace inet {
using namespace std;
using namespace ospf;

namespace spring {
//TODO:  use short node indexes to save memory.
//the order of nodes in the initial Graph must not change;
typedef std::pair<vector<SegmentId>, double> pathAnddelay; //Matching between route and delay.
typedef struct pathTable {
    SegmentId src;
    SegmentId dst;
    std::vector<pathAnddelay> shortPaths;
    std::vector<pathAnddelay> longpaths;

} pathTable;

typedef struct flowCandidate {
    SIDVector path;
    double load;
    int eOverLap;
    double delayDiff;
    bool isShortPath;

} flowCandidate;


class edge {
protected:
    SegmentId leftEdge;
    SegmentId rightEdge;
    long unsigned int load;
    double del;

public:
    edge();
    edge(SegmentId leftEdge, SegmentId rightEdge, double load = 0, double del =
            0);

    bool operator<(const edge &right) const;
    bool operator==(const edge &right) const;

    void setLoad(double load) {
        this->load = load;
    }
    long unsigned int getLoad() {
        return load;
    }
    void setDelay(double del) {
        this->del = del;
    }
    double getDelay() {
        return del;
    }
    SegmentId getLeft() {
        return leftEdge;
    }
    SegmentId getRight() {
        return rightEdge;
    }

};
class Flow {
public:
    ElephentEntry entry;
    MovingWindow *window;
    SIDVector pathForEntry;
    SegmentId first;
    simtime_t TTL;

    Flow();
    Flow(ElephentEntry entry, MovingWindow window, SIDVector pathForEntry,
            SegmentId first);
    Flow(const Flow &obj);
    ~ Flow() {
        delete window;
    }

    bool operator<(const Flow &right) const;
    bool operator==(const Flow &right) const;
public:

    double getTTLDBL() {
        return this->TTL.dbl();
    }
    simtime_t getTTL() {
        return this->TTL;
    } //this is more acutrate

};

typedef std::map<edge, double> edgeTable;
typedef std::vector<SegmentId> SIDVector;
typedef std::map<edge, edge> allEdges;

class NetworkController: public cSimpleModule, public IOpaqueListener {
    typedef std::map<SegmentId, std::vector<edge>> edgeLocation;
private:
    std::unique_ptr<ControllerNetGraph> network;
    edgeLocation edgeAgg_Location;
    edgeLocation edgeEdg_Location;
    edgeLocation edgeCores_Location;
    edgeTable edge_Table;
    edgeTable loadEstTable;
    allEdges edges_set;
    allEdges edges_set_est;
    double alpha = ALPHA;
    FILE * pFile;
    int replacePath = 0;
    int pathChange = 0;
    void readCSV();
    void readVariousCSV();
    void setDelay2();
    void readEdgesAggCSV();
    void readEdgesCoresCSV();
    void readEdgesEdgCSV();
    void init_edges_set();
    void sendEROS(SegmentId dstSID, L3Address dstAddr, int dstPort);
    void updateStats(SegmentId origSid, StatsVector& stats);
    edge findEdge(edge e);
    double findDelayOnPath(vector<SegmentId> path);
    void clacDelaysOnAllPaths();
    int howToDivaide(int i);
    double findLoadOnPath(vector<SegmentId> path);
    void Congestion_Detection();
    void CheckAndRepair(vector<edge> &Edges1, vector<edge> &Edges2,
            std::multiset<Flow> &allFlows, int which, vector<edge> &edgesWithChangedPathNotOver);
    bool IstheEdgeInSet(vector<edge> Edges, edge theEdge);
    flowCandidate findReRoutePath(vector<edge> Edges, Flow theFlow,
            int EdgeOverLap, bool isEstimatedMode, vector<edge> &edgesWithChangedPathNotOver);
    pathTable FindAllPathsS2T(SegmentId src, SegmentId dst);
    bool isEdgePartOfPath(edge e, SIDVector p);
    void reRoute(Flow f, SIDVector path);
    void updateEdges(Flow f, SIDVector path, vector<edge> &Edges1,
            vector<edge> &Edges2, vector<edge> &edgesWithChangedPathNotOver);
    void setDelay3();
    void loadFile();
    bool isWithinTheSamePod(Flow theFlow);
    flowCandidate findReRoutePathPotential(vector<edge> Edges, Flow theFlow,
            int EdgeOverLap, bool isEstimatedMode, vector<edge> &edgesWithChangedPathNotOver);
    bool isValidCandidate(const flowCandidate candidate);
    bool isCandidateBetterThanCurrentPath(const Flow theFlow,
            const flowCandidate candidate, bool isEstimatedMode);
    bool checkIfEdgeWillBeLoaded(const edge currEdge, const Flow theFlow, bool isEstimatedMode);
    bool checkIsEdgeWithMaxReroute(edge currEdge,
            vector<edge> &edgesWithChangedPathNotOver);
    vector<edge> updateEdgesWithChangedPathNotOver(const std::multiset<Flow> &allFlows,  std::multiset<Flow> flowsWhoChangedPathAndNotFinished);
    double calcNeighborLoad(SegmentId firstNode, SegmentId secondNode);


    cVarHistogram lab3messagestime;
    cOutVector msgTimeInsysStats;
protected:
    SegmentId minNodalSID;
    SegmentId maxNodalSID;
    SegmentId minAdjacencySID;
    SegmentId maxAdjacencySID;
    OSPFRouting *ospf;
    bool firstSelfmsg = false;
    int localPort;
    UDPSocket udpSocket;
    int sigmaStoT, sigmaEdgetoAgg, sigmaAggtoCore;
    /*
     * A structure that holds all short and long paths between each pair of nodes.
     * TODO make dynamic
     */
    pathTable fullbPathTable[PATHTABLESIZE];
    std::vector<pathTable> fullbPathTable2;
    std::vector<pathTable> fullbPathTableTest;
    std::unique_ptr<SRGBDatabase> srgb;
    std::unique_ptr<OpaqueLsaDatabase> lsaDatabase;
    std::vector<std::vector<SegmentId>> data;
    std::vector<SIDVector> pathsInPods;
    std::set<ElephentEntry> rerouteFlow;
    //  void insertTotable(SegmentId dst,SegmentId src, std::vector<std::vector<SegmentId>> vec);
    void buildPathTable();
public:
    bool getBestPath(SegmentId src, SegmentId dst, SIDVector & route);
    SIDVector getFirstPath(cGate* gate, SPRINGPacket* packet, SegmentId dest,
            SegmentId src);
    //SIDVector getBestPath( SegmentId src, SegmentId dst); //TODO public test function... delete it
    void loadEstEdge();
    NetworkController() {
    }
    virtual ~NetworkController() {
    }

    virtual int numInitStages() const {
        return NUM_INIT_STAGES;
    }
    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual void handleTimer(cMessage *msg);
    //IOpaqueListener
    virtual void notify(OSPFOpaqueLSA *lsa);

};

} //namespace spring

} //namespace inet

#endif /* SPRINGNETWORKCONTROLLER_H_ */
