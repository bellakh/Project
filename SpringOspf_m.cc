//
// Generated file, do not edit! Created by nedtool 5.0 from inet/networklayer/spring/control/SpringOspf.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "SpringOspf_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: no doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

namespace inet {
namespace spring {

// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(SIDLabelValue);

SIDLabelValue::SIDLabelValue() : ::omnetpp::cObject()
{
}

SIDLabelValue::SIDLabelValue(const SIDLabelValue& other) : ::omnetpp::cObject(other)
{
    copy(other);
}

SIDLabelValue::~SIDLabelValue()
{
}

SIDLabelValue& SIDLabelValue::operator=(const SIDLabelValue& other)
{
    if (this==&other) return *this;
    ::omnetpp::cObject::operator=(other);
    copy(other);
    return *this;
}

void SIDLabelValue::copy(const SIDLabelValue& other)
{
    this->sid = other.sid;
}

void SIDLabelValue::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->sid);
}

void SIDLabelValue::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->sid);
}

SegmentId& SIDLabelValue::getSid()
{
    return this->sid;
}

void SIDLabelValue::setSid(const SegmentId& sid)
{
    this->sid = sid;
}

class SIDLabelValueDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SIDLabelValueDescriptor();
    virtual ~SIDLabelValueDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SIDLabelValueDescriptor);

SIDLabelValueDescriptor::SIDLabelValueDescriptor() : omnetpp::cClassDescriptor("inet::spring::SIDLabelValue", "omnetpp::cObject")
{
    propertynames = nullptr;
}

SIDLabelValueDescriptor::~SIDLabelValueDescriptor()
{
    delete[] propertynames;
}

bool SIDLabelValueDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SIDLabelValue *>(obj)!=nullptr;
}

const char **SIDLabelValueDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SIDLabelValueDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SIDLabelValueDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int SIDLabelValueDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *SIDLabelValueDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "sid",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int SIDLabelValueDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "sid")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SIDLabelValueDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "SegmentId",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **SIDLabelValueDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SIDLabelValueDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SIDLabelValueDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SIDLabelValue *pp = (SIDLabelValue *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIDLabelValueDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SIDLabelValue *pp = (SIDLabelValue *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getSid(); return out.str();}
        default: return "";
    }
}

bool SIDLabelValueDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SIDLabelValue *pp = (SIDLabelValue *)object; (void)pp;
    switch (field) {
        default: return false;
    }
}

const char *SIDLabelValueDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(SegmentId));
        default: return nullptr;
    };
}

void *SIDLabelValueDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SIDLabelValue *pp = (SIDLabelValue *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getSid()); break;
        default: return nullptr;
    }
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::spring::SRAlgorithmType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::spring::SRAlgorithmType"));
    e->insert(IGP_METRIC_BASED_SPT, "IGP_METRIC_BASED_SPT");
);

Register_Class(SRAlgorithmValue);

SRAlgorithmValue::SRAlgorithmValue() : ::omnetpp::cObject()
{
    algoritm_arraysize = 0;
    this->algoritm = 0;
}

SRAlgorithmValue::SRAlgorithmValue(const SRAlgorithmValue& other) : ::omnetpp::cObject(other)
{
    algoritm_arraysize = 0;
    this->algoritm = 0;
    copy(other);
}

SRAlgorithmValue::~SRAlgorithmValue()
{
    delete [] this->algoritm;
}

SRAlgorithmValue& SRAlgorithmValue::operator=(const SRAlgorithmValue& other)
{
    if (this==&other) return *this;
    ::omnetpp::cObject::operator=(other);
    copy(other);
    return *this;
}

void SRAlgorithmValue::copy(const SRAlgorithmValue& other)
{
    delete [] this->algoritm;
    this->algoritm = (other.algoritm_arraysize==0) ? nullptr : new char[other.algoritm_arraysize];
    algoritm_arraysize = other.algoritm_arraysize;
    for (unsigned int i=0; i<algoritm_arraysize; i++)
        this->algoritm[i] = other.algoritm[i];
}

void SRAlgorithmValue::parsimPack(omnetpp::cCommBuffer *b) const
{
    b->pack(algoritm_arraysize);
    doParsimArrayPacking(b,this->algoritm,algoritm_arraysize);
}

void SRAlgorithmValue::parsimUnpack(omnetpp::cCommBuffer *b)
{
    delete [] this->algoritm;
    b->unpack(algoritm_arraysize);
    if (algoritm_arraysize==0) {
        this->algoritm = 0;
    } else {
        this->algoritm = new char[algoritm_arraysize];
        doParsimArrayUnpacking(b,this->algoritm,algoritm_arraysize);
    }
}

void SRAlgorithmValue::setAlgoritmArraySize(unsigned int size)
{
    char *algoritm2 = (size==0) ? nullptr : new char[size];
    unsigned int sz = algoritm_arraysize < size ? algoritm_arraysize : size;
    for (unsigned int i=0; i<sz; i++)
        algoritm2[i] = this->algoritm[i];
    for (unsigned int i=sz; i<size; i++)
        algoritm2[i] = 0;
    algoritm_arraysize = size;
    delete [] this->algoritm;
    this->algoritm = algoritm2;
}

