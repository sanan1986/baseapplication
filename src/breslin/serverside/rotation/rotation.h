#ifndef ROTATION_H
#define ROTATION_H

#include "../../baseentity/baseEntity.h"

#include "../../fsm/stateMachine.h"

//vector3D
#include "../../math/vector3D.h"

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

class Shape;

class Rotation : public BaseEntity 
{

public:

Rotation(Shape* shape);
~Rotation();

Vector3D* mRotation;
Vector3D* mRotationLast;

Shape* mShape;

float mDegrees;
float mRotationSpeed;

//acceleration
float mRotationDecel;
float mRotationAccel;

//update
virtual void update();

//handle message
virtual bool  handleMessage(const Telegram& msg);

StateMachine<Rotation>* mStateMachine;


};

#endif

