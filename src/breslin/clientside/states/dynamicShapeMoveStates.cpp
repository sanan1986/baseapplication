#include "shapeDynamicMoveStates.h"
#include "shapeDynamicStateMachine.h"

#include "../tdreamsock/dreamSockLog.h"

#include "../shape/shapeDynamic.h"

//utility
#include <math.h>



/******************** Global_ProcessTick_Move *****************/

Global_ProcessTick_Move* Global_ProcessTick_Move::Instance()
{
  static Global_ProcessTick_Move instance;
  return &instance;
}
void Global_ProcessTick_Move::enter(DynamicShape* shapeDynamic)
{

}
void Global_ProcessTick_Move::execute(DynamicShape* shapeDynamic)
{
	shapeDynamic->appendToTitle(shapeDynamic->mIndex);
	
	shapeDynamic->calculateDeltaPosition();
}
void Global_ProcessTick_Move::exit(DynamicShape* shapeDynamic)
{
}

/******************** Normal_ProcessTick_Move *****************/

Normal_ProcessTick_Move* Normal_ProcessTick_Move::Instance()
{
  static Normal_ProcessTick_Move instance;
  return &instance;
}
void Normal_ProcessTick_Move::enter(DynamicShape* shapeDynamic)
{

}
void Normal_ProcessTick_Move::execute(DynamicShape* shapeDynamic)
{
	//shapeDynamic->appendToTitle("M:Normal");

	// if distance exceeds threshold && server velocity is zero
	if(shapeDynamic->mDeltaPosition > shapeDynamic->mPosInterpLimitHigh && !shapeDynamic->mServerFrame.mVelocity.isZero())
	{
		shapeDynamic->mMoveProcessTickStateMachine->changeState(Catchup_ProcessTick_Move::Instance());
    }
    else //server stopped or we are in sync so just use server vel as is, this is meat of normal state...
    {
		Vector3D serverDest;
       // Ogre::Vector3 myDest      = Ogre::Vector3::ZERO;

		serverDest.x = shapeDynamic->mServerFrame.mVelocity.x;
	    serverDest.y = shapeDynamic->mServerFrame.mVelocity.y;
        serverDest.z = shapeDynamic->mServerFrame.mVelocity.z;
        serverDest.normalise();

       // shapeDynamic->mRunSpeed = 0.0;

        if(shapeDynamic->mCommandToRunOnShape.mMilliseconds != 0)
        {
			
			shapeDynamic->mRunSpeed =
			sqrt(
			pow(shapeDynamic->mServerFrame.mVelocity.x, 2) + 
            pow(shapeDynamic->mServerFrame.mVelocity.y, 2) +
			pow(shapeDynamic->mServerFrame.mVelocity.z, 2)) /
			shapeDynamic->mCommandToRunOnShape.mMilliseconds;
        }

        serverDest = serverDest * shapeDynamic->mRunSpeed;

		shapeDynamic->mCommandToRunOnShape.mVelocity.x = serverDest.x;
        shapeDynamic->mCommandToRunOnShape.mVelocity.y = serverDest.y;
        shapeDynamic->mCommandToRunOnShape.mVelocity.z = serverDest.z;
	}
}
void Normal_ProcessTick_Move::exit(DynamicShape* shapeDynamic)
{
}

/******************** Catchup_ProcessTick_Move *****************/