unsigned int SRAlgorithmValue::getAlgoritmArraySize() const
{
    return algoritm_arraysize;
}

char SRAlgorithmValue::getAlgoritm(unsigned int k) const
{
    if (k>=algoritm_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", algoritm_arraysize, k);
    return this->algoritm[k];
}

void SRAlgorithmValue::setAlgoritm(unsigned int k, char algoritm)
{
    if (k>=algoritm_arraysize) throw omnetpp::cRuntimeError("Array of size %d indexed by %d", algoritm_arraysize, k);
    this->algoritm[k] = algoritm;
}

class SRAlgorithmValueDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SRAlgorithmValueDescriptor();
    virtual ~SRAlgorithmValueDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SRAlgorithmValueDescriptor);

SRAlgorithmValueDescriptor::SRAlgorithmValueDescriptor() : omnetpp::cClassDescriptor("inet::spring::SRAlgorithmValue", "omnetpp::cObject")
{
    propertynames = nullptr;
}

SRAlgorithmValueDescriptor::~SRAlgorithmValueDescriptor()
{
    delete[] propertynames;
}

bool SRAlgorithmValueDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SRAlgorithmValue *>(obj)!=nullptr;
}

const char **SRAlgorithmValueDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SRAlgorithmValueDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SRAlgorithmValueDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 1+basedesc->getFieldCount() : 1;
}

unsigned int SRAlgorithmValueDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE,
    };
    return (field>=0 && field<1) ? fieldTypeFlags[field] : 0;
}

const char *SRAlgorithmValueDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "algoritm",
    };
    return (field>=0 && field<1) ? fieldNames[field] : nullptr;
}

int SRAlgorithmValueDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='a' && strcmp(fieldName, "algoritm")==0) return base+0;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SRAlgorithmValueDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "char",
    };
    return (field>=0 && field<1) ? fieldTypeStrings[field] : nullptr;
}

const char **SRAlgorithmValueDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: {
            static const char *names[] = { "enum",  nullptr };
            return names;
        }
        default: return nullptr;
    }
}

const char *SRAlgorithmValueDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0:
            if (!strcmp(propertyname,"enum")) return "inet::spring::SRAlgorithmType";
            return nullptr;
        default: return nullptr;
    }
}

int SRAlgorithmValueDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SRAlgorithmValue *pp = (SRAlgorithmValue *)object; (void)pp;
    switch (field) {
        case 0: return pp->getAlgoritmArraySize();
        default: return 0;
    }
}

std::string SRAlgorithmValueDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SRAlgorithmValue *pp = (SRAlgorithmValue *)object; (void)pp;
    switch (field) {
        case 0: return enum2string(pp->getAlgoritm(i), "inet::spring::SRAlgorithmType");
        default: return "";
    }
}

bool SRAlgorithmValueDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SRAlgorithmValue *pp = (SRAlgorithmValue *)object; (void)pp;
    switch (field) {
        case 0: pp->setAlgoritm(i,(inet::spring::SRAlgorithmType)string2enum(value, "inet::spring::SRAlgorithmType")); return true;
        default: return false;
    }
}

const char *SRAlgorithmValueDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *SRAlgorithmValueDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SRAlgorithmValue *pp = (SRAlgorithmValue *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

PrefixSIDFlags::PrefixSIDFlags()
{
    this->N_NodeSID = false;
    this->NP_NoPHP = false;
    this->M_MappingServer = false;
    this->E_ExplicitNULL = false;
    this->V_ValueIndex = false;
    this->L_LocalGlobal = false;
    this->reserved1 = false;
    this->reserved2 = false;
}

void __doPacking(omnetpp::cCommBuffer *b, const PrefixSIDFlags& a)
{
    doParsimPacking(b,a.N_NodeSID);
    doParsimPacking(b,a.NP_NoPHP);
    doParsimPacking(b,a.M_MappingServer);
    doParsimPacking(b,a.E_ExplicitNULL);
    doParsimPacking(b,a.V_ValueIndex);
    doParsimPacking(b,a.L_LocalGlobal);
    doParsimPacking(b,a.reserved1);
    doParsimPacking(b,a.reserved2);
}

void __doUnpacking(omnetpp::cCommBuffer *b, PrefixSIDFlags& a)
{
    doParsimUnpacking(b,a.N_NodeSID);
    doParsimUnpacking(b,a.NP_NoPHP);
    doParsimUnpacking(b,a.M_MappingServer);
    doParsimUnpacking(b,a.E_ExplicitNULL);
    doParsimUnpacking(b,a.V_ValueIndex);
    doParsimUnpacking(b,a.L_LocalGlobal);
    doParsimUnpacking(b,a.reserved1);
    doParsimUnpacking(b,a.reserved2);
}

class PrefixSIDFlagsDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PrefixSIDFlagsDescriptor();
    virtual ~PrefixSIDFlagsDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PrefixSIDFlagsDescriptor);

