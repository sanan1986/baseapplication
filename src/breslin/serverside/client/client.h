#ifndef CLIENT_H
#define CLIENT_H

#include <string>

#include "../message/message.h"

#ifdef WIN32
	#pragma comment (lib,"ws2_32.lib")
#pragma message ("Auto linking WinSock2 library")

	#include <winsock2.h>
#else
	#include <string.h>
	#include <netinet/in.h>
#endif

// Define SOCKET data type for UNIX (defined in WinSock for Win32)
// And socklen_t for Win32
#ifdef WIN32
	typedef int socklen_t;
#else
	typedef int SOCKET;

	#ifndef TRUE
	#define TRUE 1
	#endif
	#ifndef FALSE
	#define FALSE 0
	#endif
#endif

// Connection states
#define DREAMSOCK_CONNECTING			0
#define DREAMSOCK_CONNECTED			1
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

// Error codes
#define DREAMSOCK_SERVER_ERROR			1
#define DREAMSOCK_CLIENT_ERROR			2

// Introduce classes
class Server;
class Network;
class Shape;
class Game;

class Client
{
public:
	Client(Server* server, struct sockaddr *address, int clientID, bool disconnected);

~Client();

	//Message
        Message mMessage;

	//id used just for browser clients for now, if it's 0 then we know it's a c++ java client.
	int mClientID;

	//user id from database
	int mUserID;

	int				mConnectionState;		// Connecting, connected, disconnecting, disconnected

	signed short	mDroppedPackets;			// Dropped packets

	struct sockaddr	mSocketAddress;				// Socket address

	int				mLastMessageTime;

	bool mLoggedIn;
	std::string mStringUsername;
	std::string mStringPassword;

public:
	//processUpdate
	void processUpdate();

	//client
	void remove();

	//connect
	void sendConnected();

	//login
	void login();
	void logout();
	void checkLogin(Message* mes);
        bool getPasswordMatch(std::string username,std::string password);

	//clients address to send back messages to
	struct sockaddr *GetSocketAddress(void) { return &mSocketAddress; }
	void setSocketAddress(struct sockaddr *address); 

    	Shape* mShape;  //on server: everybody's got one ...same on clientside mShape is the clients personal avatar..

	Server* mServer;

	Game* mGame;
	
	//db
	int         db_id;
        std::string db_username;
        std::string db_password;
        std::string db_first_name;
        std::string db_last_name;
        int         db_school_id;

};
#endif
