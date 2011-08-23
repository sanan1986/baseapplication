#ifndef NETWORK_H
#define NETWORK_H

/**********************************
*          INCLUDES
**********************************/
#ifdef WIN32
	#pragma comment (lib,"ws2_32.lib")
	#pragma message ("Auto linking WinSock2 library")

	#include <winsock2.h>
#else
	#include <string.h>
	#include <netinet/in.h>
#endif

#include <stdio.h>
#include <stddef.h>

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

// Connection protocols
#define DREAMSOCK_TCP					0
#define DREAMSOCK_UDP					1

#ifdef WIN32
	#define DREAMSOCK_INVALID_SOCKET	INVALID_SOCKET
#else
	#define DREAMSOCK_INVALID_SOCKET	-1
#endif

/**********************************
*          FORWARD DECLARATIONS
**********************************/

#ifdef WIN32
class DreamWinSock;
#else
class DreamLinuxSock;
#endif

class Game;
class Command;
class Dispatch;
class Parser;

/**********************************
*          CLASS
**********************************/

class Network
{
public:
Network(Game* game, const char serverIP[32], int serverPort);
~Network();

/**********************************
*          VARIABLES
**********************************/
public:

//game
Game* mGame;

//command
Command* mCommandToServer; //for the human moves to be sent off to server
Command* mLastCommandToServer;

//parser
Parser* mParser;

//server address
int			 mServerPort;				// Port
const char*  mServerIP;


//sequences and packet loss stats
unsigned short	mOutgoingSequence;		// OutFgoing packet sequence
private:
unsigned short	mIncomingSequence;		// Incoming packet sequence
unsigned short	mDroppedPackets;			// Dropped packets

//socket
SOCKET mSocket;

//address
struct sockaddr_in sendToAddress;

#ifdef WIN32
DreamWinSock* mDreamWinSock;
#else
DreamLinuxSock* mDreamLinuxSock;
#endif

/**********************************
*          METHODS
**********************************/
public:

//send
void send(Dispatch* dispatch);

//receive
int  checkForDispatch(Dispatch* dispatch);

//close
void close();

//reset
void            reset();

//connect
void sendConnect   (const char *name);

//disconnect
void sendDisconnect(void);

//command
void sendCommand   ();

//packets
void readPackets   ();


private:

//create
SOCKET createSocket(int protocol);
void setSendToAddress(const char* serverIP, int serverPort);

//open
SOCKET open();

//block
int    setNonBlocking(u_long setMode);

//send
void send			 (int length, char *data, struct sockaddr addr);

//parse
void parsePacket(Dispatch *mes);





};
#endif