PrefixSIDFlagsDescriptor::PrefixSIDFlagsDescriptor() : omnetpp::cClassDescriptor("inet::spring::PrefixSIDFlags", "")
{
    propertynames = nullptr;
}

PrefixSIDFlagsDescriptor::~PrefixSIDFlagsDescriptor()
{
    delete[] propertynames;
}

bool PrefixSIDFlagsDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrefixSIDFlags *>(obj)!=nullptr;
}

const char **PrefixSIDFlagsDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PrefixSIDFlagsDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PrefixSIDFlagsDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int PrefixSIDFlagsDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *PrefixSIDFlagsDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "N_NodeSID",
        "NP_NoPHP",
        "M_MappingServer",
        "E_ExplicitNULL",
        "V_ValueIndex",
        "L_LocalGlobal",
        "reserved1",
        "reserved2",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int PrefixSIDFlagsDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='N' && strcmp(fieldName, "N_NodeSID")==0) return base+0;
    if (fieldName[0]=='N' && strcmp(fieldName, "NP_NoPHP")==0) return base+1;
    if (fieldName[0]=='M' && strcmp(fieldName, "M_MappingServer")==0) return base+2;
    if (fieldName[0]=='E' && strcmp(fieldName, "E_ExplicitNULL")==0) return base+3;
    if (fieldName[0]=='V' && strcmp(fieldName, "V_ValueIndex")==0) return base+4;
    if (fieldName[0]=='L' && strcmp(fieldName, "L_LocalGlobal")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved1")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved2")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PrefixSIDFlagsDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **PrefixSIDFlagsDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrefixSIDFlagsDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrefixSIDFlagsDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDFlags *pp = (PrefixSIDFlags *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PrefixSIDFlagsDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDFlags *pp = (PrefixSIDFlags *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->N_NodeSID);
        case 1: return bool2string(pp->NP_NoPHP);
        case 2: return bool2string(pp->M_MappingServer);
        case 3: return bool2string(pp->E_ExplicitNULL);
        case 4: return bool2string(pp->V_ValueIndex);
        case 5: return bool2string(pp->L_LocalGlobal);
        case 6: return bool2string(pp->reserved1);
        case 7: return bool2string(pp->reserved2);
        default: return "";
    }
}

bool PrefixSIDFlagsDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDFlags *pp = (PrefixSIDFlags *)object; (void)pp;
    switch (field) {
        case 0: pp->N_NodeSID = string2bool(value); return true;
        case 1: pp->NP_NoPHP = string2bool(value); return true;
        case 2: pp->M_MappingServer = string2bool(value); return true;
        case 3: pp->E_ExplicitNULL = string2bool(value); return true;
        case 4: pp->V_ValueIndex = string2bool(value); return true;
        case 5: pp->L_LocalGlobal = string2bool(value); return true;
        case 6: pp->reserved1 = string2bool(value); return true;
        case 7: pp->reserved2 = string2bool(value); return true;
        default: return false;
    }
}

const char *PrefixSIDFlagsDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *PrefixSIDFlagsDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDFlags *pp = (PrefixSIDFlags *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(PrefixSIDValue);

PrefixSIDValue::PrefixSIDValue() : ::omnetpp::cObject()
{
    this->reserved = 0;
    this->multiTopoID = 0;
    this->algorithm = 0;
    this->labelOrIdx = 0;
}

PrefixSIDValue::PrefixSIDValue(const PrefixSIDValue& other) : ::omnetpp::cObject(other)
{
    copy(other);
}

PrefixSIDValue::~PrefixSIDValue()
{
}

PrefixSIDValue& PrefixSIDValue::operator=(const PrefixSIDValue& other)
{
    if (this==&other) return *this;
    ::omnetpp::cObject::operator=(other);
    copy(other);
    return *this;
}

void PrefixSIDValue::copy(const PrefixSIDValue& other)
{
    this->flags = other.flags;
    this->reserved = other.reserved;
    this->multiTopoID = other.multiTopoID;
    this->algorithm = other.algorithm;
    this->labelOrIdx = other.labelOrIdx;
}

void PrefixSIDValue::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->multiTopoID);
    doParsimPacking(b,this->algorithm);
    doParsimPacking(b,this->labelOrIdx);
}

void PrefixSIDValue::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->multiTopoID);
    doParsimUnpacking(b,this->algorithm);
    doParsimUnpacking(b,this->labelOrIdx);
}

PrefixSIDFlags& PrefixSIDValue::getFlags()
{
    return this->flags;
}

void PrefixSIDValue::setFlags(const PrefixSIDFlags& flags)
{
    this->flags = flags;
}

char PrefixSIDValue::getReserved() const
{
    return this->reserved;
}

void PrefixSIDValue::setReserved(char reserved)
{
    this->reserved = reserved;
}

