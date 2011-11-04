#include "gameTag.h"

//log
#include "../tdreamsock/dreamSockLog.h"

//parser
#include "../parser/parser.h"

//server
#include "../server/serverTag.h"

//shape
#include "../shape/shapeTagPlayer.h"

//client
#include "../client/client.h"

//abilitys
#include "../ability/rotation/rotation.h"
#include "../ability/move/move.h"
#include "../ability/ai/ai.h"

//math
#include "../../math/vector3D.h"

GameTag::GameTag()
{
	LogString("Hosting GameTag");
	
}

GameTag::~GameTag()
{
	
}

void GameTag::createServer()
{
	LogString("createServer in GameTag");
	mServer = new ServerTag(this,"", 30004);
}

void GameTag::createWorld()
{
	LogString("createWorld in GameTag");
	//ai guys, let's make them sinbads
	for(int i = 0; i < 1; i++)
	{                 
		Vector3D* position = new Vector3D();
		position->x = 5.0f;
		position->y = 0.0f;
		position->z = 2.0f * i;

		Shape* shape = new ShapeTagPlayer(getOpenIndex(),this,0,new Vector3D(),new Vector3D(),new Vector3D(),
			mRoot,true,true,.66f,1,true); 

		mItShape = mShapeVector.at(0);
	}
}
void GameTag::collision(Shape* shape1, Shape* shape2)
{
	Game::collision(shape1,shape2);
	
	//let's make someone it
	if (shape1 == mItShape)
	{
		mItShape = shape2;
		LogString("IT:%d",mItShape->mIndex);
	}
	else if (shape2 == mItShape)
	{
		mItShape = shape1;
		LogString("IT:%d",mItShape->mIndex);
	}
}

void GameTag::buildDeltaMoveCommand(Message* mes, Shape* shape)
{
	Command* command = shape->mCommand;

	int flags = setFlag(command,shape);
	buildDeltaMoveMessage(command,flags,mes,shape);
}


int GameTag::setFlag(Command* command, Shape* shape)
{

	int flags = 0;

	//Origin
	if(shape->mPosition->x != command->mPosition->x)
	{
		flags |= mParser->mCommandOriginX;
	}
	if(shape->mPosition->y != command->mPosition->y)
	{
		flags |= mParser->mCommandOriginY;
	}
	if(shape->mPosition->z != command->mPosition->z)
	{
		flags |= mParser->mCommandOriginZ;
	}

	//Rotation
	if(shape->mRotation->x != command->mRotation->x)
	{
		flags |= mParser->mCommandRotationX;
	}
	if(shape->mRotation->z != command->mRotation->z)
	{
		flags |= mParser->mCommandRotationZ;
	}
	
	//Milliseconds
	if(shape->mMillisecondsTotal != command->mMillisecondsTotal)
	{
		flags |= mParser->mCommandMilliseconds;
	}

	return flags;
}

void GameTag::buildDeltaMoveMessage(Command* command, int flags, Message* message, Shape* shape)
{
	Game::buildDeltaMoveMessage(command,flags,message,shape);
}

void GameTag::storeCommands(Shape* shape)
{
	Game::storeCommands(shape);
}