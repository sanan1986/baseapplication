#ifndef TEST_H
#define TEST_H

#include "../../fsm/stateMachine.h"

#include "../message/message.h"

#include <string>
#include <vector>


class QuestionAttempts;
class Message;
class ClientPartido;
class Quiz;

class Test 
{
public:
	
	Test(ClientPartido* clientPartido);
	~Test();

void update();
StateMachine<Test>* mStateMachine;

//client
ClientPartido* mClientPartido;

//quiz
Quiz* mQuiz;
Quiz* mQuizLast;

//message
Message mMessage;

//questions
int getNewQuestionID();
std::vector<QuestionAttempts*> mQuestionAttemptsVector;
std::vector<QuestionAttempts*> mQuestionAttemptsVectorTemp;
int getLowestUnpassedLevel(int maxLevel);
void getQuestionAttempts();

//answer
void insertAnswerAttempt(int questionID, std::string stringAnswer, int answerTime);
void parseAnswer(Message* message);

//level
int  getMaxLevelAskedID();
bool checkLevel(int level);

};

#endif