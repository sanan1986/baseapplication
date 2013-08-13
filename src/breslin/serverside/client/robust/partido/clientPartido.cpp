#include "clientPartido.h"
//log
#include "../../../tdreamsock/dreamSockLog.h"

#include "../../../server/partido/serverPartido.h"
#include "../../../network/network.h"
#include "../../../game/partido/gamePartido.h"
#include "../../../shape/partido/shapePartido.h"

//utility
#include "../../../../utility/utility.h"

//states
#include "states/clientPartidoStates.h"

//quiz
#include "../../../quiz/quiz.h"


ClientPartido::ClientPartido(ServerPartido* serverPartido, struct sockaddr *address, int clientID, bool permanence,int i, std::string u, std::string p, std::string f, std::string m1, std::string m2, std::string m3, std::string l,int s) : ClientRobust(serverPartido, address, clientID, permanence,i, u,p,f,m1,m2,m3,l,s) 
{
	//server
	mServerPartido = serverPartido;

	//game
	mGamePartido = NULL;

	//quiz
	mQuiz = new Quiz(this);

        //states
        mClientPartidoStateMachine =  new StateMachine<ClientPartido>(this);
	if (clientID == -1)
	{
        	mClientPartidoStateMachine->setCurrentState      (NULL);
	}
	else
	{
        	mClientPartidoStateMachine->setCurrentState      (Game_Partido_Mode::Instance());
	}
        mClientPartidoStateMachine->setPreviousState     (NULL);
        mClientPartidoStateMachine->setGlobalState       (GlobalClientPartido::Instance());
}

ClientPartido::~ClientPartido()
{
	delete mClientPartidoStateMachine;
}

//game
void ClientPartido::addGame(GamePartido* gamePartido)
{
	ClientRobust::addGame(gamePartido);
        mGamePartidoVector.push_back(gamePartido);
}

GamePartido* ClientPartido::getGame()
{
	return mGamePartido;
}

void ClientPartido::setGame(int gameID)
{
        for (int i = 0; i < mGamePartidoVector.size(); i++)
        {
                if (mGamePartidoVector.at(i)->mID == gameID)
                {
                        mGamePartido = mGamePartidoVector.at(i);
                        mGamePartido->sendShapes(this);
                }
        }
}

bool ClientPartido::handleLetter(Letter* letter)
{
	return ClientRobust::handleLetter(letter);
}

//updates
void ClientPartido::update()
{
	ClientRobust::update();
	mClientPartidoStateMachine->update();
}

void ClientPartido::setShape(ShapePartido* shapePartido)
{
	ClientRobust::setShape(shapePartido);
	mShapePartido = shapePartido;
}

