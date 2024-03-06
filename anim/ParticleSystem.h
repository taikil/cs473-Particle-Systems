#ifndef MY_PARTICLE_SYS_H
#define MY_PARTICLE_SYS_H

/*

    This is a sample system. It accepts the command "read" followed by the
    path to an OBJ model.

*/

#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>
#include "Particle.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shared/opengl.h"

// a sample system
class ParticleSystem : public BaseSystem
{

public:
    ParticleSystem(const std::string& name);
    virtual void getState(double* p);
    virtual void setState(double* p);
    int getNumParticles();
    glm::vec3 getParticlePos(int index);
    glm::vec3 getParticleVel(int index);
    float getParticleMass(int index);
    void setParticlePos(int index, glm::vec3 pos);
    void setParticleVel(int index, glm::vec3 vel);
    void reset(double time);
    void addSpring(int i, int j);

    void display(GLenum mode = GL_RENDER);
    void displaySpring();

    void readModel(char* fname) { m_model.ReadOBJ(fname); }
    void flipNormals(void) { glmReverseWinding(&m_model); }
    int command(int argc, myCONST_SPEC char** argv);

protected:

    float m_sx;
    float m_sy;
    float m_sz;

    glm::vec3 p_pos;
    glm::vec3 p_vel;

    int numParticles = 0;
    std::vector<int> verticesi;
    std::vector<int> verticesj;


    GLMmodel m_model;
    std::vector<Particle> particles; 

};
#endif
