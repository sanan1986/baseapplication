#ifndef TEST_H
#define TEST_H

#include "../../fsm/stateMachine.h"

#include "../../message/message.h"

#include <string>
#include <vector>


class QuestionAttempts;
class Message;
class ClientPartido;
class Quiz;
class Utility;

class Test 
{
public:
	
	Test(ClientPartido* clientPartido);
	~Test();

void reset();

void update();
StateMachine<Test>* mStateMachine;



//client
ClientPartido* mClientPartido;

//message
Message mMessage;

//questions
std::string mQuestionString;
int mQuestionID;
int getNewQuestionID();
std::vector<QuestionAttempts*> mQuestionAttemptsVector;
std::vector<QuestionAttempts*> mQuestionAttemptsVectorTemp;
int getLowestUnpassedLevel(int maxLevel);
void getQuestionAttempts();
void sendQuestion(int questionID);

//answer
bool mShowCorrectAnswer;
void insertAnswerAttempt(int questionID, std::string stringAnswer, int answerTime);

//showCorrectAnswer
void sendCorrectAnswer(int questionID);

//level
int  getMaxLevelAskedID();
bool checkLevel(int level);

int mRandom;
int mCurrent;
int mAdvance;

Utility* mUtility;
};

#endif
