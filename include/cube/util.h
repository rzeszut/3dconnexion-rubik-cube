#include <glm/glm.hpp>

#ifndef UTIL_H
#define UTIL_H

#define PI  3.14159265359f

typedef signed char INT8;



#define ELEMENTS_OF(array)       (sizeof(array)/sizeof(array[0]))
#define minmax(min_, x, max_)    (min(max(min_,x), max_))


inline float toRads(float fDegrees)     {return fDegrees*2*PI/360;};
inline float toDegs(float fRadians)     {return fRadians*360/(2*PI);};


int lineTest(const glm::vec2& ptLineStart, const glm::vec2& ptLineEnd, const float ptX, const float ptY);
// bool instead of BOOL
bool poly4InsideTest(const glm::vec2 pt2Corners[], float ptX, float ptY);
bool poly4InsideTest(glm::vec3 pt3Corners[], float ptX, float ptY);   

// use const reference instead of *
// (float x, float y) or glm::vec2 insteadd of CPoint
int getYsection(glm::vec2 ptCorners[], float ptX, float ptY);
int getYsection(glm::vec3 pt3Corners[], float ptX, float ptY);
int getXsection(glm::vec2 ptCorners[], float ptX, float ptY);
int getXsection(glm::vec3 pt3Corners[], float ptX, float ptY);

#endif //UTIL_H
