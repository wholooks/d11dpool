//
// Generated file, do not edit! Created by opp_msgtool 6.0 from pool_message.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "pool_message_m.h"

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
    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) {
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
    for (int i = 0; i < n; i++) {
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
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

pool_message_reference::pool_message_reference()
{
}

void __doPacking(omnetpp::cCommBuffer *b, const pool_message_reference& a)
{
    doParsimPacking(b,a.source);
    doParsimPacking(b,a.sequenceNumber);
}

void __doUnpacking(omnetpp::cCommBuffer *b, pool_message_reference& a)
{
    doParsimUnpacking(b,a.source);
    doParsimUnpacking(b,a.sequenceNumber);
}

class pool_message_referenceDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_source,
        FIELD_sequenceNumber,
    };
  public:
    pool_message_referenceDescriptor();
    virtual ~pool_message_referenceDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(pool_message_referenceDescriptor)

pool_message_referenceDescriptor::pool_message_referenceDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(pool_message_reference)), "")
{
    propertyNames = nullptr;
}

pool_message_referenceDescriptor::~pool_message_referenceDescriptor()
{
    delete[] propertyNames;
}

bool pool_message_referenceDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<pool_message_reference *>(obj)!=nullptr;
}

const char **pool_message_referenceDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *pool_message_referenceDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int pool_message_referenceDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int pool_message_referenceDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_source
        FD_ISEDITABLE,    // FIELD_sequenceNumber
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *pool_message_referenceDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source",
        "sequenceNumber",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int pool_message_referenceDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *pool_message_referenceDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_source
        "int",    // FIELD_sequenceNumber
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **pool_message_referenceDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *pool_message_referenceDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int pool_message_referenceDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        default: return 0;
    }
}

void pool_message_referenceDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'pool_message_reference'", field);
    }
}

const char *pool_message_referenceDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string pool_message_referenceDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        case FIELD_source: return long2string(pp->source);
        case FIELD_sequenceNumber: return long2string(pp->sequenceNumber);
        default: return "";
    }
}

void pool_message_referenceDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->source = string2long(value); break;
        case FIELD_sequenceNumber: pp->sequenceNumber = string2long(value); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'pool_message_reference'", field);
    }
}

omnetpp::cValue pool_message_referenceDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        case FIELD_source: return pp->source;
        case FIELD_sequenceNumber: return pp->sequenceNumber;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'pool_message_reference' as cValue -- field index out of range?", field);
    }
}

void pool_message_referenceDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->source = omnetpp::checked_int_cast<int>(value.intValue()); break;
        case FIELD_sequenceNumber: pp->sequenceNumber = omnetpp::checked_int_cast<int>(value.intValue()); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'pool_message_reference'", field);
    }
}

const char *pool_message_referenceDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr pool_message_referenceDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void pool_message_referenceDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    pool_message_reference *pp = omnetpp::fromAnyPtr<pool_message_reference>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'pool_message_reference'", field);
    }
}

Register_Class(PoolMessage)

PoolMessage::PoolMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

PoolMessage::PoolMessage(const PoolMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

PoolMessage::~PoolMessage()
{
    delete [] this->ref;
}

PoolMessage& PoolMessage::operator=(const PoolMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void PoolMessage::copy(const PoolMessage& other)
{
    this->source = other.source;
    this->destination = other.destination;
    this->sequenceNumber = other.sequenceNumber;
    this->hopCount = other.hopCount;
    delete [] this->ref;
    this->ref = (other.ref_arraysize==0) ? nullptr : new pool_message_reference[other.ref_arraysize];
    ref_arraysize = other.ref_arraysize;
    for (size_t i = 0; i < ref_arraysize; i++) {
        this->ref[i] = other.ref[i];
    }
}

void PoolMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->source);
    doParsimPacking(b,this->destination);
    doParsimPacking(b,this->sequenceNumber);
    doParsimPacking(b,this->hopCount);
    b->pack(ref_arraysize);
    doParsimArrayPacking(b,this->ref,ref_arraysize);
}

void PoolMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->source);
    doParsimUnpacking(b,this->destination);
    doParsimUnpacking(b,this->sequenceNumber);
    doParsimUnpacking(b,this->hopCount);
    delete [] this->ref;
    b->unpack(ref_arraysize);
    if (ref_arraysize == 0) {
        this->ref = nullptr;
    } else {
        this->ref = new pool_message_reference[ref_arraysize];
        doParsimArrayUnpacking(b,this->ref,ref_arraysize);
    }
}

int PoolMessage::getSource() const
{
    return this->source;
}

void PoolMessage::setSource(int source)
{
    this->source = source;
}

int PoolMessage::getDestination() const
{
    return this->destination;
}

void PoolMessage::setDestination(int destination)
{
    this->destination = destination;
}

int PoolMessage::getSequenceNumber() const
{
    return this->sequenceNumber;
}

void PoolMessage::setSequenceNumber(int sequenceNumber)
{
    this->sequenceNumber = sequenceNumber;
}

int PoolMessage::getHopCount() const
{
    return this->hopCount;
}

void PoolMessage::setHopCount(int hopCount)
{
    this->hopCount = hopCount;
}

size_t PoolMessage::getRefArraySize() const
{
    return ref_arraysize;
}

const pool_message_reference& PoolMessage::getRef(size_t k) const
{
    if (k >= ref_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)ref_arraysize, (unsigned long)k);
    return this->ref[k];
}

