#ifndef MY_PARTICLE_H
#define MY_PARTICLE_H

/*

    This is a sample system. It accepts the command "read" followed by the
    path to an OBJ model.

*/

#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include <vector>
#include "Spring.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shared/opengl.h"

// a sample system
class Particle 
{

public:
    Particle();
    Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, const float m);
    glm::vec3 getPos();
    glm::vec3 getVel();
    float getMass();
    void setPos(glm::vec3 pos);
    void setVel(glm::vec3 vel);
    void addSpringI(Spring* s);
    void addSpringJ(Spring* s);
    std::vector<Spring*> getSpringsI();
    std::vector<Spring*> getSpringsJ();
    void reset(double time);

protected:
    glm::vec3 p_pos;
    glm::vec3 p_pos0;
    glm::vec3 p_vel;
    glm::vec3 p_vel0;
    float mass;
    std::vector<Spring*> attachedToI;
    std::vector<Spring*> attachedToJ;

};
#endif
