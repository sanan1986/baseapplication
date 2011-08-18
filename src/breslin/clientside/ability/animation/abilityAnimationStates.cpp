#include "abilityAnimationStates.h"
#include "../../states/stateMachineShapeDynamic.h"

#include "../../shape/shapeDynamic.h"

#include "../../billboard/objectTitle.h"
#include "abilityAnimation.h"

/******************** Global_InterpolateTick_Animation *****************/
Global_InterpolateTick_Animation* Global_InterpolateTick_Animation::Instance()
{
  static Global_InterpolateTick_Animation instance;
  return &instance;
}
void Global_InterpolateTick_Animation::enter(ShapeDynamic* shapeDynamic)
{
}
void Global_InterpolateTick_Animation::execute(ShapeDynamic* shapeDynamic)
{

}
void Global_InterpolateTick_Animation::exit(ShapeDynamic* shapeDynamic)
{
}

/******************** Idle_InterpolateTick_Animation *****************/

Idle_InterpolateTick_Animation* Idle_InterpolateTick_Animation::Instance()
{
  static Idle_InterpolateTick_Animation instance;
  return &instance;
}
void Idle_InterpolateTick_Animation::enter(ShapeDynamic* shapeDynamic)
{
	shapeDynamic->mAbilityAnimation->enterAnimationState(this);
}

void Idle_InterpolateTick_Animation::execute(ShapeDynamic* shapeDynamic)
{
	if (!shapeDynamic->mCommandToRunOnShape.mVelocity.isZero())
	{
		shapeDynamic->mAbilityAnimation->mAnimationInterpolateTickStateMachine->changeState(Run_InterpolateTick_Animation::Instance());
	}

	shapeDynamic->mAbilityAnimation->runAnimations();
}
void Idle_InterpolateTick_Animation::exit(ShapeDynamic* shapeDynamic)
{
}

/******************** Run_InterpolateTick_Animation *****************/

Run_InterpolateTick_Animation* Run_InterpolateTick_Animation::Instance()
{
  static Run_InterpolateTick_Animation instance;
  return &instance;
}
void Run_InterpolateTick_Animation::enter(ShapeDynamic* shapeDynamic)
{
	shapeDynamic->mAbilityAnimation->enterAnimationState(this);
}
void Run_InterpolateTick_Animation::execute(ShapeDynamic* shapeDynamic)
{
	if (shapeDynamic->mCommandToRunOnShape.mVelocity.isZero())
	{
		shapeDynamic->mAbilityAnimation->mAnimationInterpolateTickStateMachine->changeState(Idle_InterpolateTick_Animation::Instance());
	}
	
	shapeDynamic->mAbilityAnimation->runAnimations();
}

void Run_InterpolateTick_Animation::exit(ShapeDynamic* shapeDynamic)
{
}