char PrefixSIDValue::getMultiTopoID() const
{
    return this->multiTopoID;
}

void PrefixSIDValue::setMultiTopoID(char multiTopoID)
{
    this->multiTopoID = multiTopoID;
}

char PrefixSIDValue::getAlgorithm() const
{
    return this->algorithm;
}

void PrefixSIDValue::setAlgorithm(char algorithm)
{
    this->algorithm = algorithm;
}

int PrefixSIDValue::getLabelOrIdx() const
{
    return this->labelOrIdx;
}

void PrefixSIDValue::setLabelOrIdx(int labelOrIdx)
{
    this->labelOrIdx = labelOrIdx;
}

class PrefixSIDValueDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    PrefixSIDValueDescriptor();
    virtual ~PrefixSIDValueDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(PrefixSIDValueDescriptor);

PrefixSIDValueDescriptor::PrefixSIDValueDescriptor() : omnetpp::cClassDescriptor("inet::spring::PrefixSIDValue", "omnetpp::cObject")
{
    propertynames = nullptr;
}

PrefixSIDValueDescriptor::~PrefixSIDValueDescriptor()
{
    delete[] propertynames;
}

bool PrefixSIDValueDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PrefixSIDValue *>(obj)!=nullptr;
}

const char **PrefixSIDValueDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *PrefixSIDValueDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int PrefixSIDValueDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int PrefixSIDValueDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *PrefixSIDValueDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "flags",
        "reserved",
        "multiTopoID",
        "algorithm",
        "labelOrIdx",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int PrefixSIDValueDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "multiTopoID")==0) return base+2;
    if (fieldName[0]=='a' && strcmp(fieldName, "algorithm")==0) return base+3;
    if (fieldName[0]=='l' && strcmp(fieldName, "labelOrIdx")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *PrefixSIDValueDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "PrefixSIDFlags",
        "char",
        "char",
        "char",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **PrefixSIDValueDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PrefixSIDValueDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PrefixSIDValueDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDValue *pp = (PrefixSIDValue *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string PrefixSIDValueDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDValue *pp = (PrefixSIDValue *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFlags(); return out.str();}
        case 1: return long2string(pp->getReserved());
        case 2: return long2string(pp->getMultiTopoID());
        case 3: return long2string(pp->getAlgorithm());
        case 4: return long2string(pp->getLabelOrIdx());
        default: return "";
    }
}

bool PrefixSIDValueDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDValue *pp = (PrefixSIDValue *)object; (void)pp;
    switch (field) {
        case 1: pp->setReserved(string2long(value)); return true;
        case 2: pp->setMultiTopoID(string2long(value)); return true;
        case 3: pp->setAlgorithm(string2long(value)); return true;
        case 4: pp->setLabelOrIdx(string2long(value)); return true;
        default: return false;
    }
}

const char *PrefixSIDValueDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(PrefixSIDFlags));
        default: return nullptr;
    };
}

void *PrefixSIDValueDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    PrefixSIDValue *pp = (PrefixSIDValue *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFlags()); break;
        default: return nullptr;
    }
}

EXECUTE_ON_STARTUP(
    omnetpp::cEnum *e = omnetpp::cEnum::find("inet::spring::SIDLabelBindSubTLVType");
    if (!e) omnetpp::enums.getInstance()->add(e = new omnetpp::cEnum("inet::spring::SIDLabelBindSubTLVType"));
    e->insert(SID_LABEL, "SID_LABEL");
    e->insert(IPV4_ERO, "IPV4_ERO");
    e->insert(UNNUMBERED_INTERFACE_ERO, "UNNUMBERED_INTERFACE_ERO");
    e->insert(IPV4_BACKUP_ERO, "IPV4_BACKUP_ERO");
    e->insert(UNNUMBERED_INTERFACE_BACKUP_ERO, "UNNUMBERED_INTERFACE_BACKUP_ERO");
    e->insert(ERO_METRIC, "ERO_METRIC");
);

SidLabelBindFlags::SidLabelBindFlags()
{
    this->M_MirroringContext = false;
    this->reserved1 = false;
    this->reserved2 = false;
    this->reserved3 = false;
    this->reserved4 = false;
    this->reserved5 = false;
    this->reserved6 = false;
    this->reserved7 = false;
}

void __doPacking(omnetpp::cCommBuffer *b, const SidLabelBindFlags& a)
{
    doParsimPacking(b,a.M_MirroringContext);
    doParsimPacking(b,a.reserved1);
    doParsimPacking(b,a.reserved2);
    doParsimPacking(b,a.reserved3);
    doParsimPacking(b,a.reserved4);
    doParsimPacking(b,a.reserved5);
    doParsimPacking(b,a.reserved6);
    doParsimPacking(b,a.reserved7);
}

