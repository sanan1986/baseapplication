//header
#include "gamePartidoStates.h"

//log
#include "../../../tdreamsock/dreamSockLog.h"

//applicationPartido
#include "../../../application/partido/applicationPartido.h"

//gamePartido
#include "../gamePartido.h"

//shape
#include "../../../shape/shape.h"

//utility
#include <math.h>


/******************** GLOBAL_PARTIDO_GAME *****************/

GLOBAL_PARTIDO_GAME* GLOBAL_PARTIDO_GAME::Instance()
{
  static GLOBAL_PARTIDO_GAME instance;
  return &instance;
}
void GLOBAL_PARTIDO_GAME::enter(GamePartido* gamePartido)
{

}
void GLOBAL_PARTIDO_GAME::execute(GamePartido* gamePartido)
{
}
void GLOBAL_PARTIDO_GAME::exit(GamePartido* gamePartido)
{
}
bool GLOBAL_PARTIDO_GAME::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}

/******************** PLAY_PARTIDO_GAME *****************/

PLAY_PARTIDO_GAME* PLAY_PARTIDO_GAME::Instance()
{
        static PLAY_PARTIDO_GAME instance;
        return &instance;
}
void PLAY_PARTIDO_GAME::enter(GamePartido* gamePartido)
{
	LogString("PLAY_PARTIDO_GAME::enter");
}
void PLAY_PARTIDO_GAME::execute(GamePartido* gamePartido)
{
	//give precedence to reset
        if (gamePartido->mApplicationPartido->mGameReset)
        {
                gamePartido->mPartidoStateMachine->changeState(RESET_PARTIDO_GAME::Instance());
        }
}
void PLAY_PARTIDO_GAME::exit(GamePartido* gamePartido)
{
}
bool PLAY_PARTIDO_GAME::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}

/******************** RESET_PARTIDO_GAME *****************/

RESET_PARTIDO_GAME* RESET_PARTIDO_GAME::Instance()
{
  static RESET_PARTIDO_GAME instance;
  return &instance;
}
void RESET_PARTIDO_GAME::enter(GamePartido* gamePartido)
{
	LogString("RESET_PARTIDO_GAME::enter");
	gamePartido->reset();
}
void RESET_PARTIDO_GAME::execute(GamePartido* gamePartido)
{
	if (!gamePartido->mApplicationPartido->mGameReset)
        {
                gamePartido->mPartidoStateMachine->changeState(PLAY_PARTIDO_GAME::Instance());
        }

}
void RESET_PARTIDO_GAME::exit(GamePartido* gamePartido)
{
}
bool RESET_PARTIDO_GAME::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}

/******************** BATTLE_OFF *****************/

BATTLE_OFF* BATTLE_OFF::Instance()
{
  static BATTLE_OFF instance;
  return &instance;
}
void BATTLE_OFF::enter(GamePartido* gamePartido)
{
	LogString("RESET_PARTIDO_GAME::enter");
	gamePartido->reset();
}
void BATTLE_OFF::execute(GamePartido* gamePartido)
{
 	if (gamePartido->mBattleStart)
        {
                gamePartido->mPartidoStateMachine->changeState(ANSWER_QUESTION::Instance());
        }
}
void BATTLE_OFF::exit(GamePartido* gamePartido)
{
}
bool BATTLE_OFF::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}

/******************** ANSWER_QUESTION *****************/

