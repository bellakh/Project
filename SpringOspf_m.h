//
// Generated file, do not edit! Created by nedtool 5.0 from inet/networklayer/spring/control/SpringOspf.msg.
//

#ifndef __INET__SPRING_SPRINGOSPF_M_H
#define __INET__SPRING_SPRINGOSPF_M_H

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0500
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// cplusplus {{
#include "inet/common/INETDefs.h"
#include "inet/networklayer/spring/SPRINGDefs.h"
#include "inet/routing/ospfv2/opaque/OSPFOpaque.h"
// }}


namespace inet {
namespace spring {

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:41</tt> by nedtool.
 * <pre>
 * //
 * //section-2.1
 * //
 * class SIDLabelValue
 * {
 *     SegmentId sid;
 * }
 * </pre>
 */
class SIDLabelValue : public ::omnetpp::cObject
{
  protected:
    SegmentId sid;

  private:
    void copy(const SIDLabelValue& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SIDLabelValue&);

  public:
    SIDLabelValue();
    SIDLabelValue(const SIDLabelValue& other);
    virtual ~SIDLabelValue();
    SIDLabelValue& operator=(const SIDLabelValue& other);
    virtual SIDLabelValue *dup() const {return new SIDLabelValue(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual SegmentId& getSid();
    virtual const SegmentId& getSid() const {return const_cast<SIDLabelValue*>(this)->getSid();}
    virtual void setSid(const SegmentId& sid);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SIDLabelValue& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SIDLabelValue& obj) {obj.parsimUnpack(b);}

/**
 * Enum generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:50</tt> by nedtool.
 * <pre>
 * //
 * //section-3.1
 * //
 * enum SRAlgorithmType
 * {
 * 
 *     IGP_METRIC_BASED_SPT = 0;
 * }
 * </pre>
 */
enum SRAlgorithmType {
    IGP_METRIC_BASED_SPT = 0
};

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:54</tt> by nedtool.
 * <pre>
 * class SRAlgorithmValue
 * {
 *     char algoritm[] @enum(SRAlgorithmType);
 * }
 * </pre>
 */
class SRAlgorithmValue : public ::omnetpp::cObject
{
  protected:
    char *algoritm; // array ptr
    unsigned int algoritm_arraysize;

  private:
    void copy(const SRAlgorithmValue& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SRAlgorithmValue&);

  public:
    SRAlgorithmValue();
    SRAlgorithmValue(const SRAlgorithmValue& other);
    virtual ~SRAlgorithmValue();
    SRAlgorithmValue& operator=(const SRAlgorithmValue& other);
    virtual SRAlgorithmValue *dup() const {return new SRAlgorithmValue(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual void setAlgoritmArraySize(unsigned int size);
    virtual unsigned int getAlgoritmArraySize() const;
    virtual char getAlgoritm(unsigned int k) const;
    virtual void setAlgoritm(unsigned int k, char algoritm);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SRAlgorithmValue& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SRAlgorithmValue& obj) {obj.parsimUnpack(b);}

/**
 * Struct generated from inet/networklayer/spring/control/SpringOspf.msg:62 by nedtool.
 */
struct PrefixSIDFlags
{
    PrefixSIDFlags();
    bool N_NodeSID;
    bool NP_NoPHP;
    bool M_MappingServer;
    bool E_ExplicitNULL;
    bool V_ValueIndex;
    bool L_LocalGlobal;
    bool reserved1;
    bool reserved2;
};

// helpers for local use
void __doPacking(omnetpp::cCommBuffer *b, const PrefixSIDFlags& a);
void __doUnpacking(omnetpp::cCommBuffer *b, PrefixSIDFlags& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const PrefixSIDFlags& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, PrefixSIDFlags& obj) { __doUnpacking(b, obj); }

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:73</tt> by nedtool.
 * <pre>
 * class PrefixSIDValue
 * {
 *     PrefixSIDFlags flags;
 *     char reserved;
 *     char multiTopoID;
 *     char algorithm;
 *     int labelOrIdx;
 * }
 * </pre>
 */
class PrefixSIDValue : public ::omnetpp::cObject
{
  protected:
    PrefixSIDFlags flags;
    char reserved;
    char multiTopoID;
    char algorithm;
    int labelOrIdx;

  private:
    void copy(const PrefixSIDValue& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PrefixSIDValue&);

  public:
    PrefixSIDValue();
    PrefixSIDValue(const PrefixSIDValue& other);
    virtual ~PrefixSIDValue();
    PrefixSIDValue& operator=(const PrefixSIDValue& other);
    virtual PrefixSIDValue *dup() const {return new PrefixSIDValue(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual PrefixSIDFlags& getFlags();
    virtual const PrefixSIDFlags& getFlags() const {return const_cast<PrefixSIDValue*>(this)->getFlags();}
    virtual void setFlags(const PrefixSIDFlags& flags);
    virtual char getReserved() const;
    virtual void setReserved(char reserved);
    virtual char getMultiTopoID() const;
    virtual void setMultiTopoID(char multiTopoID);
    virtual char getAlgorithm() const;
    virtual void setAlgorithm(char algorithm);
    virtual int getLabelOrIdx() const;
    virtual void setLabelOrIdx(int labelOrIdx);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const PrefixSIDValue& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, PrefixSIDValue& obj) {obj.parsimUnpack(b);}

/**
 * Enum generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:85</tt> by nedtool.
 * <pre>
 * //
 * //section-6
 * //
 * enum SIDLabelBindSubTLVType
 * {
 * 
 *     SID_LABEL = 1;
 *     IPV4_ERO = 4;
 *     UNNUMBERED_INTERFACE_ERO = 5;
 *     IPV4_BACKUP_ERO = 6;
 *     UNNUMBERED_INTERFACE_BACKUP_ERO = 7;
 *     ERO_METRIC = 8;
 * }
 * </pre>
 */
enum SIDLabelBindSubTLVType {
    SID_LABEL = 1,
    IPV4_ERO = 4,
    UNNUMBERED_INTERFACE_ERO = 5,
    IPV4_BACKUP_ERO = 6,
    UNNUMBERED_INTERFACE_BACKUP_ERO = 7,
    ERO_METRIC = 8
};

/**
 * Struct generated from inet/networklayer/spring/control/SpringOspf.msg:93 by nedtool.
 */
struct SidLabelBindFlags
{
    SidLabelBindFlags();
    bool M_MirroringContext;
    bool reserved1;
    bool reserved2;
    bool reserved3;
    bool reserved4;
    bool reserved5;
    bool reserved6;
    bool reserved7;
};

// helpers for local use
void __doPacking(omnetpp::cCommBuffer *b, const SidLabelBindFlags& a);
void __doUnpacking(omnetpp::cCommBuffer *b, SidLabelBindFlags& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SidLabelBindFlags& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SidLabelBindFlags& obj) { __doUnpacking(b, obj); }

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:104</tt> by nedtool.
 * <pre>
 * class SIDLabelBindValue
 * {
 *     SidLabelBindFlags flags;
 *     char reserved;
 *     char multiTopoID;
 *     char weight;
 * 
 *     inet::ospf::OSPFOpaqueTLVs subTLVs;
 * }
 * </pre>
 */
class SIDLabelBindValue : public ::omnetpp::cObject
{
  protected:
    SidLabelBindFlags flags;
    char reserved;
    char multiTopoID;
    char weight;
    inet::ospf::OSPFOpaqueTLVs subTLVs;

  private:
    void copy(const SIDLabelBindValue& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const SIDLabelBindValue&);

  public:
    SIDLabelBindValue();
    SIDLabelBindValue(const SIDLabelBindValue& other);
    virtual ~SIDLabelBindValue();
    SIDLabelBindValue& operator=(const SIDLabelBindValue& other);
    virtual SIDLabelBindValue *dup() const {return new SIDLabelBindValue(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual SidLabelBindFlags& getFlags();
    virtual const SidLabelBindFlags& getFlags() const {return const_cast<SIDLabelBindValue*>(this)->getFlags();}
    virtual void setFlags(const SidLabelBindFlags& flags);
    virtual char getReserved() const;
    virtual void setReserved(char reserved);
    virtual char getMultiTopoID() const;
    virtual void setMultiTopoID(char multiTopoID);
    virtual char getWeight() const;
    virtual void setWeight(char weight);
    virtual inet::ospf::OSPFOpaqueTLVs& getSubTLVs();
    virtual const inet::ospf::OSPFOpaqueTLVs& getSubTLVs() const {return const_cast<SIDLabelBindValue*>(this)->getSubTLVs();}
    virtual void setSubTLVs(const inet::ospf::OSPFOpaqueTLVs& subTLVs);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const SIDLabelBindValue& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, SIDLabelBindValue& obj) {obj.parsimUnpack(b);}

/**
 * Struct generated from inet/networklayer/spring/control/SpringOspf.msg:116 by nedtool.
 */
struct AdjSIDFlags
{
    AdjSIDFlags();
    bool B_Backup;
    bool V_ValueIndex;
    bool L_LocalGlobal;
    bool S_Set;
    bool reserved1;
    bool reserved2;
    bool reserved3;
    bool reserved4;
};

// helpers for local use
void __doPacking(omnetpp::cCommBuffer *b, const AdjSIDFlags& a);
void __doUnpacking(omnetpp::cCommBuffer *b, AdjSIDFlags& a);

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AdjSIDFlags& obj) { __doPacking(b, obj); }
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AdjSIDFlags& obj) { __doUnpacking(b, obj); }

/**
 * Class generated from <tt>inet/networklayer/spring/control/SpringOspf.msg:128</tt> by nedtool.
 * <pre>
 * class AdjSIDValue
 * {
 *     AdjSIDFlags flags;
 *     char reserved;
 *     char multiTopoID;
 *     char weight;
 *     int labelOrIdx;
 * 
 *     double cost; //TODO: remove, not in specification
 *     double datarate;
 * }
 * </pre>
 */
class AdjSIDValue : public ::omnetpp::cObject
{
  protected:
    AdjSIDFlags flags;
    char reserved;
    char multiTopoID;
    char weight;
    int labelOrIdx;
    double cost;
    double datarate;

  private:
    void copy(const AdjSIDValue& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const AdjSIDValue&);

  public:
    AdjSIDValue();
    AdjSIDValue(const AdjSIDValue& other);
    virtual ~AdjSIDValue();
    AdjSIDValue& operator=(const AdjSIDValue& other);
    virtual AdjSIDValue *dup() const {return new AdjSIDValue(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b);

    // field getter/setter methods
    virtual AdjSIDFlags& getFlags();
    virtual const AdjSIDFlags& getFlags() const {return const_cast<AdjSIDValue*>(this)->getFlags();}
    virtual void setFlags(const AdjSIDFlags& flags);
    virtual char getReserved() const;
    virtual void setReserved(char reserved);
    virtual char getMultiTopoID() const;
    virtual void setMultiTopoID(char multiTopoID);
    virtual char getWeight() const;
    virtual void setWeight(char weight);
    virtual int getLabelOrIdx() const;
    virtual void setLabelOrIdx(int labelOrIdx);
    virtual double getCost() const;
    virtual void setCost(double cost);
    virtual double getDatarate() const;
    virtual void setDatarate(double datarate);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const AdjSIDValue& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, AdjSIDValue& obj) {obj.parsimUnpack(b);}

} // namespace spring
} // namespace inet

#endif // ifndef __INET__SPRING_SPRINGOSPF_M_H

