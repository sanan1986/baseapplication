#ifndef CLIENTPARTIDO_H
#define CLIENTPARTIDO_H

#include "../clientRobust.h"

#include <vector>

//postgresql
#include <postgresql/libpq-fe.h>

class ServerPartido;
class GamePartido;
class ShapePartido;
class Utility;
class Test;

class ClientPartido : public ClientRobust
{

public:

ClientPartido(ServerPartido* server, struct sockaddr *address, int clientID, bool permanence, int i, std::string username, std::string p, std::string first_name, std::string m1, std::string m2, std::string m3, std::string last_name, int s);	
~ClientPartido();

StateMachine<ClientPartido>* mClientPartidoStateMachine;

ServerPartido* mServerPartido;	

//utility
Utility* utility;

//test
Test* mTest;

//game
void addGame(GamePartido* gamePartido);
std::vector<GamePartido*> mGamePartidoVector;
GamePartido* mGamePartido;
GamePartido* getGame();
virtual void setGame(int gameID);

//battle
int mBattle;
int mBattleLast;

//shape
ShapePartido* mShapePartido;
void setShape(ShapePartido* shapePartido);

//reset
virtual void reset();

//update
virtual void update();

//handle letter 
virtual bool  handleLetter(Letter* letter);

};
#endif
