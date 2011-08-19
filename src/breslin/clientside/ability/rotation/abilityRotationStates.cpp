#include "abilityRotationStates.h"
#include "abilityStateMachine.h"

#include "../../shape/shapeDynamic.h"

#include "../../billboard/objectTitle.h"
#include "abilityRotation.h"


/******************************************************
*				INTERPOLATE
*
*				   STATES
*
********************************************************/


/******************************************************
*				GLOBAL
********************************************************/
Global_ProcessTick_Rotation* Global_ProcessTick_Rotation::Instance()
{
  static Global_ProcessTick_Rotation instance;
  return &instance;
}
void Global_ProcessTick_Rotation::enter(AbilityRotation* abilityRotation)
{
}
void Global_ProcessTick_Rotation::execute(AbilityRotation* abilityRotation)
{
	ability->calculateServerRotationSpeed();

	shapeDynamic->mGhost->yaw(shapeDynamic->mAbilityRotation->mServerRotSpeed,true);	

}
void Global_ProcessTick_Rotation::exit(AbilityRotation* abilityRotation)
{
}

/******************************************************
*				NORMAL
********************************************************/

Normal_ProcessTick_Rotation* Normal_ProcessTick_Rotation::Instance()
{
  static Normal_ProcessTick_Rotation instance;
  return &instance;
}
void Normal_ProcessTick_Rotation::enter(AbilityRotation* abilityRotation)
{
}
void Normal_ProcessTick_Rotation::execute(AbilityRotation* abilityRotation)
{
	//->mObjectTitleString.append("R:Normal");
	
	// are we too far off you need to change to catchup state
    if(abs(shapeDynamic->mAbilityRotation->mDegreesToServer) > shapeDynamic->mAbilityRotation->mRotInterpLimitHigh)
    {
		shapeDynamic->mAbilityRotation->mRotationProcessTickStateMachine->changeState(Catchup_ProcessTick_Rotation::Instance());
		return;
    }
    else
    {
         if (shapeDynamic->mAbilityRotation->mServerRotSpeed == 0.0)
         {
			shapeDynamic->mCommandToRunOnShape.mRotSpeed = 0.0;
         }
         else
         {
			// if server rot counter-clockwise hardcode server rot to +mTurnSpeed
            if(shapeDynamic->mAbilityRotation->mServerRotSpeed > 0.0)
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
			else //clockwise - set to -mTurnSpeed
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = -shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
		}
	}
}
void Normal_ProcessTick_Rotation::exit(AbilityRotation* abilityRotation)
{
}

/******************************************************
*				CATCHUP
********************************************************/

Catchup_ProcessTick_Rotation* Catchup_ProcessTick_Rotation::Instance()
{
  static Catchup_ProcessTick_Rotation instance;
  return &instance;
}
void Catchup_ProcessTick_Rotation::enter(AbilityRotation* abilityRotation)
{
}
void Catchup_ProcessTick_Rotation::execute(AbilityRotation* abilityRotation)
{
	//->mObjectTitleString.append("R:Catchup");

	// are we back on track
    if(abs(shapeDynamic->mAbilityRotation->mDegreesToServer) < shapeDynamic->mAbilityRotation->mRotInterpLimitLow)
    {
       shapeDynamic->mAbilityRotation->mRotationProcessTickStateMachine->changeState(Normal_ProcessTick_Rotation::Instance());
		return;
    }
    else
    {
		if(shapeDynamic->mAbilityRotation->mServerRotSpeed != 0.0)
        {
			// if server rot counter-clockwise hardcode server rot to +mTurnSpeed
            if(shapeDynamic->mAbilityRotation->mServerRotSpeed > 0.0)
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
            else //clockwise - set to -mTurnSpeed
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = -shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
			if(shapeDynamic->mAbilityRotation->mDegreesToServer / shapeDynamic->mAbilityRotation->mServerRotSpeed > 0.0)
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = shapeDynamic->mCommandToRunOnShape.mRotSpeed * shapeDynamic->mAbilityRotation->mRotInterpIncrease;
            }
            else
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = shapeDynamic->mCommandToRunOnShape.mRotSpeed * shapeDynamic->mAbilityRotation->mRotInterpDecrease;
            }
		}
        else if(shapeDynamic->mAbilityRotation->mServerRotSpeed == 0.0)
        {
			if (shapeDynamic->mAbilityRotation->mDegreesToServer > 0.0)
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
            else //clockwise - set to -mTurnSpeed
            {
				shapeDynamic->mCommandToRunOnShape.mRotSpeed = -shapeDynamic->mAbilityRotation->mTurnSpeed;
            }
		}
	}
}
void Catchup_ProcessTick_Rotation::exit(AbilityRotation* abilityRotation)
{
}

/******************************************************
*				INTERPOLATE
*
*				   STATES
*
********************************************************/


/******************************************************
*				Normal
********************************************************/
Normal_InterpolateTick_Rotation* Normal_InterpolateTick_Rotation::Instance()
{
  static Normal_InterpolateTick_Rotation instance;
  return &instance;
}
void Normal_InterpolateTick_Rotation::enter(AbilityRotation* abilityRotation)
{
}
void Normal_InterpolateTick_Rotation::execute(AbilityRotation* abilityRotation)
{

	//->mObjectTitleString.append("R:Normal");
	float rotSpeed = shapeDynamic->mCommandToRunOnShape.mRotSpeed * shapeDynamic->mRenderTime;
   shapeDynamic->yaw(rotSpeed, true);

   if (shapeDynamic->mAbilityRotation->mServerRotSpeed == 0.0 && abs(shapeDynamic->mAbilityRotation->getDegreesToServer()) < shapeDynamic->mAbilityRotation->mRotInterpLimitLow)
    {
		shapeDynamic->mAbilityRotation->mRotationInterpolateTickStateMachine->changeState(Off_InterpolateTick_Rotation::Instance());
    }

}
void Normal_InterpolateTick_Rotation::exit(AbilityRotation* abilityRotation)
{
}

/******************************************************
*				OFF
********************************************************/

Off_InterpolateTick_Rotation* Off_InterpolateTick_Rotation::Instance()
{
  static Off_InterpolateTick_Rotation instance;
  return &instance;
}
void Off_InterpolateTick_Rotation::enter(AbilityRotation* abilityRotation)
{
	shapeDynamic->mCommandToRunOnShape.mRotSpeed = 0.0;
}
void Off_InterpolateTick_Rotation::execute(AbilityRotation* abilityRotation)
{		
	//->mObjectTitleString.append("R:Off");
	if (abs(shapeDynamic->mAbilityRotation->getDegreesToServer()) > shapeDynamic->mAbilityRotation->mRotInterpLimitLow)
    {
		shapeDynamic->mAbilityRotation->mRotationInterpolateTickStateMachine->changeState(Normal_InterpolateTick_Rotation::Instance());
    }
}
void Off_InterpolateTick_Rotation::exit(AbilityRotation* abilityRotation)
{
}

