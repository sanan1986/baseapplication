//header
#include "applicationBreslin.h"

//log
#include "../tdreamsock/dreamSockLog.h"

//shape
#include "../shape/shape.h"

//math
#include "../../math/vector3D.h"

//byteBuffer
#include "../bytebuffer/byteBuffer.h"

//network
#include "../network/network.h"

//command
#include "../command/command.h"

//game
#include "../game/gameTag.h"

//state machine
#include "../../statemachine/stateMachine.h"
#include "states/applicationGlobal.h"
#include "states/applicationMain.h"
#include "states/applicationInitialize.h"
#include "states/applicationUsername.h"
#include "states/applicationPassword.h"
#include "states/applicationPlay.h"

//input
#include "../io/editString.h"


/***************************************
*	CONSTRUCTORS		          
***************************************/
ApplicationBreslin::ApplicationBreslin(const char* serverIP, int serverPort)
{
	StartLog();

	//network
	mNetwork = new Network(this,serverIP,serverPort);

	//initilize
	mSetup = false;
	mPlayingGame = false;
	mFake = true;

	//time
	mRenderTime = 0.0f;

	//game
	mGame = NULL;

	//state machine (Menus)
	mStateMachine = new StateMachine();

	mApplicationGlobal     = new ApplicationGlobal    (this);
	mApplicationInitialize = new ApplicationInitialize(this);
	mApplicationMain       = new ApplicationMain      (this);
	mApplicationPlay       = new ApplicationPlay      (this);
	mApplicationUsername   = new ApplicationUsername  (this);
	mApplicationPassword   = new ApplicationPassword  (this);

	mStateMachine->setGlobalState (mApplicationGlobal);
	mStateMachine->changeState(mApplicationInitialize);
        mStateMachine->setPreviousState(mApplicationInitialize);

	//input
	mEditString = new EditString(); 

}

ApplicationBreslin::~ApplicationBreslin()
{
	mNetwork->close();
	delete mNetwork;
	
	delete mStateMachine;
	delete mApplicationInitialize;
	delete mApplicationUsername;
	delete mApplicationPassword;
	delete mApplicationMain;
	delete mApplicationPlay;	
	
	delete mGame;
}

/*********************************
			update
**********************************/
void ApplicationBreslin::processUpdate()
{
	mStateMachine->update();

	if (mFake == true)
	{
		//after first update let's fake going to game with no sendCnnect...
		mButtonHit = NULL;
        	//	sendConnect();
       		mGame = new Game(this);

		hideUsernameScreen();

       		mStateMachine->changeState(mApplicationPlay);

		//sneak an update in
		mStateMachine->update();

		//fake esc from game
  		mPlayingGame = false;
                mStateMachine->changeState(mApplicationUsername);
		
		mFake = false;
	}
}

/*********************************
	ADMIN
**********************************/

void ApplicationBreslin::shutdown()
{
	ByteBuffer* byteBuffer = new ByteBuffer();
	byteBuffer->WriteByte(mMessageDisconnect);
	mNetwork->send(byteBuffer);
	mNetwork->reset();
}


/*********************************
		NETWORK
**********************************/

void ApplicationBreslin::sendConnect()
{
	ByteBuffer* byteBuffer = new ByteBuffer();
	byteBuffer->WriteByte(mMessageConnect);
	mNetwork->send(byteBuffer);
}

void ApplicationBreslin::sendLogin()
{
/*
	ByteBuffer* byteBuffer = new ByteBuffer();
	byteBuffer->WriteByte(mMessageConnect);
	mNetwork->send(byteBuffer);
*/
}


/*********************************

*		TIME
***********************************/
float ApplicationBreslin::getRenderTime()
{
	return mRenderTime;
}


/*********************************
		GRAPHICS
**********************************/
void ApplicationBreslin::createScene()
{

}

bool ApplicationBreslin::runGraphics()
{
	//Pump messages in all registered RenderWindow windows
	WindowEventUtilities::messagePump();
	if (!mRoot->renderOneFrame())
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool ApplicationBreslin::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mRenderTime = evt.timeSinceLastFrame;

	bool ret = BaseApplication::frameRenderingQueued(evt);
	
	return ret;
}

