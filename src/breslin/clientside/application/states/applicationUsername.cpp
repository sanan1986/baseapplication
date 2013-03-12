//header
#include "applicationUsername.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//application
#include "../applicationBreslin.h"

//state machine
#include "../../../statemachine/stateMachine.h"

//input
#include "../../io/editString.h"

/***************************************
*	CONSTRUCTORS
***************************************/
ApplicationUsername::ApplicationUsername(ApplicationBreslin* applicationBreslin)
{
	mApplicationBreslin = applicationBreslin;
}

ApplicationUsername::~ApplicationUsername()
{
}

void ApplicationUsername::enter()
{
	LogString("STATE: ApplicationUsername");
	createUsernameScreen();
	showUsernameScreen();
}

void ApplicationUsername::execute()
{
	//check for mFinal
	if (mApplicationBreslin->mEditString->getIsFinal())
	{
		LogString("mFinal = true");
		//change state to password
			
	}

	//check for input keys ..these will go to username and password for now
 	mLabelUsernameEdit->setCaption(mApplicationBreslin->mEditString->getText());
}

void ApplicationUsername::exit()
{
 	mApplicationBreslin->mEditString->clear();
	hideUsernameScreen();
}

//USERNAME
void ApplicationUsername::createUsernameScreen()
{
	if (!mLabelUsername)
	{
        	mLabelUsername = mApplicationBreslin->getTrayManager()->createLabel(OgreBites::TL_CENTER, "mLabelUsername", "Username:");
	}
	if (!mLabelUsernameEdit)
	{
        	mLabelUsernameEdit = mApplicationBreslin->getTrayManager()->createLabel(OgreBites::TL_CENTER, "mLabelUsernameEdit", "");
	}
	if (!mApplicationBreslin->mButtonExit)
	{
        	mApplicationBreslin->mButtonExit = mApplicationBreslin->getTrayManager()->createButton(OgreBites::TL_CENTER, "mButtonExit", "Exit Application");
	}
}

void ApplicationUsername::showUsernameScreen()
{
        mApplicationBreslin->getTrayManager()->moveWidgetToTray(mLabelUsername,OgreBites::TL_CENTER);
        mApplicationBreslin->getTrayManager()->moveWidgetToTray(mLabelUsernameEdit,OgreBites::TL_CENTER);

        mApplicationBreslin->getTrayManager()->moveWidgetToTray(mApplicationBreslin->mButtonExit,OgreBites::TL_CENTER);

        mLabelUsername->show();
        mLabelUsernameEdit->show();

        mApplicationBreslin->mButtonExit->show();

        mApplicationBreslin->getTrayManager()->showCursor();
}

void ApplicationUsername::hideUsernameScreen()
{
        mLabelUsername->hide();
        mLabelUsernameEdit->hide();

        mApplicationBreslin->mButtonExit->hide();
}

