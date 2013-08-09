/***
these states are always on...as their is always a gamePartido..and these clients are in it. it really should handle the mechanics of quiz... 

*/
//parent
#include "clientPartidoStates.h"

//log
#include "../../../../tdreamsock/dreamSockLog.h"

//states
#include "../../../../../fsm/stateMachine.h"

//ability
#include "../clientPartido.h"

//server
#include "../../../../server/partido/serverPartido.h"

//shapePartido
#include "../../../../shape/partido/shapePartido.h"

//question
#include "../../../../question/question.h"

/*****************************************
*******       GLOBAL    ******************
****************************************/
GlobalClientPartido* GlobalClientPartido::Instance()
{
  static GlobalClientPartido instance;
  return &instance;
}
void GlobalClientPartido::enter(ClientPartido* clientPartido)
{
}
void GlobalClientPartido::execute(ClientPartido* clientPartido)
{

}
void GlobalClientPartido::exit(ClientPartido* clientPartido)
{
}
bool GlobalClientPartido::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false;
}


/*****************************************
*******       CLIENT PARTIDO STATES    ******************       
****************************************/

/*****************************************
                GAME_PARTIDO_MODE               
****************************************/
Game_Partido_Mode* Game_Partido_Mode::Instance()
{
  static Game_Partido_Mode instance;
  return &instance;
}
void Game_Partido_Mode::enter(ClientPartido* clientPartido)
{
	if (clientPartido->id == 2)
	{	
        	LogString("Game_Partido_Mode::enter");
	}
}
void Game_Partido_Mode::execute(ClientPartido* clientPartido)
{
  	if (clientPartido->mShapePartido->mOpponent)
        {
                clientPartido->mClientPartidoStateMachine->changeState(Battle::Instance());
        }
}
void Game_Partido_Mode::exit(ClientPartido* clientPartido)
{
}
bool Game_Partido_Mode::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false; 
}
/***************************************
       	Battle
//battle
int mQuestionID;
std::vector<int> mMasteredQuestionIDVector;
bool mWaitingForAnswer;
std::string mQuestionString;
//int mBattleScore;
std::string mBattleRecordText;
int mComputerAskedTime;
int mComputerAnswerTime;
****************************************/
Battle* Battle::Instance()
{
  static Battle instance;
  return &instance;
}
void Battle::enter(ClientPartido* clientPartido)
{
	if (clientPartido->id == 2)
	{	
        	LogString("Battle::enter");
	}
	if (clientPartido->mLoggedIn)
        {
                clientPartido->sendBattleStart();
        }
	clientPartido->mKey = 0;
        clientPartido->mBattleScore = 0;
        clientPartido->mWaitingForAnswer = false;
        clientPartido->mQuestionString = "";
        //clientPartido->mShapePartido->mCollidable = false;
}
void Battle::execute(ClientPartido* clientPartido)
{
  	if (!clientPartido->mShapePartido->mOpponent)
        {
                clientPartido->mClientPartidoStateMachine->changeState(Game_Partido_Mode::Instance());
        }
}
void Battle::exit(ClientPartido* clientPartido)
{
}
bool Battle::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false;
}


/*****************************************
****        BATTLE STATES   ********   
****************************************/


/****************************************
	BATTLE_OFF	
****************************************/
BATTLE_OFF* BATTLE_OFF::Instance()
{
  static BATTLE_OFF instance;
  return &instance;
}
void BATTLE_OFF::enter(ClientPartido* clientPartido)
{
	if (clientPartido->id == 2)
	{	
        	LogString("BATTLE_OFF::enter");
	}
}
void BATTLE_OFF::execute(ClientPartido* clientPartido)
{
	if (clientPartido->mShapePartido)
	{
		if (!clientPartido->mWaitingForAnswer && clientPartido->mShapePartido->mOpponent)
		{
                	clientPartido->mBattleStateMachine->changeState(Sending_Question::Instance());
		}
	}
}
void BATTLE_OFF::exit(ClientPartido* clientPartido)
{
}

bool BATTLE_OFF::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false;  
}


/*****************************************
        Sending_Question
****************************************/
Sending_Question* Sending_Question::Instance()
{
  static Sending_Question instance;
  return &instance;
}
void Sending_Question::enter(ClientPartido* clientPartido)
{
	if (clientPartido->id == 2)
	{	
        	LogString("Sending_Question::enter");
	}
}
void Sending_Question::execute(ClientPartido* clientPartido)
{
  	if (!clientPartido->mShapePartido->mOpponent)
        {
                clientPartido->mBattleStateMachine->changeState(BATTLE_OFF::Instance());
        }

        if (clientPartido->mLoggedIn)
        {
                if (clientPartido->mShapePartido)
                {
                        if (clientPartido->mShapePartido->mOpponent && clientPartido->mWaitingForAnswer == false)
                        {
                                clientPartido->sendQuestion(clientPartido->getNewQuestionID());
                		clientPartido->mBattleStateMachine->changeState(Waiting_For_Answer::Instance());
                        }
                }
        }
	else //computer so just go right to wating...
	{
               	clientPartido->mBattleStateMachine->changeState(Waiting_For_Answer::Instance());
	}
}
void Sending_Question::exit(ClientPartido* clientPartido)
{
}
bool Sending_Question::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false; 
}


/*****************************************
        WAITING FOR ANSWER       
****************************************/
Waiting_For_Answer* Waiting_For_Answer::Instance()
{
  static Waiting_For_Answer instance;
  return &instance; 
} 
void Waiting_For_Answer::enter(ClientPartido* clientPartido)
{
	if (clientPartido->id == 2)
	{
        	LogString("WAITING_FOR_ANSWER::enter");
	}

	clientPartido->mComputerAskedTime = clientPartido->mServer->mGameTime;	

 	int randomAnswerTime = rand() % 3000;
 	clientPartido->mComputerAnswerTime = randomAnswerTime;

        clientPartido->mWaitingForAnswer = true;
}       
void Waiting_For_Answer::execute(ClientPartido* clientPartido)
{
  	if (!clientPartido->mShapePartido->mOpponent)
        {
                clientPartido->mBattleStateMachine->changeState(BATTLE_OFF::Instance());
        }
	
	if (!clientPartido->mWaitingForAnswer && clientPartido->mShapePartido->mOpponent)
	{
        	clientPartido->mBattleStateMachine->changeState(Sending_Question::Instance());
	}
	
	//mServerPartido->mAnswerVector.at(mQuestionID)
	if (!clientPartido->mLoggedIn) //send to readAnswer(int,string)
	{
		if (clientPartido->mComputerAskedTime + clientPartido->mComputerAnswerTime < clientPartido->mServer->mGameTime)
		{		
			clientPartido->readAnswer(clientPartido->mComputerAnswerTime,clientPartido->mServerPartido->mQuestionVector.at(clientPartido->mQuestionID)->answer);
		}
	}
	
}
void Waiting_For_Answer::exit(ClientPartido* clientPartido)
{

}
bool Waiting_For_Answer::onLetter(ClientPartido* clientPartido, Letter* letter)
{
        return false;   
}
