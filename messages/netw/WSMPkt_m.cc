//
// Generated file, do not edit! Created by opp_msgc 4.1 from messages/netw/WSMPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "WSMPkt_m.h"

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// Another default rule (prevents compiler from choosing base class' doPacking())
template<typename T>
void doPacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doPacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}

template<typename T>
void doUnpacking(cCommBuffer *, T& t) {
    throw cRuntimeError("Parsim error: no doUnpacking() function for type %s or its base class (check .msg and _m.cc/h files!)",opp_typename(typeid(t)));
}




Register_Class(WSMPkt);

WSMPkt::WSMPkt(const char *name, int kind) : NetwPkt(name,kind)
{
    this->chNum_var = 0;
    this->dataRate_var = 0;
    this->appPSI_var = 0;
    this->txPower_var = 0;
    this->length_var = 0;
}

WSMPkt::WSMPkt(const WSMPkt& other) : NetwPkt()
{
    setName(other.getName());
    operator=(other);
}

WSMPkt::~WSMPkt()
{
}

WSMPkt& WSMPkt::operator=(const WSMPkt& other)
{
    if (this==&other) return *this;
    NetwPkt::operator=(other);
    this->chNum_var = other.chNum_var;
    this->dataRate_var = other.dataRate_var;
    this->appPSI_var = other.appPSI_var;
    this->txPower_var = other.txPower_var;
    this->length_var = other.length_var;
    return *this;
}

void WSMPkt::parsimPack(cCommBuffer *b)
{
    NetwPkt::parsimPack(b);
    doPacking(b,this->chNum_var);
    doPacking(b,this->dataRate_var);
    doPacking(b,this->appPSI_var);
    doPacking(b,this->txPower_var);
    doPacking(b,this->length_var);
}

void WSMPkt::parsimUnpack(cCommBuffer *b)
{
    NetwPkt::parsimUnpack(b);
    doUnpacking(b,this->chNum_var);
    doUnpacking(b,this->dataRate_var);
    doUnpacking(b,this->appPSI_var);
    doUnpacking(b,this->txPower_var);
    doUnpacking(b,this->length_var);
}

int WSMPkt::getChNum() const
{
    return chNum_var;
}

void WSMPkt::setChNum(int chNum_var)
{
    this->chNum_var = chNum_var;
}

int WSMPkt::getDataRate() const
{
    return dataRate_var;
}

void WSMPkt::setDataRate(int dataRate_var)
{
    this->dataRate_var = dataRate_var;
}

int WSMPkt::getAppPSI() const
{
    return appPSI_var;
}

void WSMPkt::setAppPSI(int appPSI_var)
{
    this->appPSI_var = appPSI_var;
}

int WSMPkt::getTxPower() const
{
    return txPower_var;
}

void WSMPkt::setTxPower(int txPower_var)
{
    this->txPower_var = txPower_var;
}

int WSMPkt::getLength() const
{
    return length_var;
}

void WSMPkt::setLength(int length_var)
{
    this->length_var = length_var;
}

class WSMPktDescriptor : public cClassDescriptor
{
  public:
    WSMPktDescriptor();
    virtual ~WSMPktDescriptor();

    virtual bool doesSupport(cObject *obj) const;
    virtual const char *getProperty(const char *propertyname) const;
    virtual int getFieldCount(void *object) const;
    virtual const char *getFieldName(void *object, int field) const;
    virtual int findField(void *object, const char *fieldName) const;
    virtual unsigned int getFieldTypeFlags(void *object, int field) const;
    virtual const char *getFieldTypeString(void *object, int field) const;
    virtual const char *getFieldProperty(void *object, int field, const char *propertyname) const;
    virtual int getArraySize(void *object, int field) const;

    virtual std::string getFieldAsString(void *object, int field, int i) const;
    virtual bool setFieldAsString(void *object, int field, int i, const char *value) const;

    virtual const char *getFieldStructName(void *object, int field) const;
    virtual void *getFieldStructPointer(void *object, int field, int i) const;
};

Register_ClassDescriptor(WSMPktDescriptor);

WSMPktDescriptor::WSMPktDescriptor() : cClassDescriptor("WSMPkt", "NetwPkt")
{
}

WSMPktDescriptor::~WSMPktDescriptor()
{
}

bool WSMPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<WSMPkt *>(obj)!=NULL;
}

const char *WSMPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int WSMPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 5+basedesc->getFieldCount(object) : 5;
}

unsigned int WSMPktDescriptor::getFieldTypeFlags(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeFlags(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<5) ? fieldTypeFlags[field] : 0;
}

const char *WSMPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "chNum",
        "dataRate",
        "appPSI",
        "txPower",
        "length",
    };
    return (field>=0 && field<5) ? fieldNames[field] : NULL;
}

int WSMPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='c' && strcmp(fieldName, "chNum")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dataRate")==0) return base+1;
    if (fieldName[0]=='a' && strcmp(fieldName, "appPSI")==0) return base+2;
    if (fieldName[0]=='t' && strcmp(fieldName, "txPower")==0) return base+3;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+4;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *WSMPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<5) ? fieldTypeStrings[field] : NULL;
}

const char *WSMPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldProperty(object, field, propertyname);
        field -= basedesc->getFieldCount(object);
    }
    switch (field) {
        default: return NULL;
    }
}

int WSMPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    WSMPkt *pp = (WSMPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string WSMPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    WSMPkt *pp = (WSMPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getChNum());
        case 1: return long2string(pp->getDataRate());
        case 2: return long2string(pp->getAppPSI());
        case 3: return long2string(pp->getTxPower());
        case 4: return long2string(pp->getLength());
        default: return "";
    }
}

bool WSMPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    WSMPkt *pp = (WSMPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setChNum(string2long(value)); return true;
        case 1: pp->setDataRate(string2long(value)); return true;
        case 2: pp->setAppPSI(string2long(value)); return true;
        case 3: pp->setTxPower(string2long(value)); return true;
        case 4: pp->setLength(string2long(value)); return true;
        default: return false;
    }
}

const char *WSMPktDescriptor::getFieldStructName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldStructNames[] = {
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<5) ? fieldStructNames[field] : NULL;
}

void *WSMPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    WSMPkt *pp = (WSMPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


