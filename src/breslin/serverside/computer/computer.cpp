#include "ai.h"
#include "../../tdreamsock/dreamSockLog.h"

#include "../../client/client.h"

#include <string>

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

//ai states
#include "states/aiStateMachine.h"
#include "states/aiStates.h"

AI::AI(Shape* shape) : Ability(shape)
{
	mShape = shape;

 	//ai states
	mAIStateMachine = new AIStateMachine(this);    //setup the state machine
	mAIStateMachine->setCurrentState      (Random_AI::Instance());
	mAIStateMachine->setPreviousState     (Random_AI::Instance());
	mAIStateMachine->setGlobalState       (NULL);

	mCounter   = 0;
        mThreshold = 1000;
}

AI::~AI()
{
}
void AI::processTick()
{
	mAIStateMachine->update();
}

