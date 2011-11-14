//header
#include "application.h"

//log
#include "../tdreamsock/dreamSockLog.h"

//shape
#include "../shape/shape.h"

//math
#include "../../math/vector3D.h"

//byteBuffer
#include "../byteBuffer/byteBuffer.h"

//time
#include "../time/time.h"

//network
#include "../network/network.h"

//command
#include "../command/command.h"

//game
#include "game.h"


/***************************************
*			          CONSTRUCTORS
***************************************/
Application::Application(const char* serverIP, int serverPort)
{
	StartLog();

	// network
	mNetwork = new Network(this,serverIP,serverPort);

	//time
	mTime = new Time();
	mFrameTime		 = 0.0f;
	mOldTime         = 0;

	mGame = 0;

	//time
	mRenderTime = 0;

	//keys
	mKeyUp = 1;
	mKeyDown = 2;
	mKeyLeft = 4;
	mKeyRight = 8;
	mKeyCounterClockwise = 16;
	mKeyClockwise = 32;

	//initilize
	mInitializeGui = false;
	mJoinGame    = false;
	mPlayingGame = false;

	//OGRE_SPECIFIC
	mGame = new Game(this,serverIP,serverPort);
}

Application::~Application()
{
	mNetwork->close();
	delete mNetwork;
}

/*********************************
		START/LOOP/END
**********************************/
void Application::run()
{
	while(true)
    {
		//input
		processInput();
		
		//network
		runNetwork(getRenderTime() * 1000.0f);
		
		//move objects
		mGame->run();

		//draw
		if (!runGraphics())
		{
			break;
		}
	}
}

void Application::shutdown()
{
	mNetwork->sendDisconnect();
}


void Application::readServerTick(ByteBuffer* byteBuffer)
{
	// Skip sequences
	byteBuffer->ReadShort();

	while (byteBuffer->getReadCount() <= byteBuffer->GetSize())
	{
		//mDetailsPanel->setParamValue(11, Ogre::StringConverter::toString(byteBuffer->GetSize()));

		int id = byteBuffer->ReadByte();

		Shape* shape = NULL;
		shape = mGame->getShape(id);

		if (shape)
		{
			shape->readDeltaMoveCommand(byteBuffer);
			shape->processTick();
		}
	}
}

/*********************************
		NETWORK
**********************************/
void Application::runNetwork(float msec)
{
	static float time = 0.0f;
	time += msec;

	mNetwork->readPackets();
	
	// Framerate is too high
	if(time > (1000 / 60))
	{
		mNetwork->sendCommand();
		mFrameTime = time / 1000.0f;
		time = 0;
	}
}

/**********************************************
*
*		OGRE_SPECIFIC
*
**************************************************



/*********************************
*		TIME
***********************************/
float Application::getRenderTime()
{
	//LogString("r:%f",mGraphicsOgre->mRenderTime);
	return mRenderTime;
}


/*********************************
		GRAPHICS
**********************************/
void Application::createScene(void)
{
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.75, 0.75, 0.75));

    Ogre::Light* pointLight = mSceneMgr->createLight("pointLight");
    pointLight->setType(Ogre::Light::LT_POINT);
    pointLight->setPosition(Ogre::Vector3(250, 150, 250));
    pointLight->setDiffuseColour(Ogre::ColourValue::White);
    pointLight->setSpecularColour(Ogre::ColourValue::White);
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
	mRenderTime = evt.timeSinceLastFrame;

    bool ret = BaseApplication::frameRenderingQueued(evt);
	
	return ret;
}

bool Application::runGraphics()
{
	//Pump messages in all registered RenderWindow windows
	WindowEventUtilities::messagePump();
	if (!mRoot->renderOneFrame())
	{
		initializeGui();
		return false;
	}
	else
	{
		initializeGui();
		return true;
	}


}

/*********************************
		GUI
**********************************/
void Application::initializeGui()
{
	if (mInitializeGui == true)
	{
		return;
	}
	else
	{
		loadJoinScreen();
		mInitializeGui = true;
	}
}

void Application::loadJoinScreen()
{
	mJoinButton = mTrayMgr->createButton(OgreBites::TL_CENTER, "mJoinButton", "Join Game");
	mTrayMgr->moveWidgetToTray(mJoinButton,OgreBites::TL_CENTER);
	mTrayMgr->showCursor();
}

void Application::hideGui()
{
	hideJoinScreen();
	mTrayMgr->hideCursor();
}

void Application::hideJoinScreen()
{
	mTrayMgr->removeWidgetFromTray(mJoinButton);
    mJoinButton->hide();
}


/***************************************
*			INPUT
******************************************/

void Application::buttonHit(OgreBites::Button *button)
{
	//JOIN
	if (button == mJoinButton)
	{
		mJoinGame = true;
		if (mJoinGame && !mPlayingGame)
		{
			mNetwork->sendConnect();
			mPlayingGame = true;
		}
		hideGui();
	}
}

bool Application::mouseMoved( const OIS::MouseEvent &arg )
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

void Application::processInput()
{
	mNetwork->mCommandToServer->mKey = 0;
    
	if (mKeyboard->isKeyDown(OIS::KC_I)) // Forward
    {
		mNetwork->mCommandToServer->mKey |= mKeyUp;
    }

    if (mKeyboard->isKeyDown(OIS::KC_K)) // Backward
    {
		mNetwork->mCommandToServer->mKey |= mKeyDown;
    }

	if (mKeyboard->isKeyDown(OIS::KC_J)) // Left
    {
		mNetwork->mCommandToServer->mKey |= mKeyLeft;
    }

    if (mKeyboard->isKeyDown(OIS::KC_L)) // Right
    {
		mNetwork->mCommandToServer->mKey |= mKeyRight;
    }
    
	if (mKeyboard->isKeyDown(OIS::KC_Z)) // Rotate -Yaw(counter-clockwise)
    {
		mNetwork->mCommandToServer->mKey |= mKeyCounterClockwise;
    }

    if (mKeyboard->isKeyDown(OIS::KC_X)) // Right + Yaw(clockwise)
    {
		mNetwork->mCommandToServer->mKey |= mKeyClockwise;
    }
	mNetwork->mCommandToServer->mMilliseconds = (int) (mFrameTime * 1000);
}


