
#include "server.h"
#include "../tdreamsock/dreamSockLog.h"

#include "../network/network.h"
#include "../client/client.h"
#include "../message/message.h"
#include "../shape/shape.h"
#include "../game/game.h"


Server::Server(Game* serverSideGame,const char *localIP, int serverPort)
{
	init			= false;
	mGame = serverSideGame;
	mLocalIP = localIP;
	
	// Store the server IP and port for later use
	port = serverPort;

	// Create network
	mNetwork = new Network(localIP, port);

	init = true;

	//codes
	//mConnect      = -101;
//	mDisconnect   = -102;
//	mAddShape     = -103;
//	mRemoveShape  = -104;
}

Server::~Server()
{
	mClientVector.empty();
	mNetwork->dreamSock_CloseSocket(mNetwork->mSocket);
}

void Server::writeAddShape(Client* client, char local)
{
			client->mMessage.WriteByte(mAddShape); // type
			client->mMessage.WriteByte(local);
			client->mMessage.WriteByte(client->mShape->mIndex);
			
			client->mMessage.WriteFloat(client->mShape->mCommand.mPosition.x);
			client->mMessage.WriteFloat(client->mShape->mCommand.mPosition.y);
			client->mMessage.WriteFloat(client->mShape->mCommand.mPosition.z);

			client->mMessage.WriteFloat(client->mShape->mCommand.mPositionVelocity.x);
			client->mMessage.WriteFloat(client->mShape->mCommand.mPositionVelocity.y);
			client->mMessage.WriteFloat(client->mShape->mCommand.mPositionVelocity.z);

			client->mMessage.WriteFloat(client->mShape->mCommand.mRotation.x);
			client->mMessage.WriteFloat(client->mShape->mCommand.mRotation.z);

			//mesh
			client->mMessage.WriteByte(client->mShape->mMeshCode);

			//animation
			client->mMessage.WriteByte(client->mShape->mAnimated);
}

//send a shape that has a client. i.e. a new human player
void Server::sendAddShape(Client* client)
{
	// init mMessage for client
	client->mMessage.Init(client->mMessage.outgoingData,
		sizeof(client->mMessage.outgoingData));

	//this a new client so let him change to connectionState = DREAMSOCK_CONNECTED; 
	client->mMessage.WriteByte(mConnect);	// type
	client->SendPacket(&client->mMessage);

	// First inform the new client of the other shapes by looping thru entire
	//mShapeVector one by one and sending out client->SendPacket();
	for (unsigned int i = 0; i < mGame->mShapeVector.size(); i++)
	{
		//init mMessage for client
		client->mMessage.Init(client->mMessage.outgoingData,
			sizeof(client->mMessage.outgoingData));

		//client->mMessage.WriteByte(mAddShape); // tell internet clients to add a shape

		if (mGame->mShapeVector.at(i) == client->mShape)
		{
			writeAddShape(client,1);
			//client->mMessage.WriteByte(1);	// local shape
			//client->mMessage.WriteByte(mGame->mShapeVector.at(i)->mIndex);
		}
		else
		{
			writeAddShape(client,0);
			//client->mMessage.WriteByte(0);	// not-local shape
			//client->mMessage.WriteByte(mGame->mShapeVector.at(i)->mIndex);
		}
		/*
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPosition.x);
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPosition.y);
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPosition.z);


		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPositionVelocity.x);
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPositionVelocity.y);
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mPositionVelocity.z);


		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mRotation.x);
		client->mMessage.WriteFloat(mGame->mShapeVector.at(i)->mCommand.mRotation.z);

		//mesh
		client->mMessage.WriteByte(mGame->mShapeVector.at(i)->mMeshCode);

		//animate
		client->mMessage.WriteByte(mGame->mShapeVector.at(i)->mAnimated);
		*/
		client->SendPacket(&client->mMessage);
	}

	// Then tell the others about the new shape
	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		//don't send to the client you allready sent to above...
		if(mClientVector.at(i) == client)
		{
			continue;
		}
		else //inform everyone else
		{
			//init mMessage for client
			mClientVector.at(i)->mMessage.Init(mClientVector.at(i)->mMessage.outgoingData,
				sizeof(mClientVector.at(i)->mMessage.outgoingData));
			writeAddShape(mClientVector.at(i),0);
			mClientVector.at(i)->SendPacket(&mClientVector.at(i)->mMessage);
		}
	}
}

