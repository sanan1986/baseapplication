#ifndef SHAPE_H
#define SHAPE_H

//parents
#include "../../serverside/rotation/rotation.h"
#include "../../serverside/move/move.h"
#include "../../serverside/ai/ai.h"

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

#include <string>

//key defines prob should be changed to a variable if possible
#define KEY_UP					1
#define KEY_DOWN				2
#define KEY_LEFT				4
#define KEY_RIGHT				8
#define KEY_COUNTER_CLOCKWISE      16
#define KEY_CLOCKWISE              32

class Client;

class Shape
	:
	public Move ,
	public Rotation ,
	public AI
{

public:
Shape(unsigned int index, Game* game, Client* client, Vector3D* position, Vector3D* velocity, Vector3D* rotation, Ogre::Root* root,
	  bool animated, bool collidable, float collisionRadius, int meshCode, bool ai);
~Shape();

void remove();

//ticks
void processTick();

void setKeyDirection();

void write(Client* client);

//associated client if any
Client* mClient;

//mesh
int mMeshCode;

//collision
float mCollisionRadius;
bool  mCollidable;

//animation
bool mAnimated;

//ai
bool mAI;



};

#endif