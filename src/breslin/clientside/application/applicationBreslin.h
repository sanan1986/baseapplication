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

//state machine
StateMachine* mStateMachine;

State* mApplicationGlobal;
State* mApplicationInitialize;
State* mApplicationLogin;
State* mApplicationMain;
State* mApplicationPlay;

//constants
static const char mMessageServerExit  = 3;
static const char mMessageConnect     = -101;
static const char mMessageConnected   = -90;
static const char mMessageAddSchool   = -109;
static const char mMessageLogin       = -110;
static const char mMessageLogout      = -120;
static const char mMessageLoggedIn    = -113;
static const char mMessageLoggedOut   = -114;
static const char mMessageJoinGame    = -107;
static const char mMessageDisconnect  = -102;
static const char mMessageLeaveGame   =  -99;

//Network
Network*     mNetwork;

//game
Game* mGame;

//state transition variables
bool mSetup;
bool mConnected;
bool mPlayingGame;
bool mFake;
bool mConnectSent;
bool mLoggedIn;
bool mLeaveGame;
bool mSentLeaveGame;

//gui
OgreBites::SelectMenu* mSelectMenuSchool;
OgreBites::Button* mButtonHit;

OgreBites::Label* mLabelUsername;
OgreBites::Label* mLabelPassword;
OgreBites::Label* mLabelFocus;

OgreBites::Button* mButtonLogin;
OgreBites::Button* mButtonLogout;
OgreBites::Button* mButtonJoinGameA;
OgreBites::Button* mButtonExit;

std::string mStringSchool;

std::string mStringUsername;
std::string mStringPassword;

public:
//time
float mRenderTime;

private:
/***************************************
*			          METHODS
***************************************/

public:

//states
virtual void createStates();
virtual void setStates();

//process update
virtual void processUpdate();

//time
float getRenderTime();

//scene
Ogre::SceneManager* getSceneManager      () { return mSceneMgr; }

//gui
//login
void createLoginScreen();
void showLoginScreen();
void hideLoginScreen();

//main
virtual void createMainScreen();
virtual void showMainScreen();
virtual void hideMainScreen();

//input
OIS::Keyboard* getKeyboard() { return mKeyboard; }

//keys
bool mKey_up;
bool mKey_down;
bool mKey_right;
bool mKey_left;
bool mKey_counterclockwise;
bool mKey_clockwise;
bool mKey_esc;
bool mKey_q;
bool mKey_tab;
bool mKey_enter;

/********* NETWORK *******/
void checkForByteBuffer();

//connect
void sendConnect   ();

//join game
void sendJoinGame(int gameID);

//login 
void sendLogin();
void sendLogout();

//shutdown
void shutdown();

//graphics
bool runGraphics();

//camera
Ogre::Camera* getCamera() { return mCamera; }

private:
void createScene          ();

public:
//rendering
virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);

private:
//input
void buttonHit  (OgreBites::Button *button);
void labelHit   (OgreBites::Label* label );
bool mouseMoved (const OIS::MouseEvent &arg );
protected:
virtual bool keyPressed  ( const OIS::KeyEvent &arg );
virtual bool keyReleased ( const OIS::KeyEvent &arg );

};

#endif