/*********************************
		GUI
**********************************/
//USERNAME
void ApplicationBreslin::createUsernameScreen()
{
        mLabelUsername     = mTrayMgr->createLabel(OgreBites::TL_CENTER, "mLabelUsername", "Username:");
        mLabelUsernameEdit = mTrayMgr->createLabel(OgreBites::TL_CENTER, "mLabelUsernameEdit", "");

        mButtonExit      = mTrayMgr->createButton(OgreBites::TL_CENTER, "mButtonExit", "Exit Application");
}

void ApplicationBreslin::showUsernameScreen()
{
        mTrayMgr->moveWidgetToTray(mLabelUsername,OgreBites::TL_CENTER);
        mTrayMgr->moveWidgetToTray(mLabelUsernameEdit,OgreBites::TL_CENTER);

        mTrayMgr->moveWidgetToTray(mButtonExit,OgreBites::TL_CENTER);

        mLabelUsername->show();
        mLabelUsernameEdit->show();

        mButtonExit->show();

        mTrayMgr->showCursor();
}

void ApplicationBreslin::hideUsernameScreen()
{
        mLabelUsername->hide();
        mLabelUsernameEdit->hide();

        mButtonExit->hide();
}

//PASSWORD
void ApplicationBreslin::createPasswordScreen()
{
        mLabelPassword     = mTrayMgr->createLabel(OgreBites::TL_CENTER, "mLabelPassword", "Password:");
        mLabelPasswordEdit = mTrayMgr->createLabel(OgreBites::TL_CENTER, "mLabelPasswordEdit", "");

        mButtonExit      = mTrayMgr->createButton(OgreBites::TL_CENTER, "mButtonExit", "Exit Application");
}

void ApplicationBreslin::showPasswordScreen()
{
        mTrayMgr->moveWidgetToTray(mLabelPassword,OgreBites::TL_CENTER);
        mTrayMgr->moveWidgetToTray(mLabelPasswordEdit,OgreBites::TL_CENTER);

        mTrayMgr->moveWidgetToTray(mButtonExit,OgreBites::TL_CENTER);

        mLabelPassword->show();
        mLabelPasswordEdit->show();

        mButtonExit->show();

        mTrayMgr->showCursor();
}

void ApplicationBreslin::hidePasswordScreen()
{
        mLabelPassword->hide();
        mLabelPasswordEdit->hide();

        mButtonExit->hide();
}

//MAIN
void ApplicationBreslin::createMainScreen()
{
	LogString("create Main buttons");
	mButtonGame = mTrayMgr->createButton(OgreBites::TL_CENTER, "mButtonGame", "Join Game");
	mButtonExit = mTrayMgr->createButton(OgreBites::TL_CENTER, "mButtonExit", "Exit Application");
}

void ApplicationBreslin::showMainScreen()
{

	mTrayMgr->moveWidgetToTray(mButtonGame,OgreBites::TL_CENTER);
	mTrayMgr->moveWidgetToTray(mButtonExit,OgreBites::TL_CENTER);
	
	mButtonGame->show();
	mButtonExit->show();
	
	mTrayMgr->showCursor();
}

void ApplicationBreslin::hideMainScreen()
{
	mButtonGame->hide();
	mButtonExit->hide();
}

void ApplicationBreslin::buttonHit(OgreBites::Button *button)
{
	mButtonHit = button;
}

bool ApplicationBreslin::mouseMoved( const OIS::MouseEvent &arg )
{
	if (mTrayMgr->injectMouseMove(arg))
	{
		return true;
	}
	if (mPlayingGame)
	{
		mCameraMan->injectMouseMove(arg);
	}
    return true;
}


/*************************************************
*
*   OGRE_SPECIFIC
*
**************************************************



/***************************************
*                       INPUT
******************************************/
bool ApplicationBreslin::keyPressed( const OIS::KeyEvent &arg )
{
	if( mEditString->injectKeyPress( arg ) == false )
	{
	
	}
	else
	{	
	}
}

