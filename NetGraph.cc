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
#include "inet/networklayer/spring/control/NetGraph.h"
#if !STANDALONE_STREETE
#include "inet/routing/ospfv2/opaque/ExtendedPrefixLinkAttr_m.h"
#endif


namespace inet {

#if !STANDALONE_STREETE
using namespace ospf;
#endif

namespace spring {
NetGraph::NetGraph() : FlowGraph(){
    rootNode = nullptr;
}
#if STANDALONE_STREETE
#else //STANDALONE_STREETE
NetGraph::NetGraph(const IPv4Address &rootNodeID, SRGBDatabase * _srgb, bool ecmpAware /*=false*/):
        FlowGraph(false, ecmpAware){//, unsigned int nodeIdx){
    srgb = _srgb;
    rootNode = findOrCreateNode(rootNodeID);
}

NetGraph::NetGraph( SRGBDatabase * _srgb, bool ecmpAware /*=false*/) :
        FlowGraph(false, ecmpAware){//, unsigned int nodeIdx){
    srgb = _srgb;
    rootNode = nullptr;
}
bool NetGraph::parseTELinkTLV(const OSPFOpaqueTLV &tlv) {
    bool hasDuplicate;
    bool subLsaFound[ADMINISTRATIVE_GROUP + 1] = { false };

    const OSPFOpaqueTLVs *subTlvsArray = check_and_cast<const OSPFOpaqueTLVs*>(
            tlv.getValue());
    for (int i = 0; i < (int) subTlvsArray->getTlvsArraySize(); i++) {
        const OSPFOpaqueTLV &subTlv = subTlvsArray->getTlvs(i);
        int subTlvType = subTlv.getType();
        if (subTlvType < OSPFOpaqueTESubTLVType::LINK_TYPE
                || subTlvType > OSPFOpaqueTESubTLVType::ADMINISTRATIVE_GROUP) {
            continue;
        }
        if (subLsaFound[subTlvType]) {
            hasDuplicate = true;
            break;
        }
        subLsaFound[subTlvType] = true;
        switch (subTlvType) {
        case OSPFOpaqueTESubTLVType::LINK_TYPE:
        case OSPFOpaqueTESubTLVType::LINK_ID:
        case OSPFOpaqueTESubTLVType::LOCAL_IP:
        case OSPFOpaqueTESubTLVType::REMOTE_IP:
        case OSPFOpaqueTESubTLVType::TE_METRIC:
        case OSPFOpaqueTESubTLVType::MAX_BANDWIDTH:
        case OSPFOpaqueTESubTLVType::MAX_RESERVABLE_BANDWIDTH:
        case OSPFOpaqueTESubTLVType::UNRESERVED_BANDWIDTH:
        case OSPFOpaqueTESubTLVType::ADMINISTRATIVE_GROUP:
            break;
        }
    }
    return subLsaFound[OSPFOpaqueTESubTLVType::LINK_TYPE]
            && subLsaFound[OSPFOpaqueTESubTLVType::LINK_ID] && !hasDuplicate;
}

bool NetGraph::parseTETLVs(const OSPFOpaqueTLVs &tlvArray,
        TEInfo& result) {
    result = TEInfo();
    bool hasRA = false;
    bool hasDuplicate = false;

    for (int i = 0; i < (int) tlvArray.getTlvsArraySize(); i++) {
        if (hasDuplicate)
            break;

        const OSPFOpaqueTLV &tlv = tlvArray.getTlvs(i);
        short tlvType = tlv.getType();
        if (tlvType == OSPFOpaqueTETLVType::ROUTER_ADDRESS) {
            if (hasRA) {
                hasDuplicate = true;
                break;
            }
            hasRA = true;
            result.routerAddress = check_and_cast<const TLVipv4Val*>(
                    tlv.getValue())->get();
        } else if (tlvType == OSPFOpaqueTETLVType::LINK) {
            TELinkInfo linkInfo;
            if (parseTELinkTLV(tlv)) {
                result.links.push_back(linkInfo);
            }
        }
    }

    return hasRA && !hasDuplicate;
}

bool NetGraph::parseSIDLabelTLV(const OSPFOpaqueTLV *tlv,
        SegmentId &sid) {
    EV << "    parsing SID/Label sub-tlv";

    const PrefixSIDValue *sidVal = check_and_cast<const PrefixSIDValue*>(
            tlv->getValue());
    if (sidVal->getFlags().V_ValueIndex) {
        sid = sidVal->getLabelOrIdx();
        return true;
    }

    return srgb->getByIndex(sidVal->getLabelOrIdx(), sid);
}

bool NetGraph::parseExtendedPrefixLSA(const OSPFOpaqueLSA *lsa) {
    const OSPFOpaqueTLVs * tlvs = check_and_cast<const OSPFOpaqueTLVs*>(
            lsa->getInformation());

    EV << "  Parsing extended prefix LSA" << endl;
    for (int i = 0; i < (int) tlvs->getTlvsArraySize(); i++) {
        const OSPFOpaqueTLV *tlv = &(tlvs->getTlvs(i));
        if (tlv->getType() != ExtendedPrefixTLVTypes::EXTENDED_PREFIX) {
            EV << "  skipping unwanted TLV type" << endl;
            continue;
        }

        parseExtendedPrefixTLV(tlv);
    }
    return true;
}

bool NetGraph::parseExtendedPrefixTLV(const OSPFOpaqueTLV *tlv) {
    EV << "    Parsing extended prefix TLV" << endl;
    const ExtendedPrefixVal *tlvVal = check_and_cast<const ExtendedPrefixVal*>(
            tlv->getValue());

    if (tlvVal->getAddressFamily() != ExtendedPrefixAF::IPV4_UNICAST) {
        throw cRuntimeError("only ipv4 unicast supported");
    }

    //char prefLen = tlvVal->getPrefixLength();
    IPv4Address pref = tlvVal->getPrefix();

    for (int i = 0; i < (int) tlvVal->getSubTLVs().getTlvsArraySize(); i++) {
        const OSPFOpaqueTLV *subTlv = &(tlvVal->getSubTLVs().getTlvs(i));

        if (subTlv->getType() != ExtendedPrefixSubTLVTypes::SPRING_PREFIX_SID) {
            EV << "      skipping unwanted sub-TLV type: "
                      << (int) subTlv->getType() << endl;
            continue;
        }
        const PrefixSIDValue *prefSid = check_and_cast<const PrefixSIDValue*>(
                subTlv->getValue());

        //only local values implemented
        ASSERT(!prefSid->getFlags().V_ValueIndex);
        ASSERT(!prefSid->getFlags().L_LocalGlobal);

        SegmentId sid;
        bool ok = srgb->getByIndex(prefSid->getLabelOrIdx(), sid);
        ASSERT(ok);

        Node * node = findOrCreateNode(pref);
        node->setSid(sid);

        nodesBySid[sid] = node;
        EV << "      node-SID: " << sid << endl;
        return true;
    }

    return false;
}

bool NetGraph::parseExtendedLinkLSA(const OSPFOpaqueLSA *lsa) {
    const OSPFOpaqueTLVs * tlvs = check_and_cast<const OSPFOpaqueTLVs*>(
            lsa->getInformation());

    Node * srcNode = findOrCreateNode(lsa->getHeader().getAdvertisingRouter());

    EV << "  Parsing extended link LSA" << endl;
    for (int i = 0; i < (int) tlvs->getTlvsArraySize(); i++) {
        const OSPFOpaqueTLV *tlv = &(tlvs->getTlvs(i));
        if (tlv->getType() != ExtendedLinkTLVTypes::EXTENDED_LINK) {
            EV << "    skipping unwanted TLV type" << endl;
            continue;
        }

        parseExtendedLinkTLV(tlv, srcNode);
    }

    return true;

}

bool NetGraph::parseExtendedLinkTLV(const OSPFOpaqueTLV *tlv,
        Node *srcNode) {
    EV << "    Parsing extended link TLV" << endl;
    const ExtendedLinkVal *tlvVal = check_and_cast<const ExtendedLinkVal*>(
            tlv->getValue());

    if (tlvVal->getLinkType() != LinkType::POINTTOPOINT_LINK) {
        throw cRuntimeError("Only p2p links are supported now");
    }

    Node * adjNode = findOrCreateNode(tlvVal->getLinkID());
    //val->getLinkData(); //FIXME: not implemented
    for (int i = 0; i < (int) tlvVal->getSubTLVs().getTlvsArraySize(); i++) {
        const OSPFOpaqueTLV *subTlv = &(tlvVal->getSubTLVs().getTlvs(i));

        if (subTlv->getType() != ExtendedLinkSubTLVTypes::SPRING_ADJ_SID) {
            EV << "    skipping unwanted sub-TLV type" << endl;
            continue;
        }
        EV << "      parsing adjacency sid sub-tlv" << endl;

        const AdjSIDValue *sidVal = check_and_cast<const AdjSIDValue*>(
                subTlv->getValue());
        EV << "        ";

        //only local values implemented
        ASSERT(sidVal->getFlags().V_ValueIndex);
        ASSERT(sidVal->getFlags().L_LocalGlobal);
        SegmentId sid = sidVal->getLabelOrIdx();

        Link * link = findOrCreateLink(srcNode, adjNode);
        link->setWeight(sidVal->getCost() > 0 ? sidVal->getCost() : 1);
        link->setSid(sid);
        link->setDatarate(
                sidVal->getDatarate() > 0 ? sidVal->getDatarate() : 1);

        EV << " datarate: " << link->getDatarate();
        EV << " cost: " << link->getWeight();
        EV << " adj-SID: " << sid << endl;
    }

    return false;
}

void NetGraph::constructGraph(OpaqueLsaDatabase &lsaDatabase) {
    links.clear();
    allocedLinks.clear();

    if (rootNode) {
        IPv4Address rootNodeID = rootNode->getRouterID();
        clear();
        allocedNodes.clear();
        rootNode = findOrCreateNode(rootNodeID);
    } else {
        clear();
        allocedNodes.clear();
    }

    //parse the opaque LSAs and add the nodes and links to the topology
    for (const auto &lsa : lsaDatabase) {
        EV << *lsa;
        switch (lsa->getHeader().getOpaqueType()) {

        case OSPFOpaqueType::EXTENDED_LINK_TYPE:
            parseExtendedLinkLSA(lsa.get());
            break;
        case OSPFOpaqueType::EXTENDED_PREFIX_TYPE:
            parseExtendedPrefixLSA(lsa.get());
            break;
        }
    }


}
#endif //STANDALONE_STREETE



NetGraph::Node* NetGraph::getNodeBySid(SegmentId sid){
    auto it = nodesBySid.find(sid);
    if (it == nodesBySid.end()) {
        return NULL;
    }
    return it->second;
}
NetGraph::Node* NetGraph::getNodeByName(const std::string &name){
    auto it = nodesByName.find(name);
    if (it == nodesByName.end()) {
        return NULL;
    }
    return it->second;
}

std::ostream& operator<<(std::ostream& ostr, const NetGraph::Node& n) {
    if(n.name.empty()){
        ostr << n.routerID
                << "(sid=" << n.sid << " idx=" << n.getIndex() << ")";
    } else {
        ostr << n.name
                << "(sid=" << n.sid <<
                " rID=" << n.routerID <<
                " idx=" << n.getIndex() << ")";
    }

    return ostr;
}


void NetGraph::resetLinks() {
    for (LinkOut * link : links) {
        ((Link*) link)->enable();
    }
}

void NetGraph::computeRoutes() {
    ASSERT(nodes.size() > 0);
    bool OK = testIsConnected();
    ASSERT(OK);

    EV << "Computing Routes" << std::endl;
    dijkstra(rootNode->getIndex());
}

//void getPath(const )
NetGraph::Node * NetGraph::findOrCreateNode(const IPv4Address& ip) {
    Node * node;
    auto it = allocedNodes.find(ip);
    if (it == allocedNodes.end()) {
        node = (Node *) createNode();
        allocedNodes[ip] = node;

        node->setIndex(nodes.size());
        nodes.push_back(node);
    } else {
        node = it->second;
    }
    node->setRouterID(ip);
    return node;
}

NetGraph::Link * NetGraph::findOrCreateLink(Node * srcNode, Node *dstNode) {
    Link * link;
    LinkMapID id;
    id.srcIdx = srcNode->getIndex();
    id.dstIdx = dstNode->getIndex();

    auto it = allocedLinks.find(id);
    if (it == allocedLinks.end()) {
        link = (Link *) createLink();

        allocedLinks[id] = link;
        addLink(link, srcNode, dstNode);

        link->setIndex(links.size());
        links.push_back((LinkOut *) link);
    } else {
        link = it->second;
    }
    return link;
}

NetGraph::Link * NetGraph::createLink(SegmentId sid) {
    Link * l = new Link();
    l->setSid(sid);
    return l;
}

void NetGraph::disableLink(SegmentId nodeSid,
        SegmentId intfSid) {
    Node * node = getNodeBySid(nodeSid);
    if (!node) {
        return;
    }

    for (int i = 0; i < node->getNumOutLinks(); i++) {
        Link * link = (Link*) node->getLinkOut(i);
        if (link->getSid() == intfSid) {
            link->disable();
            EV << "Link disabled from node: " << nodeSid << " link sid: "
                      << intfSid << std::endl;
        }
    }
}



NetGraph::Link * NetGraph::findLinkBack(Link * link) {
    LinkOut * linkOut = (LinkOut *) link;
    Node * node = (Node *) linkOut->getRemoteNode();

    for (int i = 0; i < node->getNumOutLinks(); i++) {
        if (node->getLinkOut(i)->getRemoteNode() == linkOut->getLocalNode()) {
            return (Link*) node->getLinkOut(i);
        }
    }
    return NULL;
}


std::ostream& operator<<(std::ostream& ostr, const NetGraph::Link& l) {
    Topology::LinkOut &lOut = (Topology::LinkOut &) l;
    ostr << *((NetGraph::Node *) lOut.getLocalNode()) << "--("
            << "u=" << l.getBps()
            << " d=" << l.getDatarate() << ")-->"
            << *((NetGraph::Node *) lOut.getRemoteNode());
    return ostr;
}


} //namespace spring

} //namespace inet

