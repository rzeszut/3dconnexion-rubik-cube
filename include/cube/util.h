#ifndef UTIL_H
#define UTIL_H

#include "glm/glm.hpp"

namespace cube {

#define PI  3.14159265359f

#define ELEMENTS_OF(array)       (sizeof(array)/sizeof(array[0]))
#define minmax(min_, x, max_)    (min(max(min_,x), max_))


inline float toRads(float fDegrees) {
    return fDegrees*2*PI/360;
}

inline float toDegs(float fRadians) {
    return fRadians*360/(2*PI);
}

int lineTest(const glm::vec2& ptLineStart, const glm::vec2& ptLineEnd, const float ptX, const float ptY);
bool poly4InsideTest(glm::vec2 *pt2Corners, float ptX, float ptY);
bool poly4InsideTest(glm::vec3 *pt3Corners, float ptX, float ptY);

int getYsection(glm::vec2 *ptCorners, float ptX, float ptY);
int getYsection(glm::vec3 *pt3Corners, float ptX, float ptY);
int getXsection(glm::vec2 *ptCorners, float ptX, float ptY);
int getXsection(glm::vec3 *pt3Corners, float ptX, float ptY);

}

#endif //UTIL_H
