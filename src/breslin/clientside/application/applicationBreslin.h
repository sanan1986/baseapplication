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
class ByteBuffer;
class Shape;
class Time;
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

//constants
static const char mCommandMilliseconds = 2;

//private:

//state machine
StateMachine* mStateMachine;

State* mApplicationGlobal;
State* mApplicationInitialize;
State* mApplicationInitializeGraphics;
State* mApplicationMain;
State* mApplicationPlay;

//constants
static const char mCommandKey          = 1;

static const char mMessageFrame = 1;

static const char mMessageConnect     = -101;
static const char mMessageDisconnect  = -102;
static const char mMessageAddShape    = -103;
static const char mMessageRemoveShape = -104;

static const char mMessageServerExit = 3;
static const char mMessageKeepAlive = 12;

//Network
Network*     mNetwork;

//game
Game* mGame;

//sequences 
signed short	mOutgoingSequence;		

//time
float mFrameTime;
float mRenderTime;
float mRunNetworkTime;

//initialize
bool mSetup;
bool mInitializeGui;
bool mGraphicsRunning;
bool mJoinGame;
bool mPlayingGame;

//keys
int mKeyUp;
int mKeyDown;
int mKeyLeft;
int mKeyRight;
int mKeyCounterClockwise;
int mKeyClockwise;
	
//key input
int mKeyCurrent;         
int mKeyLast;
int mMillisecondsCurrent;
int mMillisecondsLast;

//gui
OgreBites::Button *mButtonHit;
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

//packets
void readPackets   ();

//Ticks
void readServerTick           (ByteBuffer* byteBuffer);

// Network
void runNetwork    (float msec);

//input
void processInput();

//graphics
bool runGraphics();
	
//graphics
void        createScene          ();

//rendering
virtual bool        frameRenderingQueued (const Ogre::FrameEvent& evt);

//input
void buttonHit             (OgreBites::Button *button);
bool mouseMoved            ( const OIS::MouseEvent &arg );

//command
void sendCommand   ();
};

#endif
