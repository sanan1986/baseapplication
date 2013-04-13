#ifndef GAME_H
#define GAME_H

#include <string.h>

#include <vector>

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

//postgres
#include <postgresql/libpq-fe.h>

using namespace std;

#define COMMAND_HISTORY_SIZE		64

//class Network;
class Message;
class Server;
class Client;
class Shape;
class Vector3D;
class Bounds;

class Game
{
public:

//statics
static const char mMessageKey        = 1;
static const char mMessageFrameTime  = 2;
static const char mMessageServerExit = 3;

	Ogre::Root* mRoot;

	Server	*mServer;  //go between for game(contains game logic) and Network(handles sending message across internets)

	//bounds
	Bounds* mBounds;	

	//time
	int mTickLength;
	int mFrameTime;
	int mGameTime;
	int mFrameTimeLast;

	//shapes
	std::vector<Shape*> mShapeVector;	//every tangible item in game world..

	//sequence
	signed short	mOutgoingSequence;
	
	//db
     	PGconn* mDBConnection;

public:
	Game();
	~Game();


	//world
	virtual void createWorld();

	//index
	unsigned int getOpenIndex ();
	Vector3D* getOpenPoint    ();

	//end game
	virtual void checkForEndOfGame() { }

	// Network
	void	sendCommand();
	void	sendExitNotification();
	void	readDeltaMoveCommand(Message *mes, Client *client);
	
	//time
	void	processUpdate(int msec);
	
	//collision detection
	virtual void checkCollisions();
	virtual void collision(Shape* shape1, Shape* shape2);
	virtual void checkBounds(Shape* shape);
		
	//scope
 	bool    checkScope(Client* client, Shape* shape);

	//commands
	virtual void storeCommands(Shape* shape);
};

#endif
