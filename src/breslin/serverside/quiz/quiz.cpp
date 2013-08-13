#include "quiz.h"
#include "states/quizStates.h"
#include "../client/robust/partido/clientPartido.h"
#include "../server/partido/serverPartido.h"
#include "../question/question.h"
#include "../network/network.h"
#include "../../utility/utility.h"
#include "../question/questionAttempts.h"

Quiz::Quiz(ClientPartido* clientPartido)
{
	mClientPartido = clientPartido;
	getQuestionAttempts();
 
	//quiz states
        mStateMachine =  new StateMachine<Quiz>(this);
        mStateMachine->setCurrentState      (INIT_QUIZ::Instance());
        mStateMachine->setPreviousState     (INIT_QUIZ::Instance());
        mStateMachine->setGlobalState       (GLOBAL_QUIZ::Instance());

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
        if (mClientPartido->mConnectionState == DREAMSOCK_CONNECTED)
        {
                mMessage.Init(mMessage.outgoingData, sizeof(mMessage.outgoingData));
                mMessage.WriteByte(mClientPartido->mServerPartido->mMessageAskQuestion); // add type

                if (mClientPartido->mClientID > 0)
                {
                        mMessage.WriteByte(mClientPartido->mClientID); // add mClientID for browsers
                }
                int length = mClientPartido->mServerPartido->mQuestionVector.at(questionID)->question.length();
                mMessage.WriteByte(length);

                //loop thru length and write it
                for (int i=0; i < length; i++)
                {
                        mMessage.WriteByte(mClientPartido->mServerPartido->mQuestionVector.at(questionID)->question.at(i));
                }

                //send it
                mClientPartido->mServerPartido->mNetwork->sendPacketTo(mClientPartido,&mMessage);
        }
}

void Quiz::parseAnswer(Message* mes)
{
        int answerTime = mes->ReadShort();
        int sizeOfAnswer = mes->ReadByte();

        std::string answer;

        //loop thru and set stringAnswer
        for (int i = 0; i < sizeOfAnswer; i++)
        {
                if (mClientPartido->mClientID > 0)
                {
                        char c = mes->ReadByte();
                        answer.append(1,c);
                }
                else
                {
                        int numeric = mes->ReadByte();
                        char ascii = (char)numeric;
                        answer.append(1,ascii);
                }
        }

        readAnswer(answerTime,answer);

}


int Quiz::getNewQuestionID()
{
        int maxLevel            = getMaxLevelAskedID();

        if (rand() % 2 == 1)
        {
                //you could advance here if you passed maxLevel and all before it...
                for (int i = 1; i <= maxLevel; i++)
                {
                        if (checkLevel(i))
                        {
                                if (i == maxLevel)
                                {
                                        //advance level
                                        mQuestionID = maxLevel + 1;
                                        return mQuestionID;
                                }
                        }
                        else
                        {
                                mQuestionID = i;
                                return mQuestionID;
                        }
                }
        }
        else
        {
                //do it random based on maxLevel, you cannot advance a level here.
                mQuestionID  = rand() % maxLevel + 1;
                return mQuestionID;
        }
}


void Quiz::getQuestionAttempts()
{
        PGconn          *conn;
        PGresult        *res;
        int             rec_count;
        int             row;
        int             col;
        conn = PQconnectdb("dbname=abcandyou host=localhost user=postgres password=mibesfat");
        std::string query = "select id, question_id, answer, extract(epoch from answer_attempt_time), answer_time, user_id from questions_attempts WHERE user_id = ";
        query.append(mClientPartido->mServerPartido->mUtility->intToString(mClientPartido->id));
        query.append(" ORDER BY answer_attempt_time");
        const char * q = query.c_str();
        //LogString("q:%s",q);
        res = PQexec(conn,q);

        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
                puts("We did not get any data!");
        }
        else
        {
        }
        rec_count = PQntuples(res);

        for (row=0; row<rec_count; row++)
        {
                const char* a = PQgetvalue(res, row, 0);
                std::string aString(a);
                int aInt = mClientPartido->mServerPartido->mUtility->stringToInt(aString);

                const char* b = PQgetvalue(res, row, 1);
                std::string bString(b);
                int bInt = mClientPartido->mServerPartido->mUtility->stringToInt(bString);

                const char* c = PQgetvalue(res, row, 2);
                std::string cString(c);

                const char* d = PQgetvalue(res, row, 3);
                std::string dString(d);
                double dDouble = atof(dString.c_str());

                const char* e = PQgetvalue(res, row, 4);
                std::string eString(e);
                int eInt = mClientPartido->mServerPartido->mUtility->stringToInt(eString);

                const char* f = PQgetvalue(res, row, 5);
                std::string fString(f);
                int fInt = mClientPartido->mServerPartido->mUtility->stringToInt(fString);

                QuestionAttempts* questionAttempts = new QuestionAttempts(aInt,bInt,cString,dDouble,eInt,fInt);
                mQuestionAttemptsVector.push_back(questionAttempts);
        }
        PQclear(res);
        PQfinish(conn);
}

void Quiz::insertAnswerAttempt()
{
        QuestionAttempts* questionAttempts = new QuestionAttempts(0,mQuestionID,mStringAnswer,mClientPartido->mServerPartido->mNetwork->getCurrentSystemTime(),mAnswerTime,mClientPartido->id);
        mQuestionAttemptsVector.push_back(questionAttempts);
}

int Quiz::getMaxLevelAskedID()
{
        int highestQuestionID = 1;

        for (int i = 0; i < mQuestionAttemptsVector.size(); i++)
        {
                if (mQuestionAttemptsVector.at(i)->question_id > highestQuestionID)
                {
                        highestQuestionID = mQuestionAttemptsVector.at(i)->question_id;
                }
        }
        return highestQuestionID;
}

bool Quiz::checkLevel(int level)
{
        int total = 0;
        int index = mQuestionAttemptsVector.size() - 1;
        //clear the vector...
        mQuestionAttemptsVectorTemp.clear();

        while (total < 10 && index > -1)
        {
                if (mQuestionAttemptsVector.at(index)->question_id == level)
                {
                        mQuestionAttemptsVectorTemp.push_back(mQuestionAttemptsVector.at(index));
                        total++;
                }
                index--;
        }

        if (mQuestionAttemptsVectorTemp.size() < 10)
        {
                //LogString("size is small: %d", mQuestionAttemptsVectorTemp.size());
        }
        else
        {
                //LogString("size is good: %d", mQuestionAttemptsVectorTemp.size());
        }

        return false;
}

void Quiz::readAnswer(int answerTime, std::string answer)
{
/*
        //clear answer string
        mStringAnswer.clear();

        mAnswerTime = answerTime;
        mStringAnswer = answer;

        insertAnswerAttempt();

        if (mStringAnswer.compare(mClientPartido->mServerPartido->mQuestionVector.at(mQuestionID)->answer) != 0 || mAnswerTime > 2000)
        {
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
                        // do nothing...

                }
        }
        else
        {
                mBattleScore++;
        }

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

        //set vars for new question and answer combo....
        mWaitingForAnswer = false;
        mQuestionString = "";
*/
}

