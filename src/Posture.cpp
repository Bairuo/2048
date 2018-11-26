#include<iostream>
#include "Posture.h"

Posture::Posture(float x, float y, float z, float s)
{
    transVec3 = glm::vec3(x, y, z);
    scale = s;
    this->x = x;
    this->y = y;
    this->z = z;

    transMatrix = glm::translate(transMatrix, transVec3);
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale));
}

Posture::Posture(glm::vec3 vec, float s):Posture(vec.x, vec.y, vec.z, s)
{

}

Posture::Posture():Posture(0, 0, 0, 1)
{

}

glm::mat4 Posture::GetMatrix()
{
    return transMatrix * scaleMatrix;
}

Posture Posture::Lerp(const Posture &start, const Posture &end, float k)
{
    float x = start.transVec3.x + (end.transVec3.x - start.transVec3.x) * k;
    float y = start.transVec3.y + (end.transVec3.y - start.transVec3.y) * k;
    float z = start.transVec3.z + (end.transVec3.z - start.transVec3.z) * k;
    float scale = start.scale + (end.scale - start.scale) * k;

    //std::cout << start.scale << ", " << end.scale << ", " << k << ", get " << scale << std::endl;

    return Posture(x, y, z, scale);
}
