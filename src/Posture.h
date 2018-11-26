// Posture.h
// Author: Bairuo

#ifndef POSTURE_H_
#define POSTURE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Posture
{
public:
    glm::vec3 transVec3;
    float x, y, z, scale;

    Posture(float x, float y, float z, float s);
    Posture(glm::vec3 vec, float s);
    Posture();
    glm::mat4 GetMatrix();
    static Posture Lerp(const Posture &start, const Posture &end, float k);

private:
    glm::mat4 transMatrix = glm::mat4(1);
    glm::mat4 scaleMatrix = glm::mat4(1);
};

#endif // POSTURE_H_