void __doUnpacking(omnetpp::cCommBuffer *b, SidLabelBindFlags& a)
{
    doParsimUnpacking(b,a.M_MirroringContext);
    doParsimUnpacking(b,a.reserved1);
    doParsimUnpacking(b,a.reserved2);
    doParsimUnpacking(b,a.reserved3);
    doParsimUnpacking(b,a.reserved4);
    doParsimUnpacking(b,a.reserved5);
    doParsimUnpacking(b,a.reserved6);
    doParsimUnpacking(b,a.reserved7);
}

class SidLabelBindFlagsDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SidLabelBindFlagsDescriptor();
    virtual ~SidLabelBindFlagsDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SidLabelBindFlagsDescriptor);

SidLabelBindFlagsDescriptor::SidLabelBindFlagsDescriptor() : omnetpp::cClassDescriptor("inet::spring::SidLabelBindFlags", "")
{
    propertynames = nullptr;
}

SidLabelBindFlagsDescriptor::~SidLabelBindFlagsDescriptor()
{
    delete[] propertynames;
}

bool SidLabelBindFlagsDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SidLabelBindFlags *>(obj)!=nullptr;
}

const char **SidLabelBindFlagsDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SidLabelBindFlagsDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SidLabelBindFlagsDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int SidLabelBindFlagsDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *SidLabelBindFlagsDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "M_MirroringContext",
        "reserved1",
        "reserved2",
        "reserved3",
        "reserved4",
        "reserved5",
        "reserved6",
        "reserved7",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int SidLabelBindFlagsDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='M' && strcmp(fieldName, "M_MirroringContext")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved1")==0) return base+1;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved2")==0) return base+2;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved3")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved4")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved5")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved6")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved7")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SidLabelBindFlagsDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **SidLabelBindFlagsDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SidLabelBindFlagsDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SidLabelBindFlagsDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SidLabelBindFlags *pp = (SidLabelBindFlags *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SidLabelBindFlagsDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SidLabelBindFlags *pp = (SidLabelBindFlags *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->M_MirroringContext);
        case 1: return bool2string(pp->reserved1);
        case 2: return bool2string(pp->reserved2);
        case 3: return bool2string(pp->reserved3);
        case 4: return bool2string(pp->reserved4);
        case 5: return bool2string(pp->reserved5);
        case 6: return bool2string(pp->reserved6);
        case 7: return bool2string(pp->reserved7);
        default: return "";
    }
}

bool SidLabelBindFlagsDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SidLabelBindFlags *pp = (SidLabelBindFlags *)object; (void)pp;
    switch (field) {
        case 0: pp->M_MirroringContext = string2bool(value); return true;
        case 1: pp->reserved1 = string2bool(value); return true;
        case 2: pp->reserved2 = string2bool(value); return true;
        case 3: pp->reserved3 = string2bool(value); return true;
        case 4: pp->reserved4 = string2bool(value); return true;
        case 5: pp->reserved5 = string2bool(value); return true;
        case 6: pp->reserved6 = string2bool(value); return true;
        case 7: pp->reserved7 = string2bool(value); return true;
        default: return false;
    }
}

const char *SidLabelBindFlagsDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *SidLabelBindFlagsDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SidLabelBindFlags *pp = (SidLabelBindFlags *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(SIDLabelBindValue);

SIDLabelBindValue::SIDLabelBindValue() : ::omnetpp::cObject()
{
    this->reserved = 0;
    this->multiTopoID = 0;
    this->weight = 0;
}

SIDLabelBindValue::SIDLabelBindValue(const SIDLabelBindValue& other) : ::omnetpp::cObject(other)
{
    copy(other);
}

SIDLabelBindValue::~SIDLabelBindValue()
{
}

SIDLabelBindValue& SIDLabelBindValue::operator=(const SIDLabelBindValue& other)
{
    if (this==&other) return *this;
    ::omnetpp::cObject::operator=(other);
    copy(other);
    return *this;
}

void SIDLabelBindValue::copy(const SIDLabelBindValue& other)
{
    this->flags = other.flags;
    this->reserved = other.reserved;
    this->multiTopoID = other.multiTopoID;
    this->weight = other.weight;
    this->subTLVs = other.subTLVs;
}

void SIDLabelBindValue::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->multiTopoID);
    doParsimPacking(b,this->weight);
    doParsimPacking(b,this->subTLVs);
}

void SIDLabelBindValue::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->multiTopoID);
    doParsimUnpacking(b,this->weight);
    doParsimUnpacking(b,this->subTLVs);
}

SidLabelBindFlags& SIDLabelBindValue::getFlags()
{
    return this->flags;
}

void SIDLabelBindValue::setFlags(const SidLabelBindFlags& flags)
{
    this->flags = flags;
}

char SIDLabelBindValue::getReserved() const
{
    return this->reserved;
}

void SIDLabelBindValue::setReserved(char reserved)
{
    this->reserved = reserved;
}

char SIDLabelBindValue::getMultiTopoID() const
{
    return this->multiTopoID;
}

void SIDLabelBindValue::setMultiTopoID(char multiTopoID)
{
    this->multiTopoID = multiTopoID;
}