//this is your serverside guy. he has no client, but we still need to tell everyone about the chap
void Server::sendAddAIShape(Shape* shape)
{
	// Send 'Add Shape' message to every client
	// Then tell the others about the new shape
	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		//init mMessage for client
		mClientVector.at(i)->mMessage.Init(mClientVector.at(i)->mMessage.outgoingData,
			sizeof(mClientVector.at(i)->mMessage.outgoingData));
		writeAddShape(mClientVector.at(i),0);
/*
		mClientVector.at(i)->mMessage.WriteByte(mAddShape); // type
		mClientVector.at(i)->mMessage.WriteByte(0);
		mClientVector.at(i)->mMessage.WriteByte(shape->mIndex);

		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPosition.x);
		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPosition.y);
		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPosition.z);

		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPositionVelocity.x);
		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPositionVelocity.y);
		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mPositionVelocity.z);


		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mRotation.x);
		mClientVector.at(i)->mMessage.WriteFloat(shape->mCommand.mRotation.z);

		//mesh
		mClientVector.at(i)->mMessage.WriteByte(shape->mMeshCode);

		//animated
		mClientVector.at(i)->mMessage.WriteByte(shape->mAnimated);
*/
		mClientVector.at(i)->SendPacket(&mClientVector.at(i)->mMessage);
	}
}

void Server::sendRemoveShape(Shape* shape)
{
	
	int index = shape->mIndex;

	// Send 'DREAMSOCK_MES_REMOVESHAPE' message to every client
	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		mClientVector.at(i)->mMessage.Init(mClientVector.at(i)->mMessage.outgoingData,
			sizeof(mClientVector.at(i)->mMessage.outgoingData));

		mClientVector.at(i)->mMessage.WriteByte(mRemoveShape);	// type
		mClientVector.at(i)->mMessage.WriteByte(index);							// index
	}
	sendPackets();
}

//called when internets client sends DREAMSOCK_MES_CONNECT message before it has a client, shape or anything.
void Server::addClient(struct sockaddr *address)
{
	Client* client = new Client(mNetwork);
	mClientVector.push_back(client);

	client->SetSocketAddress(address);

	client->mConnectionState = DREAMSOCK_CONNECTING;
	client->mOutgoingSequence = 1;
	client->mIncomingSequence = 0;

	memcpy(&client->mMyaddress,client->GetSocketAddress(), sizeof(struct sockaddr));

	mGame->createClientAvatar(client,true,true,.66f,1,false);

	LogString("LIB: Adding client with shape index %d", client->mShape->mIndex);

	sendAddShape(client);  
}

void Server::removeClient(Client *client)
{
	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		if (mClientVector.at(i) == client)
		{
			mGame->removeShape(client->mShape); //remove the shape associated with this client while your at it.
			mClientVector.erase (mClientVector.begin()+i);
		}
	}
}

void Server::parsePacket(Message *mes, struct sockaddr *address)
{
	mes->BeginReading();
    int type = mes->ReadByte();
	//LogString("pp");
	if (type == mConnect)
	{
				addClient(address);
				LogString("LIBRARY: Server: a client connected succesfully");
	}
	else
	{
		// Find the correct client by comparing addresses
		for (unsigned int i = 0; i < mClientVector.size(); i++)
		{
			if(memcmp(mClientVector.at(i)->GetSocketAddress(), address, sizeof(address)) == 0)
			{
				mClientVector.at(i)->mLastMessageTime = mNetwork->dreamSock_GetCurrentSystemTime();

				// Check if the type is a positive number
				// -> is the packet sequenced
				if(type > 0)
				{
					signed short sequence         = mes->ReadShort();

					if(sequence <= mClientVector.at(i)->mIncomingSequence)
					{
						LogString("LIB: Server: Sequence mismatch (sequence: %ld <= incoming seq: %ld)",
						sequence, mClientVector.at(i)->mIncomingSequence);
					}

					mClientVector.at(i)->mDroppedPackets  = sequence - mClientVector.at(i)->mIncomingSequence - 1;
					//set mIncomingSequence to current one that just came in for next time comparison...
					mClientVector.at(i)->mIncomingSequence = sequence;
				}

				// Wait for one message before setting state to connected
				if(mClientVector.at(i)->mConnectionState == DREAMSOCK_CONNECTING)
				{
					mClientVector.at(i)->mConnectionState = DREAMSOCK_CONNECTED;
				}

				// Parse through the system messages
				switch(type)
				{
					case mDisconnect:
						//if(mClientVector.at(i) == NULL)
						//	break;

					    removeClient(mClientVector.at(i));

						LogString("LIBRARY: Server: a client disconnected");
						break;
				}
			}
		}
	}
}

