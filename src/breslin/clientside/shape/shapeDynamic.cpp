/******************************************************
*				INCLUDES
********************************************************/

//parent
#include "shapeDynamic.h"

//standard library
#include <string.h>

//log
#include "../tdreamsock/dreamSockLog.h"

//game
#include "../game/game.h"

//ability
#include "../ability/ability.h"

//dispatch
#include "../dispatch/dispatch.h"

//parser
#include "../parser/parser.h"


/******************************************************
*				CLASS
********************************************************/

ShapeDynamic::ShapeDynamic(Game* game, Dispatch* dispatch)
:
	Shape         ()
{
	//game
	mGame = game;

	//commands
	mServerFrame         = new Command();
	mCommandToRunOnShape = new Command();

	//speed
	mSpeed     = 0.0;
	mSpeedMax  = 200.0;

	//orientation
	mPosition = new Vector3D();
	mVelocity = new Vector3D();
	mRotation = new Vector3D();

	//dispatch
	parseDispatch(dispatch);

	//ghost
	mGhost = NULL;


}

ShapeDynamic::~ShapeDynamic()
{
}

float ShapeDynamic::getSpeed()
{
	return mSpeed;
}	

void ShapeDynamic::addAbility(Ability* ability)
{
	
	mAbilityVector.push_back(ability);	
}

Ability* ShapeDynamic::getAbility(Ability* ability)
{
	for (unsigned int i = 0; i < mAbilityVector.size(); i++)
	{
		//typeid(ability);

		if (typeid(ability) == typeid(mAbilityVector.at(i)))
		{
			return mAbilityVector.at(i);
		}
	}
	return 0;
}

void ShapeDynamic::parseDispatch(Dispatch* dispatch)
{
	dispatch->BeginReading();
	dispatch->ReadByte(); //should read -103 to add a shape..
	mLocal	=    dispatch->ReadByte();
	mIndex		=    dispatch->ReadByte();
	dispatch->ReadString(); //strcpy(name, dispatch->ReadString()); //name
	mPosition->x =   dispatch->ReadFloat();
	mPosition->y =   dispatch->ReadFloat();
	mPosition->z =   dispatch->ReadFloat();
	mVelocity->x = dispatch->ReadFloat();
	mVelocity->y = dispatch->ReadFloat();
	mVelocity->z = dispatch->ReadFloat();
	mRotation->x = dispatch->ReadFloat();
	mRotation->z = dispatch->ReadFloat();
}

void ShapeDynamic::processTick()
{
	clearTitle(); //empty title string so it can be filled anew

	//process ticks on abilitys
	for (unsigned int i = 0; i < mAbilityVector.size(); i++)
	{
		mAbilityVector.at(i)->processTick();
	}

	//run billboard here for now.
	//drawTitle();
}
void ShapeDynamic::interpolateTick(float renderTime)
{
	//interpolate ticks on abilitys
	for (unsigned int i = 0; i < mAbilityVector.size(); i++)
	{
		mAbilityVector.at(i)->interpolateTick(renderTime);
	}
}

//this is all shapes coming to client game from server
//should a shape be responsible to read it's own command?????
//once we determine it's about him shouldn't we pass it off to
//shape object to handle?
void ShapeDynamic::readDeltaMoveCommand(Dispatch *mes)
{
	//ShapeDynamic* shape = NULL;

	int flags = 0;

	bool x = true;
	bool z = true;
	bool y = true;

	// Flags
	flags = mes->ReadByte();

	// Origin
	if(flags & mCommandOriginX)
	{
		mServerFrame->mOriginOld->x = mServerFrame->mOrigin->x;
		mServerFrame->mOrigin->x = mes->ReadFloat();		
	}
	else
	{
		x = false;
	}

	if(flags & mCommandOriginY)
	{
		mServerFrame->mOriginOld->y = mServerFrame->mOrigin->y;
		mServerFrame->mOrigin->y = mes->ReadFloat();
	}
	else
	{
		y = false;
	}

	if(flags & mCommandOriginZ)
	{
		mServerFrame->mOriginOld->z = mServerFrame->mOrigin->z;
		mServerFrame->mOrigin->z = mes->ReadFloat();	
	}
	else
	{
		z = false;
	}

	//set old rot
	mServerFrame->mRotOld->x = mServerFrame->mRot->x;
	mServerFrame->mRotOld->z = mServerFrame->mRot->z;

	//rotation
	if(flags & mCommandRotationX)
	{
		mServerFrame->mRot->x = mes->ReadFloat();
	}

	if(flags & mCommandRotationZ)
	{
		mServerFrame->mRot->z = mes->ReadFloat();
	}

	//milliseconds
	if (flags & mParser->mCommandMilliseconds)
	{
		mServerFrame->mMilliseconds = mes->ReadByte();
		mCommandToRunOnShape->mMilliseconds = mServerFrame->mMilliseconds;
	}

	if (mServerFrame->mMilliseconds != 0) 
	{
		if(!x && !z && !y && mServerFrame->mMilliseconds != 0)
		{
			mServerFrame->mVelocity->x = 0.0;
			mServerFrame->mVelocity->y = 0.0;
			mServerFrame->mVelocity->z = 0.0;
		}
		else
		{
			mServerFrame->mVelocity->x = mServerFrame->mOrigin->x - mServerFrame->mOriginOld->x;
			mServerFrame->mVelocity->y = mServerFrame->mOrigin->y - mServerFrame->mOriginOld->y;
			mServerFrame->mVelocity->z = mServerFrame->mOrigin->z - mServerFrame->mOriginOld->z;
		}
	}
	processTick();
	moveGhostShape();
}


void ShapeDynamic::moveGhostShape()
{
	Vector3D transVector;

	transVector.x = mServerFrame->mOrigin->x;
	transVector.y = 0;
	transVector.z = mServerFrame->mOrigin->z;

	if (mGhost)
	{
		mGhost->setPosition(transVector);
	}
}
