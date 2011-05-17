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




Register_Class(CCWSApplPkt);

CCWSApplPkt::CCWSApplPkt(const char *name, int kind) : ApplPkt(name,kind)
{
    this->id_var = 0;
    this->x_var = 0;
    this->y_var = 0;
    this->speed_var = 0;
    this->angleX_var = 0;
    this->angleY_var = 0;
    this->accel_var = 0;
    this->width_var = 0;
    this->length_var = 0;
    this->utc_var = 0;
    this->id1_var = 0;
    this->id2_var = 0;
    this->id3_var = 0;
    this->id4_var = 0;
}

CCWSApplPkt::CCWSApplPkt(const CCWSApplPkt& other) : ApplPkt()
{
    setName(other.getName());
    operator=(other);
}

CCWSApplPkt::~CCWSApplPkt()
{
}

CCWSApplPkt& CCWSApplPkt::operator=(const CCWSApplPkt& other)
{
    if (this==&other) return *this;
    ApplPkt::operator=(other);
    this->id_var = other.id_var;
    this->x_var = other.x_var;
    this->y_var = other.y_var;
    this->speed_var = other.speed_var;
    this->angleX_var = other.angleX_var;
    this->angleY_var = other.angleY_var;
    this->accel_var = other.accel_var;
    this->width_var = other.width_var;
    this->length_var = other.length_var;
    this->utc_var = other.utc_var;
    this->id1_var = other.id1_var;
    this->id2_var = other.id2_var;
    this->id3_var = other.id3_var;
    this->id4_var = other.id4_var;
    return *this;
}

void CCWSApplPkt::parsimPack(cCommBuffer *b)
{
    ApplPkt::parsimPack(b);
    doPacking(b,this->id_var);
    doPacking(b,this->x_var);
    doPacking(b,this->y_var);
    doPacking(b,this->speed_var);
    doPacking(b,this->angleX_var);
    doPacking(b,this->angleY_var);
    doPacking(b,this->accel_var);
    doPacking(b,this->width_var);
    doPacking(b,this->length_var);
    doPacking(b,this->utc_var);
    doPacking(b,this->id1_var);
    doPacking(b,this->id2_var);
    doPacking(b,this->id3_var);
    doPacking(b,this->id4_var);
}

void CCWSApplPkt::parsimUnpack(cCommBuffer *b)
{
    ApplPkt::parsimUnpack(b);
    doUnpacking(b,this->id_var);
    doUnpacking(b,this->x_var);
    doUnpacking(b,this->y_var);
    doUnpacking(b,this->speed_var);
    doUnpacking(b,this->angleX_var);
    doUnpacking(b,this->angleY_var);
    doUnpacking(b,this->accel_var);
    doUnpacking(b,this->width_var);
    doUnpacking(b,this->length_var);
    doUnpacking(b,this->utc_var);
    doUnpacking(b,this->id1_var);
    doUnpacking(b,this->id2_var);
    doUnpacking(b,this->id3_var);
    doUnpacking(b,this->id4_var);
}

int CCWSApplPkt::getId() const
{
    return id_var;
}

void CCWSApplPkt::setId(int id_var)
{
    this->id_var = id_var;
}

double CCWSApplPkt::getX() const
{
    return x_var;
}

void CCWSApplPkt::setX(double x_var)
{
    this->x_var = x_var;
}

double CCWSApplPkt::getY() const
{
    return y_var;
}

void CCWSApplPkt::setY(double y_var)
{
    this->y_var = y_var;
}

double CCWSApplPkt::getSpeed() const
{
    return speed_var;
}

void CCWSApplPkt::setSpeed(double speed_var)
{
    this->speed_var = speed_var;
}

double CCWSApplPkt::getAngleX() const
{
    return angleX_var;
}

void CCWSApplPkt::setAngleX(double angleX_var)
{
    this->angleX_var = angleX_var;
}

double CCWSApplPkt::getAngleY() const
{
    return angleY_var;
}

void CCWSApplPkt::setAngleY(double angleY_var)
{
    this->angleY_var = angleY_var;
}

double CCWSApplPkt::getAccel() const
{
    return accel_var;
}

void CCWSApplPkt::setAccel(double accel_var)
{
    this->accel_var = accel_var;
}

double CCWSApplPkt::getWidth() const
{
    return width_var;
}

void CCWSApplPkt::setWidth(double width_var)
{
    this->width_var = width_var;
}

double CCWSApplPkt::getLength() const
{
    return length_var;
}

void CCWSApplPkt::setLength(double length_var)
{
    this->length_var = length_var;
}

simtime_t CCWSApplPkt::getUtc() const
{
    return utc_var;
}

void CCWSApplPkt::setUtc(simtime_t utc_var)
{
    this->utc_var = utc_var;
}

int CCWSApplPkt::getId1() const
{
    return id1_var;
}

void CCWSApplPkt::setId1(int id1_var)
{
    this->id1_var = id1_var;
}

int CCWSApplPkt::getId2() const
{
    return id2_var;
}

void CCWSApplPkt::setId2(int id2_var)
{
    this->id2_var = id2_var;
}

int CCWSApplPkt::getId3() const
{
    return id3_var;
}

void CCWSApplPkt::setId3(int id3_var)
{
    this->id3_var = id3_var;
}

int CCWSApplPkt::getId4() const
{
    return id4_var;
}

void CCWSApplPkt::setId4(int id4_var)
{
    this->id4_var = id4_var;
}

