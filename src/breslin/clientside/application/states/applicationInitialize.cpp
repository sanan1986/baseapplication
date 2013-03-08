//header
#include "applicationInitialize.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//state machine
#include "../../../statemachine/stateMachine.h"

//application
#include "../applicationBreslin.h"

/***************************************
*	CONSTRUCTORS		          
***************************************/
ApplicationInitialize::ApplicationInitialize(ApplicationBreslin* applicationBreslin)
{
	mApplicationBreslin = applicationBreslin;
}

ApplicationInitialize::~ApplicationInitialize()
{

}

void ApplicationInitialize::enter()
{
	LogString("STATE: ApplicationInitialize");
}

void ApplicationInitialize::execute()
{
	//setup calls ogre specific graphics setup,
	//when it returns true we can begin our graphics stuff
	if (mApplicationBreslin->setup())
	{
		mApplicationBreslin->mSetup = true;
		mApplicationBreslin->createLoginScreen();
        	mApplicationBreslin->mStateMachine->changeState(mApplicationBreslin->mApplicationLogin);
	}	
}

void ApplicationInitialize::exit()
{

}
