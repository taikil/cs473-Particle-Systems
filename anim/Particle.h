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
    virtual void getState(glm::vec3& position);
    virtual void setState(const glm::vec3& position);
    void reset(double time);

    void display(GLenum mode = GL_RENDER);

    void readModel(char* fname) { m_model.ReadOBJ(fname); }
    void flipNormals(void) { glmReverseWinding(&m_model); }
    int command(int argc, myCONST_SPEC char** argv);

protected:

    float m_sx;
    float m_sy;
    float m_sz;

    glm::vec3 p_pos;
    glm::vec3 p_vel;

    GLMmodel m_model;

};
#endif
