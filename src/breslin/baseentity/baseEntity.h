#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "../letter/letter.h"

class BaseEntity
{
public:

BaseEntity(int id);
~BaseEntity();

int mID;

static int mNextValidID;

virtual void update()=0;

virtual bool handleLetter(Letter* letter)=0;

void setID(int id);
int  getID()const { return mID; }

static int   getNextValidID(){ return mNextValidID; }

};
#endif

