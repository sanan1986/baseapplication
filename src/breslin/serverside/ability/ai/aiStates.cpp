//parent
#include "aiStates.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//states
#include "aiStateMachine.h"

//ability
#include "ai.h"

//game
#include "../../game/game.h"

//shapes
#include "../../shape/shape.h"

//rand
#include <stdlib.h>
#include <time.h>

#define MAX_RUN_SPEED 1.66           // character running speed in units per second

Normal_AI* Normal_AI::Instance()
{
  static Normal_AI instance;
  return &instance;
}
void Normal_AI::enter(AI* ai)
{
}
void Normal_AI::execute(AI* ai)
{
}
void Normal_AI::exit(AI* ai)
{
}


Random_AI* Random_AI::Instance()
{
  static Random_AI instance;
  return &instance;
}

void Random_AI::enter(AI* ai)
{

}

void Random_AI::execute(AI* ai)
{

	srand ( (short)time(NULL) + ai->mShape->mIndex);	/* initialize random seed: */
	ai->mShape->mKey = rand() % 16 + 1;  //assign random key 0-16 or is it 1-16 or 0-15?

	if (ai->mShape->mKey == 16)
	{
		ai->mShape->mKey = 2;
	}
	if (ai->mShape->mKey == 0)
	{
		ai->mShape->mKey = 4;
	}
}

void Random_AI::exit(AI* ai)
{
}

No_AI* No_AI::Instance()
{
	static No_AI instance;
	return &instance;
}

void No_AI::enter(AI* ai)
{
}

void No_AI::execute(AI* ai)
{
}

void No_AI::exit(AI* ai)
{
}

Accelerate_AI* Accelerate_AI::Instance()
{
	static Accelerate_AI instance;
	return &instance;
}
void Accelerate_AI::enter(AI* ai)
{
}
void Accelerate_AI::execute(AI* ai)
{
}
void Accelerate_AI::exit(AI* ai)
{
}

Decelerate_AI* Decelerate_AI::Instance()
{
	static Decelerate_AI instance;
	return &instance;
}
void Decelerate_AI::enter(AI* ai)
{
}
void Decelerate_AI::execute(AI* ai)
{
}
void Decelerate_AI::exit(AI* ai)
{
}
