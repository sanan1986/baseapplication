#ifndef CLIENTPARTIDO_H
#define CLIENTPARTIDO_H

#include "client.h"

class ServerPartido;

class ClientPartido : public Client
{
public:
 	ClientPartido(ServerPartido* server, struct sockaddr *address, int clientID);	
	~ClientPartido();

	void sendSchools();
	void parseAnswer();

	ServerPartido* mServer;	

	void parseAnswer(Message* message);
};

#endif
