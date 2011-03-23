//
// Generated file, do not edit! Created by opp_msgc 4.1 from CCWSApplPkt.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#include <iostream>
#include <sstream>
#include "CCWSApplPkt_m.h"

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




Register_Class(CCWSAppPkt);

CCWSAppPkt::CCWSAppPkt(const char *name, int kind) : cPacket(name,kind)
{
    this->id_var = 0;
    this->x_var = 0;
    this->y_var = 0;
    this->speed_var = 0;
    this->angle_var = 0;
    this->accel_var = 0;
    this->width_var = 0;
    this->length_var = 0;
    this->utc_var = 0;
}

CCWSAppPkt::CCWSAppPkt(const CCWSAppPkt& other) : cPacket()
{
    setName(other.getName());
    operator=(other);
}

CCWSAppPkt::~CCWSAppPkt()
{
}

CCWSAppPkt& CCWSAppPkt::operator=(const CCWSAppPkt& other)
{
    if (this==&other) return *this;
    cPacket::operator=(other);
    this->id_var = other.id_var;
    this->x_var = other.x_var;
    this->y_var = other.y_var;
    this->speed_var = other.speed_var;
    this->angle_var = other.angle_var;
    this->accel_var = other.accel_var;
    this->width_var = other.width_var;
    this->length_var = other.length_var;
    this->utc_var = other.utc_var;
    return *this;
}

void CCWSAppPkt::parsimPack(cCommBuffer *b)
{
    cPacket::parsimPack(b);
    doPacking(b,this->id_var);
    doPacking(b,this->x_var);
    doPacking(b,this->y_var);
    doPacking(b,this->speed_var);
    doPacking(b,this->angle_var);
    doPacking(b,this->accel_var);
    doPacking(b,this->width_var);
    doPacking(b,this->length_var);
    doPacking(b,this->utc_var);
}

void CCWSAppPkt::parsimUnpack(cCommBuffer *b)
{
    cPacket::parsimUnpack(b);
    doUnpacking(b,this->id_var);
    doUnpacking(b,this->x_var);
    doUnpacking(b,this->y_var);
    doUnpacking(b,this->speed_var);
    doUnpacking(b,this->angle_var);
    doUnpacking(b,this->accel_var);
    doUnpacking(b,this->width_var);
    doUnpacking(b,this->length_var);
    doUnpacking(b,this->utc_var);
}

int CCWSAppPkt::getId() const
{
    return id_var;
}

void CCWSAppPkt::setId(int id_var)
{
    this->id_var = id_var;
}

double CCWSAppPkt::getX() const
{
    return x_var;
}

void CCWSAppPkt::setX(double x_var)
{
    this->x_var = x_var;
}

double CCWSAppPkt::getY() const
{
    return y_var;
}

void CCWSAppPkt::setY(double y_var)
{
    this->y_var = y_var;
}

double CCWSAppPkt::getSpeed() const
{
    return speed_var;
}

void CCWSAppPkt::setSpeed(double speed_var)
{
    this->speed_var = speed_var;
}

double CCWSAppPkt::getAngle() const
{
    return angle_var;
}

void CCWSAppPkt::setAngle(double angle_var)
{
    this->angle_var = angle_var;
}

double CCWSAppPkt::getAccel() const
{
    return accel_var;
}

void CCWSAppPkt::setAccel(double accel_var)
{
    this->accel_var = accel_var;
}

double CCWSAppPkt::getWidth() const
{
    return width_var;
}

void CCWSAppPkt::setWidth(double width_var)
{
    this->width_var = width_var;
}

double CCWSAppPkt::getLength() const
{
    return length_var;
}

void CCWSAppPkt::setLength(double length_var)
{
    this->length_var = length_var;
}

double CCWSAppPkt::getUtc() const
{
    return utc_var;
}

void CCWSAppPkt::setUtc(double utc_var)
{
    this->utc_var = utc_var;
}

class CCWSAppPktDescriptor : public cClassDescriptor
{
  public:
    CCWSAppPktDescriptor();
    virtual ~CCWSAppPktDescriptor();

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

Register_ClassDescriptor(CCWSAppPktDescriptor);

CCWSAppPktDescriptor::CCWSAppPktDescriptor() : cClassDescriptor("CCWSAppPkt", "cPacket")
{
}

CCWSAppPktDescriptor::~CCWSAppPktDescriptor()
{
}

bool CCWSAppPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CCWSAppPkt *>(obj)!=NULL;
}

const char *CCWSAppPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CCWSAppPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 9+basedesc->getFieldCount(object) : 9;
}

unsigned int CCWSAppPktDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<9) ? fieldTypeFlags[field] : 0;
}

const char *CCWSAppPktDescriptor::getFieldName(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldName(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldNames[] = {
        "id",
        "x",
        "y",
        "speed",
        "angle",
        "accel",
        "width",
        "length",
        "utc",
    };
    return (field>=0 && field<9) ? fieldNames[field] : NULL;
}

int CCWSAppPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+1;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+3;
    if (fieldName[0]=='a' && strcmp(fieldName, "angle")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "accel")==0) return base+5;
    if (fieldName[0]=='w' && strcmp(fieldName, "width")==0) return base+6;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+7;
    if (fieldName[0]=='u' && strcmp(fieldName, "utc")==0) return base+8;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CCWSAppPktDescriptor::getFieldTypeString(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldTypeString(object, field);
        field -= basedesc->getFieldCount(object);
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
        "double",
    };
    return (field>=0 && field<9) ? fieldTypeStrings[field] : NULL;
}

const char *CCWSAppPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CCWSAppPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CCWSAppPkt *pp = (CCWSAppPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CCWSAppPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CCWSAppPkt *pp = (CCWSAppPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getId());
        case 1: return double2string(pp->getX());
        case 2: return double2string(pp->getY());
        case 3: return double2string(pp->getSpeed());
        case 4: return double2string(pp->getAngle());
        case 5: return double2string(pp->getAccel());
        case 6: return double2string(pp->getWidth());
        case 7: return double2string(pp->getLength());
        case 8: return double2string(pp->getUtc());
        default: return "";
    }
}

bool CCWSAppPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CCWSAppPkt *pp = (CCWSAppPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setId(string2long(value)); return true;
        case 1: pp->setX(string2double(value)); return true;
        case 2: pp->setY(string2double(value)); return true;
        case 3: pp->setSpeed(string2double(value)); return true;
        case 4: pp->setAngle(string2double(value)); return true;
        case 5: pp->setAccel(string2double(value)); return true;
        case 6: pp->setWidth(string2double(value)); return true;
        case 7: pp->setLength(string2double(value)); return true;
        case 8: pp->setUtc(string2double(value)); return true;
        default: return false;
    }
}

const char *CCWSAppPktDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<9) ? fieldStructNames[field] : NULL;
}

void *CCWSAppPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CCWSAppPkt *pp = (CCWSAppPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