char SIDLabelBindValue::getWeight() const
{
    return this->weight;
}

void SIDLabelBindValue::setWeight(char weight)
{
    this->weight = weight;
}

inet::ospf::OSPFOpaqueTLVs& SIDLabelBindValue::getSubTLVs()
{
    return this->subTLVs;
}

void SIDLabelBindValue::setSubTLVs(const inet::ospf::OSPFOpaqueTLVs& subTLVs)
{
    this->subTLVs = subTLVs;
}

class SIDLabelBindValueDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    SIDLabelBindValueDescriptor();
    virtual ~SIDLabelBindValueDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(SIDLabelBindValueDescriptor);

SIDLabelBindValueDescriptor::SIDLabelBindValueDescriptor() : omnetpp::cClassDescriptor("inet::spring::SIDLabelBindValue", "omnetpp::cObject")
{
    propertynames = nullptr;
}

SIDLabelBindValueDescriptor::~SIDLabelBindValueDescriptor()
{
    delete[] propertynames;
}

bool SIDLabelBindValueDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<SIDLabelBindValue *>(obj)!=nullptr;
}

const char **SIDLabelBindValueDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *SIDLabelBindValueDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int SIDLabelBindValueDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount() : 5;
}

unsigned int SIDLabelBindValueDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *SIDLabelBindValueDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "flags",
        "reserved",
        "multiTopoID",
        "weight",
        "subTLVs",
    };
    return (field>=0 && field<5) ? fieldNames[field] : nullptr;
}

int SIDLabelBindValueDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "multiTopoID")==0) return base+2;
    if (fieldName[0]=='w' && strcmp(fieldName, "weight")==0) return base+3;
    if (fieldName[0]=='s' && strcmp(fieldName, "subTLVs")==0) return base+4;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *SIDLabelBindValueDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "SidLabelBindFlags",
        "char",
        "char",
        "char",
        "inet::ospf::OSPFOpaqueTLVs",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : nullptr;
}

const char **SIDLabelBindValueDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *SIDLabelBindValueDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int SIDLabelBindValueDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    SIDLabelBindValue *pp = (SIDLabelBindValue *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string SIDLabelBindValueDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    SIDLabelBindValue *pp = (SIDLabelBindValue *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFlags(); return out.str();}
        case 1: return long2string(pp->getReserved());
        case 2: return long2string(pp->getMultiTopoID());
        case 3: return long2string(pp->getWeight());
        case 4: {std::stringstream out; out << pp->getSubTLVs(); return out.str();}
        default: return "";
    }
}

bool SIDLabelBindValueDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    SIDLabelBindValue *pp = (SIDLabelBindValue *)object; (void)pp;
    switch (field) {
        case 1: pp->setReserved(string2long(value)); return true;
        case 2: pp->setMultiTopoID(string2long(value)); return true;
        case 3: pp->setWeight(string2long(value)); return true;
        default: return false;
    }
}

const char *SIDLabelBindValueDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(SidLabelBindFlags));
        case 4: return omnetpp::opp_typename(typeid(inet::ospf::OSPFOpaqueTLVs));
        default: return nullptr;
    };
}

void *SIDLabelBindValueDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    SIDLabelBindValue *pp = (SIDLabelBindValue *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFlags()); break;
        case 4: return (void *)(&pp->getSubTLVs()); break;
        default: return nullptr;
    }
}

AdjSIDFlags::AdjSIDFlags()
{
    this->B_Backup = false;
    this->V_ValueIndex = false;
    this->L_LocalGlobal = false;
    this->S_Set = false;
    this->reserved1 = false;
    this->reserved2 = false;
    this->reserved3 = false;
    this->reserved4 = false;
}

void __doPacking(omnetpp::cCommBuffer *b, const AdjSIDFlags& a)
{
    doParsimPacking(b,a.B_Backup);
    doParsimPacking(b,a.V_ValueIndex);
    doParsimPacking(b,a.L_LocalGlobal);
    doParsimPacking(b,a.S_Set);
    doParsimPacking(b,a.reserved1);
    doParsimPacking(b,a.reserved2);
    doParsimPacking(b,a.reserved3);
    doParsimPacking(b,a.reserved4);
}

void __doUnpacking(omnetpp::cCommBuffer *b, AdjSIDFlags& a)
{
    doParsimUnpacking(b,a.B_Backup);
    doParsimUnpacking(b,a.V_ValueIndex);
    doParsimUnpacking(b,a.L_LocalGlobal);
    doParsimUnpacking(b,a.S_Set);
    doParsimUnpacking(b,a.reserved1);
    doParsimUnpacking(b,a.reserved2);
    doParsimUnpacking(b,a.reserved3);
    doParsimUnpacking(b,a.reserved4);
}

class AdjSIDFlagsDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    AdjSIDFlagsDescriptor();
    virtual ~AdjSIDFlagsDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(AdjSIDFlagsDescriptor);

