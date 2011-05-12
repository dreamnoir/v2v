#ifndef __COUTBINARYVECTOR_H
#define __COUTBINARYVECTOR_H

#include <stdio.h>
#include "cownedobject.h"
#include "simtime_t.h"

class cOutBinaryVector : public cNoncopyableOwnedObject
{
protected:



public:
	cOutBinaryVector(const char *name=NULL) : cNoncopyableOwnedObject(name) {}
	~cOutBinaryVector();

	virtual void setName(const char *name)
	{
		cOwnedObject::setName(name);
	}

	virtual bool record(double value);

};

#endif /*__COUTBINARYVECTOR_H */
