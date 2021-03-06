//
// Generated file, do not edit! Created by nedtool 5.0 from inet/networklayer/spring/control/SpringControlPackets.msg.
//

#ifndef __INET__SPRING_SPRINGCONTROLPACKETS_M_H
#define __INET__SPRING_SPRINGCONTROLPACKETS_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0500
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// cplusplus {{
#include <vector>
#include <utility>
#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/networklayer/spring/control/ERO.h"
#include "inet/networklayer/spring/control/NetGraph.h"
// }}


namespace inet {
namespace spring {

// cplusplus {{
typedef std::vector<std::pair<SegmentId, double>> StatsVector;
typedef std::vector<std::pair<unsigned int, SegmentId>> OffLinks;
// }}

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringControlPackets.msg:38</tt> by nedtool.
 * <pre>
 * packet UDPStats
 * {
 *     SegmentId origSid;
 *     StatsVector stats;
 * }
 * </pre>
 */
class UDPStats : public ::omnetpp::cPacket
{
  protected:
    SegmentId origSid;
    StatsVector stats;

  private:
    void copy(const UDPStats& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const UDPStats&);

  public:
    UDPStats(const char *name=nullptr, int kind=0);
    UDPStats(const UDPStats& other);
    virtual ~UDPStats();
    UDPStats& operator=(const UDPStats& other);
    virtual UDPStats *dup() const {return new UDPStats(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual SegmentId& getOrigSid();
    virtual const SegmentId& getOrigSid() const {return const_cast<UDPStats*>(this)->getOrigSid();}
    virtual void setOrigSid(const SegmentId& origSid);
    virtual StatsVector& getStats();
    virtual const StatsVector& getStats() const {return const_cast<UDPStats*>(this)->getStats();}
    virtual void setStats(const StatsVector& stats);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const UDPStats& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, UDPStats& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringControlPackets.msg:43</tt> by nedtool.
 * <pre>
 * packet UDPEros
 * {
 *     EROs eros;
 * }
 * </pre>
 */
class UDPEros : public ::omnetpp::cPacket
{
  protected:
    EROs eros;

  private:
    void copy(const UDPEros& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const UDPEros&);

  public:
    UDPEros(const char *name=nullptr, int kind=0);
    UDPEros(const UDPEros& other);
    virtual ~UDPEros();
    UDPEros& operator=(const UDPEros& other);
    virtual UDPEros *dup() const {return new UDPEros(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual EROs& getEros();
    virtual const EROs& getEros() const {return const_cast<UDPEros*>(this)->getEros();}
    virtual void setEros(const EROs& eros);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const UDPEros& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, UDPEros& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringControlPackets.msg:48</tt> by nedtool.
 * <pre>
 * class OSPFOpaqueLinksOff
 * {
 *     OffLinks offLinks;
 * }
 * </pre>
 */
class OSPFOpaqueLinksOff : public ::omnetpp::cObject
{
  protected:
    OffLinks offLinks;

  private:
    void copy(const OSPFOpaqueLinksOff& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const OSPFOpaqueLinksOff&);

  public:
    OSPFOpaqueLinksOff();
    OSPFOpaqueLinksOff(const OSPFOpaqueLinksOff& other);
    virtual ~OSPFOpaqueLinksOff();
    OSPFOpaqueLinksOff& operator=(const OSPFOpaqueLinksOff& other);
    virtual OSPFOpaqueLinksOff *dup() const {return new OSPFOpaqueLinksOff(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual OffLinks& getOffLinks();
    virtual const OffLinks& getOffLinks() const {return const_cast<OSPFOpaqueLinksOff*>(this)->getOffLinks();}
    virtual void setOffLinks(const OffLinks& offLinks);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const OSPFOpaqueLinksOff& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, OSPFOpaqueLinksOff& obj) {obj.parsimUnpack(b);}

} // namespace spring
} // namespace inet

#endif // ifndef __INET__SPRING_SPRINGCONTROLPACKETS_M_H

