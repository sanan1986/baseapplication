#ifndef NETWORK_H
#define NETWORK_H

#include <string.h>
#include <netinet/in.h>

#include <stdio.h>
#include <stddef.h>

// Define SOCKET data type for UNIX (defined in WinSock for Win32)
// And socklen_t for Win32
typedef int SOCKET;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

// Connection protocols
#define DREAMSOCK_TCP					0
#define DREAMSOCK_UDP					1

#define DREAMSOCK_INVALID_SOCKET	-1

class DreamLinuxSock;

class Network 
{
public:
Network(const char netInterface[32], int port);
~Network();

DreamLinuxSock* mDreamLinuxSock;

// Function prototypes
void shutdown();
SOCKET createSocket(int protocol);
int setNonBlocking(SOCKET sock, u_long setMode);
int setBroadcasting(SOCKET sock, int mode);
int stringToSockaddr(const char *addressString, struct sockaddr *sadr);
SOCKET openUDPSocket(const char netInterface[32], int port);
void closeSocket(SOCKET sock);

int getPacket(SOCKET sock, char *data, struct sockaddr *from);
void sendPacket(SOCKET sock, int length, char *data, struct sockaddr addr);
void broadcast(SOCKET sock, int length, char *data, int port);

int getCurrentSystemTime();

SOCKET mSocket;

};
#endif
