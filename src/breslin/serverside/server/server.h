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

#ifdef WIN32
	#define DREAMSOCK_INVALID_SOCKET	INVALID_SOCKET
#else
	#define DREAMSOCK_INVALID_SOCKET	-1
#endif

// System messages
// Note (for all messages - system and user):
// positive = sequenced message
// negative = un-sequenced message

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

// Introduce classes
class Network;
class Client;
class Game;
class Shape;

class Server
{
public:

	//Ogre Root
	Ogre::Root* mRoot;

	//games
	std::vector<Game*> mGameVector;

	//clients
	std::vector<Client*> mClientVector;

	//port
	int mPort;					// Port

	//time
	int mTickLength;
 	int mFrameTime;
        int mGameTime;
        int mFrameTimeLast;
 	
	//sequence
        signed short    mOutgoingSequence;

	//network
	Network* mNetwork;

	//Message
	Message mMessage;

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

	//quit game
	static const int mMessageQuitGame               = -108;
	static const int mMessageQuitGameBrowser        = -118;

	//disconnect	
	static const int mMessageDisconnect             = -102;
	static const int mMessageDisconnectBrowser      = -112;

	//add school
	static const int mMessageAddSchool              = -109;

	//add question 
	static const int mMessageAddQuestion            = -76;

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
	void processUpdate(int msec);

	//port
	int getPort() { return mPort; }

	//schools
	std::vector<std::string> mSchoolVector;  
	void getSchools();

	//questions
	std::vector<std::string> mQuestionIDVector;  
	std::vector<std::string> mQuestionsVector;  
	std::vector<std::string> mAnswersVector;  
	std::vector<std::string> mLevelsVector;  
	void getQuestions();

	//timeout
	int  checkForTimeout();

	//game
	void addGame(Game* game);

	//client
	void addClient(Client* client);
	
	//packets
	int  getPacket  (char *data, struct sockaddr *from);
	void sendPackets();
	void readPackets();
	void parsePacket(Message *mes, struct sockaddr *address);

	//db
	void readDB();
	
        //commands
        void storeCommands(Shape* shape);
	void sendCommand(Game* game);
};

#endif
