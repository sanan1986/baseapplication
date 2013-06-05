//header
#include "gamePlayPartidoBattle.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//application
#include "../../application/applicationPartido.h"

//game
#include "../gamePartido.h"

//state machine
#include "../../../statemachine/stateMachine.h"

/***************************************
*	CONSTRUCTORS		          
***************************************/
GamePlayPartidoBattle::GamePlayPartidoBattle(GamePartido* gamePartido)
{
	mGamePartido = gamePartido;
}

GamePlayPartidoBattle::~GamePlayPartidoBattle()
{

}

void GamePlayPartidoBattle::enter()
{
	LogString("GamePlayPartidoBattle::enter");
	mGamePartido->mApplicationPartido->createBattleScreen();
	mGamePartido->mApplicationPartido->showBattleScreen();
	mGamePartido->mBattleStart = false;

	//reset text box 
	mGamePartido->mApplicationPartido->mStringAnswer.clear();
        mGamePartido->mApplicationPartido->mLabelAnswer->setCaption("");
}

void GamePlayPartidoBattle::execute()
{
 	if (mGamePartido->mBattleEnd)
        {
		LogString("battle end!!!");
                mGamePartido->mStateMachine->changeState(mGamePartido->mGamePlay); 
        }
}

void GamePlayPartidoBattle::exit()
{
	mGamePartido->mApplicationPartido->hideBattleScreen();
	mGamePartido->mBattleEnd   = false;
	mGamePartido->mBattleStart = false;
}
