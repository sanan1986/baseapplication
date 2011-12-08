//header
#include "applicationMain.h"


//log
#include "../../tdreamsock/dreamSockLog.h"

//application
#include "../applicationBreslin.h"

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
	LogString("enter app main");
	mApplicationBreslin->loadJoinScreen();
}
void ApplicationMain::execute()
{

}
void ApplicationMain::exit()
{
	//mApplicationBre
}
