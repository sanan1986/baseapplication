#include "gamePartido.h"

//log
#include "../tdreamsock/dreamSockLog.h"

//server
#include "../server/serverPartido.h"

//shape
#include "../shape/shapePartido.h"

//client
#include "../client/client.h"

//battles
#include "../battle/battle.h"

#include <stdio.h>

GamePartido::GamePartido(ServerPartido* server, int id) : Game(server,id)
{
	mServer = server;
}

GamePartido::~GamePartido()
{
}

//you should call this from server processUpdate
void GamePartido::processUpdate()
{
	Game::processUpdate();
	
	//process Battles	
	for (unsigned int i = 0; i < mBattleVector.size(); i++)
        {
                mBattleVector.at(i)->processUpdate();
        }
}

void GamePartido::collision(Shape* shape1, Shape* shape2)
{
	//do regular collision of backing off shapes
	Game::collision(shape1,shape2);	

	bool battleCollision = true;

	//now let's check if these guys are already in a battle
	for (unsigned int i = 0; i < mBattleVector.size(); i++)
	{
		for (unsigned int s = 0; s < mBattleVector.at(i)->mShapeVector.size(); s++)
		{
			if (shape1 == mBattleVector.at(i)->mShapeVector.at(s) || shape2 == mBattleVector.at(i)->mShapeVector.at(s))
			{
				battleCollision = false;	
			} 
		}
	}

	if (battleCollision)
	{

		std::vector<Shape*> shapeVector;
		shapeVector.push_back(shape1);
		shapeVector.push_back(shape2);

		//create a battle
		Battle* battle = new Battle(this,shapeVector);
		mBattleVector.push_back(battle);
	}
}

void GamePartido::sendAnswer(Client* client, std::string answer)
{
	LogString("GamePartido::sendAnswer from db_id:%d", client->db_id);
} 