ANSWER_QUESTION* ANSWER_QUESTION::Instance()
{
  static ANSWER_QUESTION instance;
  return &instance;
}
void ANSWER_QUESTION::enter(GamePartido* gamePartido)
{
	LogString("ANSWER_QUESTION::enter");
	//going into battle then right into play but server still thinks your in battle...        
	ApplicationPartido* app = gamePartido->mApplicationPartido;
        app->createBattleScreen();
        app->showBattleScreen();

        gamePartido->mBattleStart = false;
        //gamePartido->mBattleEnd = false;
        gamePartido->mCorrectAnswerStart = false;
        gamePartido->mApplicationPartido->mGameReset = false;

        app->mAnswerTime = 0;

        //set mKeyArray to false
        for (int i = 0; i < 128; i++)
        {
                gamePartido->mApplicationPartido->mKeyArray[i] = false;
        }
        gamePartido->mFirstTimeExecute = true;
}
void ANSWER_QUESTION::execute(GamePartido* gamePartido)
{
        ApplicationPartido* app = gamePartido->mApplicationPartido;

        if (gamePartido->mCorrectAnswerStart)
        {
		LogString("mCorrectAnswerStart");
                gamePartido->mPartidoStateMachine->changeState(SHOWCORRECTANSWER_PARTIDO_GAME::Instance());
        }

        if (gamePartido->mBattleEnd)
        {
		LogString("mBattleEnd");
                gamePartido->mPartidoStateMachine->changeState(BATTLE_OFF::Instance());
        }

	if (app->mLabelFocus == app->mLabelAnswer)
        {
                if (app->mKeyArray[8]) //backspace
                {
                        app->mKeyArray[8] = false;
                        int size = app->mStringAnswer.size();
                        if (size > 0)
                        {
                                app->mStringAnswer.resize(size - 1);
                        }
                        app->mLabelAnswer->setCaption(app->mStringAnswer);
                }

                for (int i = 47; i < 123; i++)
                {
                        if (app->mKeyArray[i])
                        {
                                app->mKeyArray[i] = false;
                                char ascii = (char)i;
                                app->mStringAnswer.append(1,ascii);
                                app->mLabelAnswer->setCaption(app->mStringAnswer);
                        }
                }

                if (app->mKeyArray[13]) //enter
                {
                        app->mKeyArray[13] = false;
                        //LogString("sendAnswer via enter");
                        app->sendAnswer();
                        app->mStringQuestion.clear();
                        app->mStringAnswer.clear();
                        app->mLabelQuestion->setCaption("");
                        app->mLabelAnswer->setCaption("");
                }

                //set mKeyArray to false
                for (int i = 0; i < 128; i++)
                {
                        gamePartido->mApplicationPartido->mKeyArray[i] = false;
                }
        }
        
	for (int i = 0; i < 128; i++)
        {
                gamePartido->mApplicationPartido->mKeyArray[i] = false;
       	}
        gamePartido->mFirstTimeExecute = false;
}

void ANSWER_QUESTION::exit(GamePartido* gamePartido)
{
	gamePartido->mApplicationPartido->hideBattleScreen();
        gamePartido->mBattleEnd   = false;
        gamePartido->mBattleStart = false;

        //reset text box
        gamePartido->mApplicationPartido->mStringQuestion.clear();
        gamePartido->mApplicationPartido->mStringAnswer.clear();
        gamePartido->mApplicationPartido->mLabelQuestion->setCaption("");
        gamePartido->mApplicationPartido->mLabelAnswer->setCaption("");

}
bool ANSWER_QUESTION::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}

/******************** SHOWCORRECTANSWER_PARTIDO_GAME *****************/

SHOWCORRECTANSWER_PARTIDO_GAME* SHOWCORRECTANSWER_PARTIDO_GAME::Instance()
{
  static SHOWCORRECTANSWER_PARTIDO_GAME instance;
  return &instance;
}
void SHOWCORRECTANSWER_PARTIDO_GAME::enter(GamePartido* gamePartido)
{
	LogString("SHOWCORRECTANSWER_PARTIDO_GAME::enter");
        ApplicationPartido* app = gamePartido->mApplicationPartido;
        app->createCorrectAnswerScreen();
        app->showCorrectAnswerScreen();
        gamePartido->mCorrectAnswerStart = false;
}
void SHOWCORRECTANSWER_PARTIDO_GAME::execute(GamePartido* gamePartido)
{
	ApplicationPartido* app = gamePartido->mApplicationPartido;

	//again give precedence to a reset...
/*
        if (gamePartido->mApplicationPartido->mGameReset)
        {
                gamePartido->mPartidoStateMachine->changeState(RESET_PARTIDO_GAME::Instance());
        }
*/
        if (gamePartido->mCorrectAnswerEnd || gamePartido->mBattleEnd)
        {
                gamePartido->mPartidoStateMachine->changeState(ANSWER_QUESTION::Instance());
        }
        if (gamePartido->mBattleEnd)
        {
		LogString("mBattleEnd");
                gamePartido->mPartidoStateMachine->changeState(BATTLE_OFF::Instance());
        }
}
void SHOWCORRECTANSWER_PARTIDO_GAME::exit(GamePartido* gamePartido)
{
	gamePartido->mApplicationPartido->hideCorrectAnswerScreen();
        gamePartido->mCorrectAnswerStart = false;
        gamePartido->mCorrectAnswer      = false;
        gamePartido->mCorrectAnswerEnd   = false;

        //reset text box
        gamePartido->mApplicationPartido->mStringCorrectAnswer.clear();
        gamePartido->mApplicationPartido->mLabelCorrectAnswer->setCaption("");

}
bool SHOWCORRECTANSWER_PARTIDO_GAME::onLetter(GamePartido* gamePartido, Letter* letter)
{
        return true;
}



