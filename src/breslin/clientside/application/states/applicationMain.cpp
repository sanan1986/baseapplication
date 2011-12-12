//header
#include "applicationMain.h"


//log
#include "../../tdreamsock/dreamSockLog.h"

//application
#include "../applicationBreslin.h"

//game
#include "../../game/game.h"
#include "../../game/gameTag.h"

//state machine
#include "../../../statemachine/stateMachine.h"

/***************************************
*	CONSTRUCTORS		          
***************************************/
ApplicationMain::ApplicationMain(ApplicationBreslin* applicationBreslin)
{
	mApplicationBreslin = applicationBreslin;
}

ApplicationMain::~ApplicationMain()
{

}

void ApplicationMain::enter()
{
	LogString("STATE: ApplicationGlobal");
	mApplicationBreslin->showMainScreen();
}

void ApplicationMain::execute()
{
	if (mApplicationBreslin->mButtonHit == mApplicationBreslin->mButtonGame)
	{
		mApplicationBreslin->mButtonHit = NULL;	
		mApplicationBreslin->sendConnect();
		mApplicationBreslin->mGame = new Game(mApplicationBreslin);
		
		mApplicationBreslin->hideMainScreen();

		mApplicationBreslin->mStateMachine->changeState(mApplicationBreslin->mApplicationPlay);			
	}

	if (mApplicationBreslin->mButtonHit == mApplicationBreslin->mButtonTag)
	{
		mApplicationBreslin->mButtonHit = NULL;	
		mApplicationBreslin->sendConnect();
		mApplicationBreslin->mGame = new GameTag(mApplicationBreslin);
		
		mApplicationBreslin->hideMainScreen();

		mApplicationBreslin->mStateMachine->changeState(mApplicationBreslin->mApplicationPlay);			
	}
	
	if (mApplicationBreslin->mButtonHit == mApplicationBreslin->mButtonExit)
	{
		mApplicationBreslin->mButtonHit = NULL;	
				
		mApplicationBreslin->shutdown();
		
		mApplicationBreslin->mShutDown = true;			
		
		delete mApplicationBreslin;
			
	}	
	
	
}
void ApplicationMain::exit()
{
}
