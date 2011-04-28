//
// Generated file, do not edit! Created by opp_msgc 4.1 from CCWSApplPkt.msg.
//

#ifndef _CCWSAPPLPKT_M_H_
#define _CCWSAPPLPKT_M_H_

#include <omnetpp.h>

// opp_msgc version check
#define MSGC_VERSION 0x0401
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgc: 'make clean' should help.
#endif

// cplusplus {{
#include "ApplPkt_m.h"
// }}



/**
 * Class generated from <tt>CCWSApplPkt.msg</tt> by opp_msgc.
 * <pre>
 * packet CCWSApplPkt extends ApplPkt {
 *     int id;
 *     double x;
 *     double y;
 *     double speed;
 *     double angleX;
 *     double angleY;
 *     double accel;
 *     double width;
 *     double length;
 *     simtime_t utc;
 * }
 * </pre>
 */
class CCWSApplPkt : public ::ApplPkt
{
  protected:
    int id_var;
    double x_var;
    double y_var;
    double speed_var;
    double angleX_var;
    double angleY_var;
    double accel_var;
    double width_var;
    double length_var;
    simtime_t utc_var;

    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CCWSApplPkt&);

  public:
    CCWSApplPkt(const char *name=NULL, int kind=0);
    CCWSApplPkt(const CCWSApplPkt& other);
    virtual ~CCWSApplPkt();
    CCWSApplPkt& operator=(const CCWSApplPkt& other);
    virtual CCWSApplPkt *dup() const {return new CCWSApplPkt(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual int getId() const;
    virtual void setId(int id_var);
    virtual double getX() const;
    virtual void setX(double x_var);
    virtual double getY() const;
    virtual void setY(double y_var);
    virtual double getSpeed() const;
    virtual void setSpeed(double speed_var);
    virtual double getAngleX() const;
    virtual void setAngleX(double angleX_var);
    virtual double getAngleY() const;
    virtual void setAngleY(double angleY_var);
    virtual double getAccel() const;
    virtual void setAccel(double accel_var);
    virtual double getWidth() const;
    virtual void setWidth(double width_var);
    virtual double getLength() const;
    virtual void setLength(double length_var);
    virtual simtime_t getUtc() const;
    virtual void setUtc(simtime_t utc_var);
};

inline void doPacking(cCommBuffer *b, CCWSApplPkt& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, CCWSApplPkt& obj) {obj.parsimUnpack(b);}


#endif // _CCWSAPPLPKT_M_H_
