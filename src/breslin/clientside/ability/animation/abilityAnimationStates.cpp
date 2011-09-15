#include "abilityAnimationStates.h"
#include "abilityAnimationStateMachine.h"

#include "../../shape/shapeDynamic.h"

#include "../../billboard/objectTitle.h"
#include "abilityAnimation.h"


/******************************************************
*				INTERPOLATE
*
*				   STATES
*
********************************************************/


/******************************************************
*				IDLE
********************************************************/

Idle_InterpolateTick_Animation* Idle_InterpolateTick_Animation::Instance()
{
  static Idle_InterpolateTick_Animation instance;
  return &instance;
}
void Idle_InterpolateTick_Animation::enter(AbilityAnimation* abilityAnimation)
{
	abilityAnimation->enterAnimationState(this);
}

void Idle_InterpolateTick_Animation::execute(AbilityAnimation* abilityAnimation)
{
	if (!abilityAnimation->mShapeDynamic->mCommandToRunOnShape->mMoveVelocity->isZero())
	{
		abilityAnimation->mAnimationInterpolateTickStateMachine->changeState(Run_InterpolateTick_Animation::Instance());
	}

	abilityAnimation->runAnimations();
}
void Idle_InterpolateTick_Animation::exit(AbilityAnimation* abilityAnimation)
{
}

/******************************************************
*				RUN
********************************************************/

Run_InterpolateTick_Animation* Run_InterpolateTick_Animation::Instance()
{
  static Run_InterpolateTick_Animation instance;
  return &instance;
}
void Run_InterpolateTick_Animation::enter(AbilityAnimation* abilityAnimation)
{
	abilityAnimation->enterAnimationState(this);
}
void Run_InterpolateTick_Animation::execute(AbilityAnimation* abilityAnimation)
{
	if (abilityAnimation->mShapeDynamic->mCommandToRunOnShape->mMoveVelocity->isZero())
	{
		abilityAnimation->mAnimationInterpolateTickStateMachine->changeState(Idle_InterpolateTick_Animation::Instance());
	}
	
	abilityAnimation->runAnimations();
}

void Run_InterpolateTick_Animation::exit(AbilityAnimation* abilityAnimation)
{
}