Catchup_ProcessTick_Move* Catchup_ProcessTick_Move::Instance()
{
	static Catchup_ProcessTick_Move instance;
	return &instance;
}
void Catchup_ProcessTick_Move::enter(DynamicShape* shapeDynamic)
{
}
void Catchup_ProcessTick_Move::execute(DynamicShape* shapeDynamic)
{
	//shapeDynamic->appendToTitle("M:Catchup");

	//if we are back in sync
    if(shapeDynamic->mDeltaPosition <= shapeDynamic->mPosInterpLimitHigh || shapeDynamic->mServerFrame.mVelocity.isZero())
    {
		shapeDynamic->mMoveProcessTickStateMachine->changeState(Normal_ProcessTick_Move::Instance());
    }
    else
    {
		Vector3D serverDest; //vector to future server pos
        Vector3D myDest; //vector from clienr pos to future server pos

        serverDest.x = shapeDynamic->mServerFrame.mVelocity.x;
        serverDest.y = shapeDynamic->mServerFrame.mVelocity.y;
        serverDest.z = shapeDynamic->mServerFrame.mVelocity.z;
        serverDest.normalise();

        float multiplier = shapeDynamic->mDeltaPosition * shapeDynamic->mPosInterpFactor;
        serverDest = serverDest * multiplier;
        serverDest.x = shapeDynamic->mServerFrame.mOrigin.x + serverDest.x;
        serverDest.y = shapeDynamic->mServerFrame.mOrigin.y + serverDest.y;
        serverDest.z = shapeDynamic->mServerFrame.mOrigin.z + serverDest.z;
                //LogString("mOrigin.y %f", shapeDynamic->mClient->mServerFrame.mOrigin.y);

        myDest.x = serverDest.x - shapeDynamic->getPosition().x;
        myDest.y = serverDest.y - shapeDynamic->getPosition().y;
        myDest.z = serverDest.z - shapeDynamic->getPosition().z;


        //dist from clienr pos to future server pos
        float predictDist = pow(myDest.x, 2) + pow(myDest.y, 2) + pow(myDest.z, 2);
        predictDist = sqrt(predictDist);

        //server velocity
		if(shapeDynamic->mCommandToRunOnShape.mMilliseconds != 0)
        {
           shapeDynamic->mRunSpeed = sqrt(pow(shapeDynamic->mServerFrame.mVelocity.x, 2) + 
           pow(shapeDynamic->mServerFrame.mVelocity.y, 2) + pow(shapeDynamic->mServerFrame.mVelocity.z, 2))/shapeDynamic->mCommandToRunOnShape.mMilliseconds;
		}

		if(shapeDynamic->mRunSpeed != 0.0)
		{
           //time needed to get to future server pos
           float time = shapeDynamic->mDeltaPosition * shapeDynamic->mPosInterpFactor/shapeDynamic->mRunSpeed;

           myDest.normalise();

           //client vel needed to get to future server pos in time
		   float distTime = predictDist/time;
           myDest = myDest * distTime;

           shapeDynamic->mCommandToRunOnShape.mVelocity.x = myDest.x;
           shapeDynamic->mCommandToRunOnShape.mVelocity.y = myDest.y;
           shapeDynamic->mCommandToRunOnShape.mVelocity.z = myDest.z;

		}
		else
		{
			//why would catchup ever need to set velocity to zero, wouldn't we simply leave catchup state??
           shapeDynamic->mCommandToRunOnShape.mVelocity.x = 0.0;
           shapeDynamic->mCommandToRunOnShape.mVelocity.y = 0.0;
           shapeDynamic->mCommandToRunOnShape.mVelocity.z = 0.0;

		}
	}
}
void Catchup_ProcessTick_Move::exit(DynamicShape* shapeDynamic)
{
}


/******************** Global_InterpolateTick_Move *****************/

Global_InterpolateTick_Move* Global_InterpolateTick_Move::Instance()
{
  static Global_InterpolateTick_Move instance;
  return &instance;
}
void Global_InterpolateTick_Move::enter(DynamicShape* shapeDynamic)
{

}
void Global_InterpolateTick_Move::execute(DynamicShape* shapeDynamic)
{

}
void Global_InterpolateTick_Move::exit(DynamicShape* shapeDynamic)
{
}

/******************** Normal_InterpolateTick_Move *****************/

Normal_InterpolateTick_Move* Normal_InterpolateTick_Move::Instance()
{
  static Normal_InterpolateTick_Move instance;
  return &instance;
}
void Normal_InterpolateTick_Move::enter(DynamicShape* shapeDynamic)
{

}
void Normal_InterpolateTick_Move::execute(DynamicShape* shapeDynamic)
{
	Vector3D transVector;

    transVector.x = shapeDynamic->mCommandToRunOnShape.mVelocity.x;
    transVector.y = shapeDynamic->mCommandToRunOnShape.mVelocity.y;
    transVector.z = shapeDynamic->mCommandToRunOnShape.mVelocity.z;
        
    shapeDynamic->translate(transVector * shapeDynamic->mRenderTime * 1000, 1);

	//does this just prevent you from going below 0 up and down?
    if(shapeDynamic->getPosition().y < 0.0)
	{	
		shapeDynamic->setPosition(shapeDynamic->getPosition().x, 0.0 ,shapeDynamic->getPosition().z);
	}
}
void Normal_InterpolateTick_Move::exit(DynamicShape* shapeDynamic)
{
}

/******************** Off_InterpolateTick_Move *****************/

Off_InterpolateTick_Move* Off_InterpolateTick_Move::Instance()
{
	static Off_InterpolateTick_Move instance;
	return &instance;
}
void Off_InterpolateTick_Move::enter(DynamicShape* shapeDynamic)
{
}
void Off_InterpolateTick_Move::execute(DynamicShape* shapeDynamic)
{

}
void Off_InterpolateTick_Move::exit(DynamicShape* shapeDynamic)
{
}