int Server::checkForTimeout(char *data, struct sockaddr *from)
{
	int currentTime = mNetwork->dreamSock_GetCurrentSystemTime();

	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		// Don't timeout when connecting
		if(mClientVector.at(i)->mConnectionState == DREAMSOCK_CONNECTING)
		{
			continue;
		}

		// Check if the client has been silent for 30 seconds
		// If yes, assume crashed and remove the client
		if(currentTime - mClientVector.at(i)->mLastMessageTime > 30000)
		{
			LogString("Client timeout, disconnecting (%d - %d = %d)",
				currentTime, mClientVector.at(i)->mLastMessageTime, currentTime - mClientVector.at(i)->mLastMessageTime);

			// Build a 'fake' message so the application will also
			// receive notification of a client disconnecting
			Message mes;
			mes.Init(data, sizeof(data));
			mes.WriteByte(mDisconnect);

			*(struct sockaddr *) from = *mClientVector.at(i)->GetSocketAddress();

			removeClient(mClientVector.at(i));

			return mes.GetSize();
		}
	}
	return 0;
}

int Server::getPacket(char *data, struct sockaddr *from)
{
	// Check if the server is set up
	if(!mNetwork->mSocket)
		return 0;

	// Check for timeout
	int timeout = checkForTimeout(data, from);

	if(timeout)
		return timeout;

	// Wait for a while or incoming data
	int maxfd = mNetwork->mSocket;
	fd_set allset;
	struct timeval waittime;

	waittime.tv_sec = 10 / 1000;
	waittime.tv_usec = (10 % 1000) * 1000;

	FD_ZERO(&allset); 
	FD_SET(mNetwork->mSocket, &allset);

	fd_set reading = allset;

	int nready = select(maxfd + 1, &reading, NULL, NULL, &waittime);

	if(!nready)
		return 0;

	// Read data of the socket
	int ret = 0;

	Message mes;
	mes.Init(data, sizeof(data));

	ret = mNetwork->dreamSock_GetPacket(mNetwork->mSocket, mes.data, from);

	if(ret <= 0)
		return 0;

	mes.SetSize(ret);

	// Parse system messages
	parsePacket(&mes, from);

	return ret;
}

//this loops thru each client instance and then calls their sendPacket(mess) function
void Server::sendPackets(void)
{
	// Check if the server is set up
	if(!mNetwork->mSocket)
		return;

	for (unsigned int i = 0; i < mClientVector.size(); i++)
	{
		if(mClientVector.at(i)->mMessage.GetSize() == 0)
			continue;

		mClientVector.at(i)->SendPacket(&mClientVector.at(i)->mMessage);
	}
}

void Server::readPackets(void)
{
	char data[1400];

	int type;
	int ret;

	struct sockaddr address;

	Message mes;
	mes.Init(data, sizeof(data));

	// Get the packet from the socket
	try
	{
		while(ret = getPacket(mes.data, &address))
		{
			mes.SetSize(ret);
			mes.BeginReading();

			type = mes.ReadByte();
			//LogString("type:%d",type);
			// Check the type of the message
			switch(type)
			{
			case mConnect:

				break;

			case mDisconnect:

				for (unsigned int i = 0; i < mClientVector.size(); i++)
				{
					if(memcmp(&mClientVector.at(i)->mMyaddress, &address, sizeof(address)) == 0)
					{
						//mClientVector.erase(
					}
				}
				break;

			case USER_MES_FRAME:
			//LogString("Got frame (size: %d bytes)", ret);

				// Skip sequences
				signed short seq = mes.ReadShort();
				//LogString("seq:%d",seq);
				//let's try this with shapes instead.....
				for (unsigned int i = 0; i < mGame->mShapeVector.size(); i++)
				{
					if (mGame->mShapeVector.at(i)->mClient != NULL)
					{
						if(memcmp(&mGame->mShapeVector.at(i)->mClient->mMyaddress, &address, sizeof(address)) == 0)
						{
							mGame->readDeltaMoveCommand(&mes, mGame->mShapeVector.at(i)->mClient);
							mGame->mShapeVector.at(i)->processTick();

							break;
						}
					}
				}
				break;
			}
		}
	}
	catch(...)
	{
		LogString("Unknown Exception caught in Lobby ReadPackets loop");

#ifdef WIN32
		MessageBox(NULL, "Unknown Exception caught in Lobby ReadPackets loop", "Error", MB_OK | MB_TASKMODAL);
#endif
	}
}
