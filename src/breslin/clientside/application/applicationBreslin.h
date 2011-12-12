#ifndef APPLICATIONBRESLIN_H
#define APPLICATIONBRESLIN_H

/***************************************
*   		INCLUDES
***************************************/

//parents
#include "BaseApplication.h"

/***************************************
*   		FORWARD DECLARATIONS
***************************************/
class Game;
class Network;
class StateMachine;
class State;

class ApplicationBreslin : public BaseApplication
{
public:
	
	ApplicationBreslin(const char* serverIP, int serverPort);
	~ApplicationBreslin();

/***************************************
*   		MEMBER VARIABLES
***************************************/

public:

//private:

//state machine
StateMachine* mStateMachine;

State* mApplicationGlobal;
State* mApplicationInitialize;
State* mApplicationInitializeGraphics;
State* mApplicationMain;
State* mApplicationPlay;

//constants
static const char mMessageServerExit = 3;

static const char mMessageConnect     = -101;
static const char mMessageDisconnect  = -102;

//time
float mRenderTime;

//Network
Network*     mNetwork;

//game
Game* mGame;

//initialize
bool mSetup;
bool mPlayingGame;

//gui
OgreBites::Button* mButtonHit;
OgreBites::Button* mButtonExit;
OgreBites::Button* mButtonGame;
OgreBites::Button* mButtonTag;

/***************************************
*			          METHODS
***************************************/

public:

//process update
void processUpdate();

//time
float getRenderTime();

//scene
Ogre::SceneManager* getSceneManager      () { return mSceneMgr; }

//gui
void createMainScreen();
void showMainScreen();
void hideMainScreen();

//input
OIS::Keyboard* getKeyboard() { return mKeyboard; }

//private:

//connect
void sendConnect   ();

//shutdown
void shutdown();

//graphics
bool runGraphics();

//camera
Ogre::Camera* getCamera() { return mCamera; }

private:
void createScene          ();

//rendering
virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);

//input
void buttonHit             (OgreBites::Button *button);
bool mouseMoved            ( const OIS::MouseEvent &arg );



};

#endif
