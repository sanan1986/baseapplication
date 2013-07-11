#ifndef SHAPE_H
#define SHAPE_H

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

//message
#include "../message/message.h"

#include <string>

//forward declarations
class Game;
class ClientStable;
class Rotation;
class Move;
class Computer;
class Seek;
class Vector3D;
class BaseEntity;
class Letter;

class Shape
{

public:
Shape(unsigned int index, Game* game, ClientStable* client, Vector3D* position, Vector3D* velocity, Vector3D* rotation, Ogre::Root* root,
	  bool animated, bool collidable, float collisionRadius, int meshCode, bool ai);
~Shape();


// ByteBufferes
static const char mCommandFrameTime = 2;

static const char mMessageRemoveShape = -104;
static const char mMessageSetText     = -66;

static const char mMessageNonDeltaFrame = 2;
static const char mMessageServerExit = 3;
static const char mMessageKeepAlive = 12;

//network flags
static const char mCommandOriginX      = 1;
static const char mCommandOriginY      = 2;
static const char mCommandOriginZ      = 4;
static const char mCommandRotationX    = 8;
static const char mCommandRotationZ    = 16;


//ogre scene stuff
SceneNode*          mSceneNode;

int			mIndex;

//message
Message	mMessage; //is this all i need to get information over the internets?

//game
Game* mGame;

//abilitys
std::vector<BaseEntity*> mAbilityVector;	 //all abilitys for this shape
std::vector<BaseEntity*> mSteeringAbilityVector;	 //all steering abilitys for this shape

Rotation* mRotation;
Move* 	  mMove;
Seek*     mSeek;
Computer* mComputer;

//associated client if any
ClientStable* mClient;

//mesh
int mMeshCode;

//collision
float mCollisionRadius;
float mCollisionRadiusSpawn;
bool  mCollidable;

//animation
bool mAnimated;

//ai
bool mIsComputer;

/******************************************************
*				METHODS
********************************************************/

void remove();

//abilitys
void     addAbility(BaseEntity* ability);
void     addSteeringAbility(BaseEntity* ability);

//create
void createShape(Ogre::Root* root, Vector3D* position);

//ticks
virtual void processTick();

void setValues();

void setText(std::string text);
void sendText();
std::string mText;
std::string mTextLast;

void insertIntoDB();

//collision
virtual void collision(Shape* shape);

	//delta move command
virtual int  setFlag();
virtual void addToMoveMessage(Message* message);
virtual bool handleLetter(Letter* letter) { return false; }
};

#endif