AdjSIDFlagsDescriptor::AdjSIDFlagsDescriptor() : omnetpp::cClassDescriptor("inet::spring::AdjSIDFlags", "")
{
    propertynames = nullptr;
}

AdjSIDFlagsDescriptor::~AdjSIDFlagsDescriptor()
{
    delete[] propertynames;
}

bool AdjSIDFlagsDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AdjSIDFlags *>(obj)!=nullptr;
}

const char **AdjSIDFlagsDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *AdjSIDFlagsDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int AdjSIDFlagsDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 8+basedesc->getFieldCount() : 8;
}

unsigned int AdjSIDFlagsDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<8) ? fieldTypeFlags[field] : 0;
}

const char *AdjSIDFlagsDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "B_Backup",
        "V_ValueIndex",
        "L_LocalGlobal",
        "S_Set",
        "reserved1",
        "reserved2",
        "reserved3",
        "reserved4",
    };
    return (field>=0 && field<8) ? fieldNames[field] : nullptr;
}

int AdjSIDFlagsDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='B' && strcmp(fieldName, "B_Backup")==0) return base+0;
    if (fieldName[0]=='V' && strcmp(fieldName, "V_ValueIndex")==0) return base+1;
    if (fieldName[0]=='L' && strcmp(fieldName, "L_LocalGlobal")==0) return base+2;
    if (fieldName[0]=='S' && strcmp(fieldName, "S_Set")==0) return base+3;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved1")==0) return base+4;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved2")==0) return base+5;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved3")==0) return base+6;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved4")==0) return base+7;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *AdjSIDFlagsDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
        "bool",
    };
    return (field>=0 && field<8) ? fieldTypeStrings[field] : nullptr;
}

const char **AdjSIDFlagsDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AdjSIDFlagsDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AdjSIDFlagsDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    AdjSIDFlags *pp = (AdjSIDFlags *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AdjSIDFlagsDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    AdjSIDFlags *pp = (AdjSIDFlags *)object; (void)pp;
    switch (field) {
        case 0: return bool2string(pp->B_Backup);
        case 1: return bool2string(pp->V_ValueIndex);
        case 2: return bool2string(pp->L_LocalGlobal);
        case 3: return bool2string(pp->S_Set);
        case 4: return bool2string(pp->reserved1);
        case 5: return bool2string(pp->reserved2);
        case 6: return bool2string(pp->reserved3);
        case 7: return bool2string(pp->reserved4);
        default: return "";
    }
}

bool AdjSIDFlagsDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    AdjSIDFlags *pp = (AdjSIDFlags *)object; (void)pp;
    switch (field) {
        case 0: pp->B_Backup = string2bool(value); return true;
        case 1: pp->V_ValueIndex = string2bool(value); return true;
        case 2: pp->L_LocalGlobal = string2bool(value); return true;
        case 3: pp->S_Set = string2bool(value); return true;
        case 4: pp->reserved1 = string2bool(value); return true;
        case 5: pp->reserved2 = string2bool(value); return true;
        case 6: pp->reserved3 = string2bool(value); return true;
        case 7: pp->reserved4 = string2bool(value); return true;
        default: return false;
    }
}

