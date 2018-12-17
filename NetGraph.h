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

#ifndef SPRINGNETWORK_H_
#define SPRINGNETWORK_H_

#include <unordered_map>


#if !STANDALONE_STREETE
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"
#include "inet/routing/ospfv2/router/LSA.h"
#include "inet/routing/ospfv2/opaque/OSPFTE.h"
#include "inet/networklayer/spring/control/SpringOspf_m.h"
#include "inet/networklayer/spring/control/SpringControlPackets_m.h"
#endif

#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/base/FlowGraph.h"
#include "inet/networklayer/spring/base/GraphDSSSPTree.h"
#include "inet/networklayer/spring/control/SRGBDatabase.h"
#include "inet/networklayer/spring/control/ERO.h"

namespace inet {


#if !STANDALONE_STREETE
using namespace ospf;
#endif

namespace spring {

class NetGraph: public FlowGraph {
public:
    class Link: public FlowGraph::Link {
        friend class NetGraph;
    protected:
        SegmentId sid;
        double datarate;
    public:
        Link() : FlowGraph::Link(){sid=0; currentBps = 0;}

        void setSid(SegmentId _sid) {sid = _sid;}
        SegmentId getSid(){return sid;}

        void setDatarate(double _datarate) {datarate = _datarate;}
        double getDatarate() const {return datarate;}
        double getUtilization() const {return datarate>0?(currentBps/datarate):0;} //TODO

        friend std::ostream& operator<<(std::ostream& ostr, const Link& l) ;
    };

    class Node: public FlowGraph::Node {
        friend class NetworkGraph;
    protected:
        SegmentId sid;
        IPv4Address routerID;
        std::string name;
    public:
        Node(SegmentId id) : FlowGraph::Node(id) {sid = 0;}
        Node() : FlowGraph::Node(){sid = 0;}

        void setName(const std::string &_name) {name = _name;}
        const std::string& getName(){return name;}
        void setSid(SegmentId _sid) {sid = _sid;}
        SegmentId getSid(){return sid;}
        void setRouterID(IPv4Address &ip){routerID = ip;}
        void setRouterID(const IPv4Address &ip){routerID = ip;}
        IPv4Address& getRouterID(){return routerID;}

        friend std::ostream& operator<<(std::ostream& ostr, const Node& n);
    };

private:
    //for the hash tables
    struct hash_IPv4Address {
        size_t operator()(const IPv4Address &ip) const {
            return std::hash<int>()((int) ip.getInt());
        }
    };
    struct equals_IPv4Address {
        size_t operator()(const IPv4Address &ip1,
                const IPv4Address &ip2) const {
            return ip1 == ip2;
        }
    };
    struct LinkMapID {
        int srcIdx;
        int dstIdx;
        //IPv4Address srcIntfIP;

        struct hash {
            size_t operator()(const LinkMapID &id) const {
                return 37 * std::hash<int>()(id.srcIdx) + std::hash<int>()(id.dstIdx);
            }
        };
        struct equals {
            size_t operator()(const LinkMapID &id1,
                    const LinkMapID &id2) const {
                return id1.srcIdx == id2.srcIdx && id1.dstIdx == id2.dstIdx;
            }
        };
    };

protected:
    //keeps track of the already created nodes
    std::unordered_map<IPv4Address, Node *, hash_IPv4Address, equals_IPv4Address> allocedNodes;
    SRGBDatabase * srgb;
    Node * rootNode;

    std::unordered_map<SegmentId, Node *> nodesBySid;
    std::unordered_map<std::string, Node *> nodesByName;
    std::unordered_map<LinkMapID, Link *, LinkMapID::hash, LinkMapID::equals> allocedLinks;

public:

#if STANDALONE_STREETE
#else
    NetGraph(const IPv4Address &rootNodeID, SRGBDatabase * _srgb, bool ecmpAware=false);
    NetGraph( SRGBDatabase * _srgb, bool ecmpAware=false);

    void constructGraph(OpaqueLsaDatabase &lsaDatabase) ;
#endif
    NetGraph( );
    virtual ~NetGraph(){}

    Node * findOrCreateNode(const IPv4Address& ip);
    Link * findOrCreateLink(Node * srcNode, Node *dstNode ) ;
    Link * findLinkBack(Link * link);
    Node * getNodeBySid(SegmentId sid);
    Node * getNodeByName(const std::string &name);
    Link * createLink(SegmentId sid);
    void disableLink(SegmentId nodeSid, SegmentId intfSid);

#if STANDALONE_STREETE
#else //STANDALONE_STREETE
    bool parseTELinkTLV(const OSPFOpaqueTLV &tlv) ;
    bool parseTETLVs(const OSPFOpaqueTLVs &tlvArray, TEInfo& result) ;
    bool parseSIDLabelTLV(const OSPFOpaqueTLV *tlv, SegmentId &sid) ;
    bool parseExtendedPrefixLSA(const OSPFOpaqueLSA *lsa);
    bool parseExtendedPrefixTLV(const OSPFOpaqueTLV *tlv) ;
    bool parseExtendedLinkLSA(const OSPFOpaqueLSA *lsa) ;
    bool parseExtendedLinkTLV(const OSPFOpaqueTLV *tlv, Node *srcNode);
#endif //STANDALONE_STREETE


    void resetLinks();
    void computeRoutes();

protected:
#if !STANDALONE_STREETE
    virtual Topology::Node *createNode(cModule *module) {return new Node(module->getId());}
#endif
    virtual Topology::Node *createNode() {return new Node();}
    virtual Topology::Link *createLink() {return new Link();}
};



} //namespace spring

} //namespace inet



#endif /* SPRINGNETWORK_H_ */
