#ifndef BATTLE_H
#define BATTLE_H

#include <vector>

using namespace std;

class Shape;
class GamePartido;

class Battle 
{
public:
	
	Battle(GamePartido* game, std::vector<Shape*> shapeVector);
	~Battle();

//shapes
std::vector<Shape*> mShapeVector;       //every shape in the battle..

void processUpdate();
int getQuestionLevelID(int userID);

bool mOver;
	
GamePartido* mGame; 
	
};

#endif