const char *AdjSIDFlagsDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *AdjSIDFlagsDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    AdjSIDFlags *pp = (AdjSIDFlags *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

Register_Class(AdjSIDValue);

AdjSIDValue::AdjSIDValue() : ::omnetpp::cObject()
{
    this->reserved = 0;
    this->multiTopoID = 0;
    this->weight = 0;
    this->labelOrIdx = 0;
    this->cost = 0;
    this->datarate = 0;
}

AdjSIDValue::AdjSIDValue(const AdjSIDValue& other) : ::omnetpp::cObject(other)
{
    copy(other);
}

AdjSIDValue::~AdjSIDValue()
{
}

AdjSIDValue& AdjSIDValue::operator=(const AdjSIDValue& other)
{
    if (this==&other) return *this;
    ::omnetpp::cObject::operator=(other);
    copy(other);
    return *this;
}

void AdjSIDValue::copy(const AdjSIDValue& other)
{
    this->flags = other.flags;
    this->reserved = other.reserved;
    this->multiTopoID = other.multiTopoID;
    this->weight = other.weight;
    this->labelOrIdx = other.labelOrIdx;
    this->cost = other.cost;
    this->datarate = other.datarate;
}

void AdjSIDValue::parsimPack(omnetpp::cCommBuffer *b) const
{
    doParsimPacking(b,this->flags);
    doParsimPacking(b,this->reserved);
    doParsimPacking(b,this->multiTopoID);
    doParsimPacking(b,this->weight);
    doParsimPacking(b,this->labelOrIdx);
    doParsimPacking(b,this->cost);
    doParsimPacking(b,this->datarate);
}

void AdjSIDValue::parsimUnpack(omnetpp::cCommBuffer *b)
{
    doParsimUnpacking(b,this->flags);
    doParsimUnpacking(b,this->reserved);
    doParsimUnpacking(b,this->multiTopoID);
    doParsimUnpacking(b,this->weight);
    doParsimUnpacking(b,this->labelOrIdx);
    doParsimUnpacking(b,this->cost);
    doParsimUnpacking(b,this->datarate);
}

AdjSIDFlags& AdjSIDValue::getFlags()
{
    return this->flags;
}

void AdjSIDValue::setFlags(const AdjSIDFlags& flags)
{
    this->flags = flags;
}

char AdjSIDValue::getReserved() const
{
    return this->reserved;
}

void AdjSIDValue::setReserved(char reserved)
{
    this->reserved = reserved;
}

char AdjSIDValue::getMultiTopoID() const
{
    return this->multiTopoID;
}

void AdjSIDValue::setMultiTopoID(char multiTopoID)
{
    this->multiTopoID = multiTopoID;
}

char AdjSIDValue::getWeight() const
{
    return this->weight;
}

void AdjSIDValue::setWeight(char weight)
{
    this->weight = weight;
}

int AdjSIDValue::getLabelOrIdx() const
{
    return this->labelOrIdx;
}

void AdjSIDValue::setLabelOrIdx(int labelOrIdx)
{
    this->labelOrIdx = labelOrIdx;
}

double AdjSIDValue::getCost() const
{
    return this->cost;
}

void AdjSIDValue::setCost(double cost)
{
    this->cost = cost;
}

double AdjSIDValue::getDatarate() const
{
    return this->datarate;
}

void AdjSIDValue::setDatarate(double datarate)
{
    this->datarate = datarate;
}

class AdjSIDValueDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    AdjSIDValueDescriptor();
    virtual ~AdjSIDValueDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(AdjSIDValueDescriptor);

AdjSIDValueDescriptor::AdjSIDValueDescriptor() : omnetpp::cClassDescriptor("inet::spring::AdjSIDValue", "omnetpp::cObject")
{
    propertynames = nullptr;
}

AdjSIDValueDescriptor::~AdjSIDValueDescriptor()
{
    delete[] propertynames;
}

bool AdjSIDValueDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<AdjSIDValue *>(obj)!=nullptr;
}

const char **AdjSIDValueDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *AdjSIDValueDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int AdjSIDValueDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 7+basedesc->getFieldCount() : 7;
}

unsigned int AdjSIDValueDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<7) ? fieldTypeFlags[field] : 0;
}

const char *AdjSIDValueDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "flags",
        "reserved",
        "multiTopoID",
        "weight",
        "labelOrIdx",
        "cost",
        "datarate",
    };
    return (field>=0 && field<7) ? fieldNames[field] : nullptr;
}

int AdjSIDValueDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='f' && strcmp(fieldName, "flags")==0) return base+0;
    if (fieldName[0]=='r' && strcmp(fieldName, "reserved")==0) return base+1;
    if (fieldName[0]=='m' && strcmp(fieldName, "multiTopoID")==0) return base+2;
    if (fieldName[0]=='w' && strcmp(fieldName, "weight")==0) return base+3;
    if (fieldName[0]=='l' && strcmp(fieldName, "labelOrIdx")==0) return base+4;
    if (fieldName[0]=='c' && strcmp(fieldName, "cost")==0) return base+5;
    if (fieldName[0]=='d' && strcmp(fieldName, "datarate")==0) return base+6;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *AdjSIDValueDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "AdjSIDFlags",
        "char",
        "char",
        "char",
        "int",
        "double",
        "double",
    };
    return (field>=0 && field<7) ? fieldTypeStrings[field] : nullptr;
}

const char **AdjSIDValueDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *AdjSIDValueDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int AdjSIDValueDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    AdjSIDValue *pp = (AdjSIDValue *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string AdjSIDValueDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    AdjSIDValue *pp = (AdjSIDValue *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getFlags(); return out.str();}
        case 1: return long2string(pp->getReserved());
        case 2: return long2string(pp->getMultiTopoID());
        case 3: return long2string(pp->getWeight());
        case 4: return long2string(pp->getLabelOrIdx());
        case 5: return double2string(pp->getCost());
        case 6: return double2string(pp->getDatarate());
        default: return "";
    }
}

bool AdjSIDValueDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    AdjSIDValue *pp = (AdjSIDValue *)object; (void)pp;
    switch (field) {
        case 1: pp->setReserved(string2long(value)); return true;
        case 2: pp->setMultiTopoID(string2long(value)); return true;
        case 3: pp->setWeight(string2long(value)); return true;
        case 4: pp->setLabelOrIdx(string2long(value)); return true;
        case 5: pp->setCost(string2double(value)); return true;
        case 6: pp->setDatarate(string2double(value)); return true;
        default: return false;
    }
}

const char *AdjSIDValueDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(AdjSIDFlags));
        default: return nullptr;
    };
}

void *AdjSIDValueDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    AdjSIDValue *pp = (AdjSIDValue *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getFlags()); break;
        default: return nullptr;
    }
}

} // namespace spring
} // namespace inet

