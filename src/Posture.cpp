#include<iostream>
#include "Posture.h"

Posture::Posture(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ)
    :transVec3(posX, posY, posZ),
     scaleVec3(scaleX, scaleY, scaleZ),
     transMatrix(glm::translate(glm::mat4(1), transVec3)),
     scaleMatrix(glm::scale(glm::mat4(1), scaleVec3))
{

}


Posture::Posture(float x, float y, float z, float s)
    :Posture(x, y, z, s, s, s)
{

}

Posture::Posture(float x, float y, float z, glm::vec3 scaleVec3)
    :Posture(x, y, z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}


Posture::Posture(glm::vec3 transVec3, float s)
    :Posture(transVec3.x, transVec3.y, transVec3.z, s, s, s)
{

}

Posture::Posture(glm::vec3 transVec3, glm::vec3 scaleVec3)
    :Posture(transVec3.x, transVec3.y, transVec3.z, scaleVec3.x, scaleVec3.y, scaleVec3.z)
{

}


Posture::Posture()
    :Posture(0, 0, 0, 1, 1, 1)
{

}

float Posture::getPosX() const
{
    return transVec3.x;
}

float Posture::getPosY() const
{
    return transVec3.y;
}

float Posture::getPosZ() const
{
    return transVec3.z;
}

float Posture::getScaleX() const
{
    return scaleVec3.x;
}

float Posture::getScaleY() const
{
    return scaleVec3.y;
}

float Posture::getScaleZ() const
{
    return scaleVec3.z;
}

glm::mat4 Posture::getMatrix() const
{
    return transMatrix * scaleMatrix;
}

Posture Posture::Lerp(const Posture &start, const Posture &end, float k)
{
    float posX = start.transVec3.x + (end.transVec3.x - start.transVec3.x) * k;
    float posY = start.transVec3.y + (end.transVec3.y - start.transVec3.y) * k;
    float posZ = start.transVec3.z + (end.transVec3.z - start.transVec3.z) * k;

    float scaleX = start.scaleVec3.x + (end.scaleVec3.x - start.scaleVec3.x) * k;
    float scaleY = start.scaleVec3.y + (end.scaleVec3.y - start.scaleVec3.y) * k;
    float scaleZ = start.scaleVec3.z + (end.scaleVec3.z - start.scaleVec3.z) * k;

    return Posture(posX, posY, posZ, scaleX, scaleY, scaleZ);
}