class CCWSApplPktDescriptor : public cClassDescriptor
{
  public:
    CCWSApplPktDescriptor();
    virtual ~CCWSApplPktDescriptor();

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

Register_ClassDescriptor(CCWSApplPktDescriptor);

CCWSApplPktDescriptor::CCWSApplPktDescriptor() : cClassDescriptor("CCWSApplPkt", "ApplPkt")
{
}

CCWSApplPktDescriptor::~CCWSApplPktDescriptor()
{
}

bool CCWSApplPktDescriptor::doesSupport(cObject *obj) const
{
    return dynamic_cast<CCWSApplPkt *>(obj)!=NULL;
}

const char *CCWSApplPktDescriptor::getProperty(const char *propertyname) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : NULL;
}

int CCWSApplPktDescriptor::getFieldCount(void *object) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 14+basedesc->getFieldCount(object) : 14;
}

unsigned int CCWSApplPktDescriptor::getFieldTypeFlags(void *object, int field) const
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
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<14) ? fieldTypeFlags[field] : 0;
}

const char *CCWSApplPktDescriptor::getFieldName(void *object, int field) const
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
        "angleX",
        "angleY",
        "accel",
        "width",
        "length",
        "utc",
        "id1",
        "id2",
        "id3",
        "id4",
    };
    return (field>=0 && field<14) ? fieldNames[field] : NULL;
}

int CCWSApplPktDescriptor::findField(void *object, const char *fieldName) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount(object) : 0;
    if (fieldName[0]=='i' && strcmp(fieldName, "id")==0) return base+0;
    if (fieldName[0]=='x' && strcmp(fieldName, "x")==0) return base+1;
    if (fieldName[0]=='y' && strcmp(fieldName, "y")==0) return base+2;
    if (fieldName[0]=='s' && strcmp(fieldName, "speed")==0) return base+3;
    if (fieldName[0]=='a' && strcmp(fieldName, "angleX")==0) return base+4;
    if (fieldName[0]=='a' && strcmp(fieldName, "angleY")==0) return base+5;
    if (fieldName[0]=='a' && strcmp(fieldName, "accel")==0) return base+6;
    if (fieldName[0]=='w' && strcmp(fieldName, "width")==0) return base+7;
    if (fieldName[0]=='l' && strcmp(fieldName, "length")==0) return base+8;
    if (fieldName[0]=='u' && strcmp(fieldName, "utc")==0) return base+9;
    if (fieldName[0]=='i' && strcmp(fieldName, "id1")==0) return base+10;
    if (fieldName[0]=='i' && strcmp(fieldName, "id2")==0) return base+11;
    if (fieldName[0]=='i' && strcmp(fieldName, "id3")==0) return base+12;
    if (fieldName[0]=='i' && strcmp(fieldName, "id4")==0) return base+13;
    return basedesc ? basedesc->findField(object, fieldName) : -1;
}

const char *CCWSApplPktDescriptor::getFieldTypeString(void *object, int field) const
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
        "simtime_t",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<14) ? fieldTypeStrings[field] : NULL;
}

const char *CCWSApplPktDescriptor::getFieldProperty(void *object, int field, const char *propertyname) const
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

int CCWSApplPktDescriptor::getArraySize(void *object, int field) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getArraySize(object, field);
        field -= basedesc->getFieldCount(object);
    }
    CCWSApplPkt *pp = (CCWSApplPkt *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

std::string CCWSApplPktDescriptor::getFieldAsString(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldAsString(object,field,i);
        field -= basedesc->getFieldCount(object);
    }
    CCWSApplPkt *pp = (CCWSApplPkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getId());
        case 1: return double2string(pp->getX());
        case 2: return double2string(pp->getY());
        case 3: return double2string(pp->getSpeed());
        case 4: return double2string(pp->getAngleX());
        case 5: return double2string(pp->getAngleY());
        case 6: return double2string(pp->getAccel());
        case 7: return double2string(pp->getWidth());
        case 8: return double2string(pp->getLength());
        case 9: return double2string(pp->getUtc());
        case 10: return long2string(pp->getId1());
        case 11: return long2string(pp->getId2());
        case 12: return long2string(pp->getId3());
        case 13: return long2string(pp->getId4());
        default: return "";
    }
}

bool CCWSApplPktDescriptor::setFieldAsString(void *object, int field, int i, const char *value) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->setFieldAsString(object,field,i,value);
        field -= basedesc->getFieldCount(object);
    }
    CCWSApplPkt *pp = (CCWSApplPkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setId(string2long(value)); return true;
        case 1: pp->setX(string2double(value)); return true;
        case 2: pp->setY(string2double(value)); return true;
        case 3: pp->setSpeed(string2double(value)); return true;
        case 4: pp->setAngleX(string2double(value)); return true;
        case 5: pp->setAngleY(string2double(value)); return true;
        case 6: pp->setAccel(string2double(value)); return true;
        case 7: pp->setWidth(string2double(value)); return true;
        case 8: pp->setLength(string2double(value)); return true;
        case 9: pp->setUtc(string2double(value)); return true;
        case 10: pp->setId1(string2long(value)); return true;
        case 11: pp->setId2(string2long(value)); return true;
        case 12: pp->setId3(string2long(value)); return true;
        case 13: pp->setId4(string2long(value)); return true;
        default: return false;
    }
}

const char *CCWSApplPktDescriptor::getFieldStructName(void *object, int field) const
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
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    };
    return (field>=0 && field<14) ? fieldStructNames[field] : NULL;
}

void *CCWSApplPktDescriptor::getFieldStructPointer(void *object, int field, int i) const
{
    cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount(object))
            return basedesc->getFieldStructPointer(object, field, i);
        field -= basedesc->getFieldCount(object);
    }
    CCWSApplPkt *pp = (CCWSApplPkt *)object; (void)pp;
    switch (field) {
        default: return NULL;
    }
}


