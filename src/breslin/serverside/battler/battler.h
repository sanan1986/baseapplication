#ifndef BATTLER_H
#define BATTLER_H

#include <vector>
using namespace std;

class Shape;
class Battle;

class Battler 
{
public:
	
	Battler(Battle* battle, Shape* shape);
	~Battler();

void processUpdate();
int getQuestionLevelID();
	
Battle* mBattle; 
Shape* mShape;
int mFirstUnmasteredQuestionID;	
std::vector<int*> mMasteredQuestionIDVector;    
};

#endif
