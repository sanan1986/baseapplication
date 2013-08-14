#include "quiz.h"
#include "states/quizStates.h"
#include "../client/robust/partido/clientPartido.h"
#include "../server/partido/serverPartido.h"
#include "../question/question.h"
#include "../network/network.h"
#include "../../utility/utility.h"
#include "../question/questionAttempts.h"
#include "../combatant/combatant.h"
#include "../test/test.h"

Quiz::Quiz(Combatant* combatant)
{
	mCombatant = combatant;

	//let this clients test know about this quiz
	mCombatant->mClientPartido->mTest->mQuiz = this; 
 
	//quiz states
        mStateMachine =  new StateMachine<Quiz>(this);
        mStateMachine->setCurrentState      (INIT_QUIZ::Instance());
        mStateMachine->setPreviousState     (INIT_QUIZ::Instance());
        mStateMachine->setGlobalState       (GLOBAL_QUIZ::Instance());

       	//question 
	mQuestionID = 0;
	mComputerAskedTime = 0;
        
	//answer	
	mAnswerTime = 0;
	mComputerAnswerTime = 0;
        mWaitingForAnswer = false;
}

Quiz::~Quiz()
{
}

void Quiz::update()
{
	mStateMachine->update();
}
void Quiz::sendQuestion(int questionID)
{
	if (mCombatant->mClientPartido->mConnectionState == DREAMSOCK_CONNECTED)
	{
        	mMessage.Init(mMessage.outgoingData, sizeof(mMessage.outgoingData));
        	mMessage.WriteByte(mCombatant->mClientPartido->mServerPartido->mMessageAskQuestion); // add type

        	if (mCombatant->mClientPartido->mClientID > 0)
        	{
                	mMessage.WriteByte(mCombatant->mClientPartido->mClientID); // add mClientID for browsers
        	}
       		int length = mCombatant->mClientPartido->mServerPartido->mQuestionVector.at(questionID)->question.length();  
        	mMessage.WriteByte(length); 

        	//loop thru length and write it
        	for (int i=0; i < length; i++)
        	{
                	mMessage.WriteByte(mCombatant->mClientPartido->mServerPartido->mQuestionVector.at(questionID)->question.at(i));
        	}

        	//send it
        	mCombatant->mClientPartido->mServerPartido->mNetwork->sendPacketTo(mCombatant->mClientPartido,&mMessage);
	}
}
void Quiz::readAnswer(int answerTime, std::string answer)
{

        //clear answer string
        mStringAnswer.clear();
        mAnswerTime = answerTime;
        mStringAnswer = answer;

        mCombatant->mClientPartido->mTest->insertAnswerAttempt(mQuestionID,mStringAnswer,mAnswerTime);

        //if (mStringAnswer.compare(mCombatant->mClientPartido->mServerPartido->mQuestionVector.at(mQuestionID)->answer) != 0 || mAnswerTime > 2000)
        if (mStringAnswer.compare(mCombatant->mClientPartido->mServerPartido->mQuestionVector.at(mQuestionID)->answer) != 0)
        {
		LogString("wrong");		
/*
                ShapePartido* opponent  = mShapePartido->mOpponent;
                if (opponent)
                {
                        //score battle
                        scoreBattle(LOSS);
                        opponent->mClientPartido->scoreBattle(WIN);

                        //set battle record text .. mBattleRecordText
                        setBattleRecordText();
                        opponent->mClientPartido->setBattleRecordText();

                        //set Text of shape .. mText
                        mShapePartido->setText(mBattleRecordText);
                        opponent->mClientPartido->mShapePartido->setText(opponent->mClientPartido->mBattleRecordText);

                        //reset battle
                        resetBattle();
                        opponent->mClientPartido->resetBattle();

                        //send battle end to client
                        sendBattleEnd();
                        opponent->mClientPartido->sendBattleEnd();
                }
                else //opponent took care of loss....
                {
                }
*/
        }
        else
        {
		LogString("correct");		
 //               mBattleScore++;
        }
/*
        if (mBattleScore > 9)
        {
                ShapePartido* opponent  = mShapePartido->mOpponent;

                //score battle
                scoreBattle(WIN);
                opponent->mClientPartido->scoreBattle(LOSS);

                //set battle record text .. mBattleRecordText
                setBattleRecordText();
                opponent->mClientPartido->setBattleRecordText();

                //set Text of shape .. mText
                mShapePartido->setText(mBattleRecordText);
                opponent->mClientPartido->mShapePartido->setText(opponent->mClientPartido->mBattleRecordText);

                //reset battle
                resetBattle();
                opponent->mClientPartido->resetBattle();

                //send battle end to client
                sendBattleEnd();
                opponent->mClientPartido->sendBattleEnd();
        }
*/
        //set vars for new question and answer combo....
        mWaitingForAnswer = false;
        mQuestionString = "";
}

