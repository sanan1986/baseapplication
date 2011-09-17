#ifndef ROTATION_H
#define ROTATION_H

//parents
#include "../../serverside/shape/ogreShape.h"

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

class Command;
class RotationStateMachine;
class Shape;

class Rotation :  public virtual OgreShape
{

public:

Rotation();
~Rotation();

float mRotationSpeed;

//acceleration
float mRotationDecel;
float mRotationAccel;

//movement
void processTick    ();

RotationStateMachine* mRotationStateMachine;


};

#endif