void PoolMessage::setRefArraySize(size_t newSize)
{
    pool_message_reference *ref2 = (newSize==0) ? nullptr : new pool_message_reference[newSize];
    size_t minSize = ref_arraysize < newSize ? ref_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        ref2[i] = this->ref[i];
    delete [] this->ref;
    this->ref = ref2;
    ref_arraysize = newSize;
}

void PoolMessage::setRef(size_t k, const pool_message_reference& ref)
{
    if (k >= ref_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)ref_arraysize, (unsigned long)k);
    this->ref[k] = ref;
}

void PoolMessage::insertRef(size_t k, const pool_message_reference& ref)
{
    if (k > ref_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)ref_arraysize, (unsigned long)k);
    size_t newSize = ref_arraysize + 1;
    pool_message_reference *ref2 = new pool_message_reference[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        ref2[i] = this->ref[i];
    ref2[k] = ref;
    for (i = k + 1; i < newSize; i++)
        ref2[i] = this->ref[i-1];
    delete [] this->ref;
    this->ref = ref2;
    ref_arraysize = newSize;
}

void PoolMessage::appendRef(const pool_message_reference& ref)
{
    insertRef(ref_arraysize, ref);
}

void PoolMessage::eraseRef(size_t k)
{
    if (k >= ref_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)ref_arraysize, (unsigned long)k);
    size_t newSize = ref_arraysize - 1;
    pool_message_reference *ref2 = (newSize == 0) ? nullptr : new pool_message_reference[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        ref2[i] = this->ref[i];
    for (i = k; i < newSize; i++)
        ref2[i] = this->ref[i+1];
    delete [] this->ref;
    this->ref = ref2;
    ref_arraysize = newSize;
}

class PoolMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_source,
        FIELD_destination,
        FIELD_sequenceNumber,
        FIELD_hopCount,
        FIELD_ref,
    };
  public:
    PoolMessageDescriptor();
    virtual ~PoolMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(PoolMessageDescriptor)

PoolMessageDescriptor::PoolMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(PoolMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

PoolMessageDescriptor::~PoolMessageDescriptor()
{
    delete[] propertyNames;
}

bool PoolMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<PoolMessage *>(obj)!=nullptr;
}

const char **PoolMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *PoolMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int PoolMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 5+base->getFieldCount() : 5;
}

unsigned int PoolMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_source
        FD_ISEDITABLE,    // FIELD_destination
        FD_ISEDITABLE,    // FIELD_sequenceNumber
        FD_ISEDITABLE,    // FIELD_hopCount
        FD_ISARRAY | FD_ISCOMPOUND | FD_ISRESIZABLE,    // FIELD_ref
    };
    return (field >= 0 && field < 5) ? fieldTypeFlags[field] : 0;
}

const char *PoolMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "source",
        "destination",
        "sequenceNumber",
        "hopCount",
        "ref",
    };
    return (field >= 0 && field < 5) ? fieldNames[field] : nullptr;
}

int PoolMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "source") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "destination") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "sequenceNumber") == 0) return baseIndex + 2;
    if (strcmp(fieldName, "hopCount") == 0) return baseIndex + 3;
    if (strcmp(fieldName, "ref") == 0) return baseIndex + 4;
    return base ? base->findField(fieldName) : -1;
}

const char *PoolMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_source
        "int",    // FIELD_destination
        "int",    // FIELD_sequenceNumber
        "int",    // FIELD_hopCount
        "pool_message_reference",    // FIELD_ref
    };
    return (field >= 0 && field < 5) ? fieldTypeStrings[field] : nullptr;
}

const char **PoolMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *PoolMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int PoolMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ref: return pp->getRefArraySize();
        default: return 0;
    }
}

void PoolMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ref: pp->setRefArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'PoolMessage'", field);
    }
}

const char *PoolMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string PoolMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_source: return long2string(pp->getSource());
        case FIELD_destination: return long2string(pp->getDestination());
        case FIELD_sequenceNumber: return long2string(pp->getSequenceNumber());
        case FIELD_hopCount: return long2string(pp->getHopCount());
        case FIELD_ref: return "";
        default: return "";
    }
}

void PoolMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->setSource(string2long(value)); break;
        case FIELD_destination: pp->setDestination(string2long(value)); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(string2long(value)); break;
        case FIELD_hopCount: pp->setHopCount(string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PoolMessage'", field);
    }
}

omnetpp::cValue PoolMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_source: return pp->getSource();
        case FIELD_destination: return pp->getDestination();
        case FIELD_sequenceNumber: return pp->getSequenceNumber();
        case FIELD_hopCount: return pp->getHopCount();
        case FIELD_ref: return omnetpp::toAnyPtr(&pp->getRef(i)); break;
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'PoolMessage' as cValue -- field index out of range?", field);
    }
}

void PoolMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_source: pp->setSource(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_destination: pp->setDestination(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_sequenceNumber: pp->setSequenceNumber(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_hopCount: pp->setHopCount(omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PoolMessage'", field);
    }
}

const char *PoolMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        case FIELD_ref: return omnetpp::opp_typename(typeid(pool_message_reference));
        default: return nullptr;
    };
}

omnetpp::any_ptr PoolMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        case FIELD_ref: return omnetpp::toAnyPtr(&pp->getRef(i)); break;
        default: return omnetpp::any_ptr(nullptr);
    }
}

void PoolMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    PoolMessage *pp = omnetpp::fromAnyPtr<PoolMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'PoolMessage'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

