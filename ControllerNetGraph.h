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

#ifndef SPRING_CONTROLLER_NETRORK_H
#define SPRING_CONTROLLER_NETRORK_H

#include <unordered_map>

#if !STANDALONE_STREETE
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"
#include "inet/routing/ospfv2/router/LSA.h"
#include "inet/routing/ospfv2/opaque/OSPFTE.h"
#include "inet/networklayer/spring/control/SpringOspf_m.h"
#include "inet/networklayer/spring/control/SpringControlPackets_m.h"
#include "inet/networklayer/spring/base/ConfigurableThruputMeteringChannel.h"
#endif


#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/base/GraphDAPSP.h"
#include "inet/networklayer/spring/control/SRGBDatabase.h"
#include "inet/networklayer/spring/control/NetGraph.h"
#include "inet/networklayer/spring/control/ERO.h"

namespace inet {

#if !STANDALONE_STREETE
using namespace ospf;
#endif

namespace spring {

class ControllerNetGraph: public NetGraph
{
protected:
    DAPSPGraph dapsp;
    std::vector<std::vector<double> > trafficMatrices; //Traffic matrices

public:
#if STANDALONE_STREETE
#else
    ControllerNetGraph(SRGBDatabase *_srgb):NetGraph(_srgb), dapsp(this){}
    void construct(OpaqueLsaDatabase &lsaDatabase){ NetGraph::constructGraph(lsaDatabase); resetTMs(); }
    void constructFromOmnet();
    void updateDisplay();
#endif
    ControllerNetGraph():NetGraph(), dapsp(this){}
    virtual ~ControllerNetGraph(){}

    const std::vector<std::vector<double>>& getTMs(){return trafficMatrices;}
    virtual void initializeShortestPaths() {dapsp.initShortestPaths(false);}


    void updateLinkUtilization() { resetLinkUtilization(); dapsp.routeStaticSPFinParent(trafficMatrices);}
    void updateLinkUtilization(std::vector<std::vector<double>>& tms) {resetLinkUtilization(); dapsp.routeStaticSPFinParent(tms);}
    void routeTrafficStaticSPF(const std::vector<std::vector<double>>& tms) { dapsp.routeStaticSPFinParent(tms); }

    void resetTMs();
    bool updateTM(const std::string &srcName, const std::string & dstName, double newTraffic);
    void updateTM(SegmentId src, SegmentId dst, double newTraffic);

    virtual void getAllEROs(std::vector<std::vector<ERO>> &eros);
    virtual void getAllEROsFrom(SegmentId srcSid, std::vector<ERO> &eros);
    virtual void getAllEROsFrom(Node *src, std::vector<ERO> &eros);
    virtual void getEROBetween(Node *src, Node * dst, ERO &ero);

    friend std::ostream& operator<<(std::ostream& ostr,
                                    const ControllerNetGraph& e);
protected:

#if !STANDALONE_STREETE
    virtual Topology::Node *createNode(cModule *module) {return new Node(module->getId());}
#endif
    virtual Topology::Node *createNode() {return new Node();}
    virtual Topology::Link *createLink() {return new Link();}
};

} //namespace spring

} //namespace inet



#endif /* SPRING_CONTROLLER_NETRORK_H */
