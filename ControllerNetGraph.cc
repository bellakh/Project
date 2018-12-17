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

#include <algorithm>


#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/control/ControllerNetGraph.h"


namespace inet {

namespace spring {

#if STANDALONE_STREETE
#else
void ControllerNetGraph::constructFromOmnet(){
         links.clear();
         allocedLinks.clear();
         clear();
         allocedNodes.clear();
         nodesBySid.clear();
         nodesByName.clear();
         extractByProperty("springNode");

         for(unsigned int i = 0; i<nodes.size(); i++){
             Node * node = static_cast<Node*>(nodes[i]);

             node->setName(node->getModule()->getName());
             nodesByName[node->getModule()->getName()] = node;

             SegmentId sid = node->getModule()->getSubmodule("springNodeController")->par("nodeId").longValue();
             node->setSid(sid);
             nodesBySid[sid] = node;

             node->setIndex(i);
             L3AddressResolver resolver;
             node->setRouterID(resolver.routerIdOf(node->getModule()).toIPv4());

             for(int j=0; j<node->getNumOutLinks();j++){
                 LinkOut *link = node->getLinkOut(j);


                 ConfigurableThruputMeteringChannel* channel =
                         dynamic_cast<ConfigurableThruputMeteringChannel*>(link->getLocalGate()->getChannel());
                 ASSERT(channel);
                 ((Link*) link)->setDatarate(channel->getDatarate());
                 links.push_back(link);
             }

             //std::cout <<*node<< std::endl;

         }

         resetTMs();
         initializeShortestPaths();
     }

void ControllerNetGraph::updateDisplay(){
    for (unsigned int i=0; i<links.size(); i++){
        Link * link = (Link*)(links[i]);
        //std::cout << *link << std::endl;
        ConfigurableThruputMeteringChannel* channel=
                dynamic_cast<ConfigurableThruputMeteringChannel*>(links[i]->getLocalGate()->getChannel());
        ASSERT(channel);

        channel->setColor(
                std::min(1.0, link->getUtilization()),
                std::min(1.0, findLinkBack(link)->getUtilization()),
                true
                );
    }
}
#endif


void ControllerNetGraph::resetTMs(){
    trafficMatrices.resize(nodes.size());
    for (auto &tm : trafficMatrices) {
        tm.assign(nodes.size(), 1);
    }

}
bool ControllerNetGraph::updateTM(const std::string &srcName, const std::string & dstName,
        double newTraffic) {
    Node * srcN = getNodeByName(srcName);
    if (!srcN) {
        throw cRuntimeError("no node with such name");
    }
    int srcIdx = srcN->getIndex();

    Node * dstN = getNodeByName(dstName);
    if (!dstN) {
        throw cRuntimeError("no node with such name");
    }
    int dstIdx = dstN->getIndex();

    trafficMatrices[srcIdx][dstIdx] = newTraffic;
    return true;
}

void ControllerNetGraph::updateTM(SegmentId src, SegmentId dst,
        double newTraffic) {
    Node * srcN = getNodeBySid(src);
    if (!srcN) {
        throw cRuntimeError("no node with such sid");
    }
    int srcIdx = srcN->getIndex();

    Node * dstN = getNodeBySid(dst);
    if (!dstN) {
        throw cRuntimeError("no node with such sid");
    }
    int dstIdx = dstN->getIndex();

    trafficMatrices[srcIdx][dstIdx] = newTraffic;
    EV << "TM updated from " << srcIdx << " to " << dstIdx << " " << newTraffic
              << std::endl;
}
void ControllerNetGraph::getAllEROs(std::vector<std::vector<ERO>> &eros){
    std::vector<Node *> &nodesCasted =
            (std::vector<Node *> &) nodes;

    eros.resize(nodesCasted.size());
    for(Node* src: nodesCasted){
        getAllEROsFrom(src,eros[src->getIndex()]);
    }
}

void ControllerNetGraph::getAllEROsFrom(SegmentId srcSid, std::vector<ERO> &eros){
    Node * srcNode = getNodeBySid(srcSid);
    if(srcNode){
        getAllEROsFrom(srcNode, eros );
    }
}

void ControllerNetGraph::getAllEROsFrom(Node *src, std::vector<ERO> &eros){
    std::vector<Node *> &nodesCasted =
            (std::vector<Node *> &) nodes;

    eros.resize(nodesCasted.size());
    for (Node *dst : nodesCasted) {
        getEROBetween(src, dst, eros[dst->getIndex()]);
    }
}

void ControllerNetGraph::getEROBetween(Node *src, Node * dst, ERO &ero){

    if(src==dst){
        ero.srcIp=IPv4Address((uint32) 0);
        ero.dstIp=IPv4Address((uint32) 0);

        return;
    }

    ero.srcIp = src->getRouterID();
    ero.dstIp = dst->getRouterID();
    auto& spfTree = dapsp.getShortestPathTree(src);

    ero.explicitRoute.clear();
    Node *curNode = dst;

    LinkIn *curLink = spfTree.linkFromParent[curNode->getIndex()];
    while(curLink != nullptr){
        ero.explicitRoute.push_back(curNode->getSid());

        curNode = (Node *) curLink->getRemoteNode();
        curLink = spfTree.linkFromParent[curNode->getIndex()];
    }

    ero.explicitRoute.push_back(src->getSid());

    std::reverse(ero.explicitRoute.begin(), ero.explicitRoute.end());
    return;
}


std::ostream& operator<<(std::ostream& ostr,
                                const ControllerNetGraph& e){
    ostr << "Graph nodes: [";
    for (unsigned int i = 0; i < e.nodes.size(); i++) {
        ostr << i << ":" << ((NetGraph::Node *) e.nodes[i])->getRouterID() << ", ";
    }
    ostr << "]" << std::endl;
    for (unsigned int i = 0; i < e.nodes.size(); i++) {
        ostr << "   " << i << ":" << e.dapsp << std::endl;
    }
    return ostr;
}


} //namespace spring

} //namespace inet

