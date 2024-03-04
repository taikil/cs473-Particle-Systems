#ifndef MY_PARTICLE_H
#define MY_PARTICLE_H

/*

    This is a sample system. It accepts the command "read" followed by the
    path to an OBJ model.

*/

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shared/opengl.h"

// a sample system
class Particle : public BaseSystem
{

public:
    Particle(const std::string& name);
    Particle(const std::string& name, const glm::vec3& initialPosition, const glm::vec3& initialVelocity, const float m);
    virtual void getState(double* p);
    virtual void setState(double* p);
    glm::vec3 getPos();
    glm::vec3 getVel();
    void setPos(glm::vec3 pos);
    void setVel(glm::vec3 vel);
    void reset(double time);

    void display(GLenum mode = GL_RENDER);

    void readModel(char* fname) { m_model.ReadOBJ(fname); }
    int command(int argc, myCONST_SPEC char** argv);

protected:

    float m_sx;
    float m_sy;
    float m_sz;

    glm::vec3 p_pos;
    glm::vec3 p_pos0;
    glm::vec3 p_vel;
    glm::vec3 p_vel0;
    float mass;

    GLMmodel m_model;

};
#endif
