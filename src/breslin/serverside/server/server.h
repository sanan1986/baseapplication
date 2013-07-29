#ifndef SERVER_H
#define SERVER_H

#include "../message/message.h"

#include <vector>
#include <string>

// Connection states
#define DREAMSOCK_CONNECTING			0
#define DREAMSOCK_CONNECTED				1
#define DREAMSOCK_DISCONNECTING			2
#define DREAMSOCK_DISCONNECTED			4
#define DREAMSOCK_INVALID_SOCKET	-1

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

// Introduce classes
class BaseEntity;
class Network;
class ClientRobust;
class Game;
class Shape;
class MailMan;

class Server
{
public:

	//Ogre Root
	Ogre::Root* mRoot;

	//network
	Network* mNetwork;

	//Message
	Message mMessage;

	//MailMan
	MailMan* mMailMan;
	
	//games
	std::vector<Game*> mGameVector;

	//clients
	std::vector<ClientRobust*> mClientVector;
	std::vector<Client*> mClientVectorTemp;

	//port
	int mPort;					// Port

	//time
	int mTickLength;
 	int mFrameTime;
        int mGameTime;
        int mFrameTimeLast;
 	
	//sequence
        signed short    mOutgoingSequence;

	//address
	const char *mLocalIP;

	//codes
	//frames
	static const int mMessageFrame 	                = 1;
	static const int mMessageFrameBrowser     	= 2;

	//questions	
	static const int mMessageQuestion               = -105;
	static const int mMessageQuestionBrowser        = -115;

	//answer
	static const int mMessageAnswer                 = -106;
	static const int mMessageAnswerBrowser          = -116;

	//connect
	static const int mMessageConnected              = -90;
	static const int mMessageConnect                = -101;
	static const int mMessageConnectBrowser         = -111;
	static const int mMessageConnectNode            = -121;

	//join game
	static const int mMessageJoinGame               = -107;
	static const int mMessageJoinGameBrowser        = -117;
	static const int mMessageLeaveGame              = -99;
	static const int mMessageLeaveGameBrowser       = -45;

	//disconnect	
	static const int mMessageDisconnect             = -102;
	static const int mMessageDisconnectBrowser      = -112;

	//login
	static const int mMessageLogin              	 = -110;
	static const int mMessageLoginBrowser        	 = -125;
	static const int mMessageLogout        	         = -120;
	static const int mMessageLogoutBrowser        	 = -98;
	static const int mMessageLoggedIn             	 = -113;
	static const int mMessageLoggedOut            	 = -114;
	
	//add shape	
	static const int mMessageAddShape    	         = -103;
	static const int mMessageRemoveShape             = -104;

public:
	Server(Ogre::Root* root, const char *localIP, int serverPort);
	~Server();

	//update
	void update(int msec);
	virtual void processGames();
	virtual void processClients();
	virtual void sendCommands();

	//game
	void addGame(Game* game);	

	//client
   	void createClients();
	virtual void addClient(Client* client, bool permanent);

	//packets
	int  getPacket  (char *data, struct sockaddr *from);
	void sendPackets();
        void readPackets();
	virtual void parsePacket(Message *mes, struct sockaddr *address);

        //commands
        void storeCommands(Shape* shape);
	void sendCommand(Game* game);
};

#endif
