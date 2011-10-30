#ifndef VECTOR3D_H
#define VECTOR3D_H

//Ogre headers
#include "Ogre.h"
using namespace Ogre;

class Vector3D
{

public:
Vector3D();
Vector3D(float x, float y, float z);
~Vector3D();

float length();

float x;
float y;
float z;

bool isZero();
void zero();

void normalise();

void multiply(float num);
void add     (Vector3D* vectorToAddtoThisOne);
void subtract(Vector3D* vectorToAddtoThisOne);
void copyValuesFrom(Vector3D* copyFrom);
float dot(Vector3D* v2);
Vector3D* crossProduct(Vector3D* b);
Vector3 convertToVector3();
};
#endif
