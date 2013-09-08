//header
#include "gamePartido.h"

//log
#include "../../tdreamsock/dreamSockLog.h"

//ObjectTitle
#include "../../billboard/objectTitle.h"

//byteBuffer
#include "../../bytebuffer/byteBuffer.h"

//shape
#include "../../shape/shape.h"

//application
#include "../../application/partido/applicationPartido.h"

//states
#include "states/gamePartidoStates.h"

/***************************************
*			          CONSTRUCTORS
***************************************/
GamePartido::GamePartido(ApplicationPartido* applicationPartido) : Game(applicationPartido)
{
	mApplicationPartido = applicationPartido;

	//battle
	mBattleStart = false;
	mBattleEnd   = false;
	mFirstTimeExecute = true;

	//correctAnswer
	mCorrectAnswerStart = false;
	mCorrectAnswer = false;
	mCorrectAnswerEnd = false;

	//states
        mPartidoStateMachine =  new StateMachine<GamePartido>(this);
        mPartidoStateMachine->setCurrentState      (PLAY_PARTIDO_GAME::Instance());
        mPartidoStateMachine->setPreviousState     (PLAY_PARTIDO_GAME::Instance());
        mPartidoStateMachine->setGlobalState       (GLOBAL_PARTIDO_GAME::Instance());

}

GamePartido::~GamePartido()
{
}

void GamePartido::processUpdate()
{
	Game::processUpdate();
        mPartidoStateMachine->update();
}

void GamePartido::checkByteBuffer(ByteBuffer* byteBuffer)
{
	Game::checkByteBuffer(byteBuffer);

        byteBuffer->BeginReading();

        int type = byteBuffer->ReadByte();
       
	switch(type)
        {
                case mMessageAskQuestion:
                        askQuestion(byteBuffer);
                        break;

                case mMessageBattleStart:
			mBattleStart = true;
                        break;

                case mMessageBattleEnd:
			mBattleEnd = true;
                        break;
                
		case mMessageCorrectAnswerStart:
			mCorrectAnswerStart = true;
                        correctAnswer(byteBuffer);
                        break;
		
		case mMessageCorrectAnswerEnd:
			mCorrectAnswerEnd = true;
                        break;
        }
}

void GamePartido::correctAnswer(ByteBuffer* byteBuffer)
{
        mApplicationPartido->mStringCorrectAnswer.clear();

        int length = byteBuffer->ReadByte();
        for (int i = 0; i < length; i++)
        {
                char c =  byteBuffer->ReadByte();
                mApplicationPartido->mStringCorrectAnswer.append(1,c);
        }
        if (mApplicationPartido->mLabelCorrectAnswer && mApplicationPartido->mStringCorrectAnswer.size() > 0)
        {
                mApplicationPartido->mLabelCorrectAnswer->setCaption(mApplicationPartido->mStringCorrectAnswer);
        }
        else
        {
                LogString("no label or no string");
        }
}

void GamePartido::askQuestion(ByteBuffer* byteBuffer)
{
	mApplicationPartido->mStringQuestion.clear();
        int length = byteBuffer->ReadByte();
        for (int i = 0; i < length; i++)
        {
        	char c =  byteBuffer->ReadByte();
                mApplicationPartido->mStringQuestion.append(1,c);
        }
	if (mApplicationPartido->mLabelQuestion && mApplicationPartido->mStringQuestion.size() > 0)
	{
		mApplicationPartido->mLabelQuestion->setCaption(mApplicationPartido->mStringQuestion);
	}
	else
	{
		LogString("no label or no string");
	}
	
	//reset mAnswerTime	
	mApplicationPartido->mAnswerTime = 0; 
}

