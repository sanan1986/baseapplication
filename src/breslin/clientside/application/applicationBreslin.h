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
class EditString;

class ApplicationBreslin : public BaseApplication
{
public:
	
	ApplicationBreslin(const char* serverIP, int serverPort);
	~ApplicationBreslin();

/***************************************
*   		MEMBER VARIABLES
***************************************/

public:

//state machine
StateMachine* mStateMachine;

State* mApplicationGlobal;
State* mApplicationInitialize;
State* mApplicationLogin;
State* mApplicationMain;
State* mApplicationPlay;

//constants
static const char mMessageServerExit = 3;
static const char mMessageConnect     = -101;
static const char mMessageDisconnect  = -102;

//Network
Network*     mNetwork;

//game
Game* mGame;

//state transition variables
bool mSetup;
bool mPlayingGame;
bool mFake;

/*****GUI ********************/

//button 
OgreBites::Button* mButtonHit;

//exit
OgreBites::Button* mButtonExit;

//game
OgreBites::Button* mButtonGame;

//login
OgreBites::Label* mLabelLogin;
OgreBites::Label* mLabelLoginEdit;


private:

//time
float mRenderTime;

public:
//input
EditString* mEditString;

//login
std::string mUsername;
std::string mPassword;

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
void createLoginScreen();
void showLoginScreen();
void hideLoginScreen();

void createMainScreen();
void showMainScreen();
void hideMainScreen();

//input
OIS::Keyboard* getKeyboard() { return mKeyboard; }

//private:

//connect
void sendConnect();
void sendLogin  ();

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
public:
virtual bool keyPressed( const OIS::KeyEvent &arg );

};

#endif
