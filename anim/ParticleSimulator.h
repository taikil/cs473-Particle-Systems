#ifndef MY_PARTICLE_SIMULATOR_H
#define MY_PARTICLE_SIMULATOR_H

#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include <vector>

#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "Spring.h"
#include "ResourceManager.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

enum IntegrationMethod {
	FORWARD_EULER,
	SYMPLECTIC_EULER,
	VERLET
};

class ParticleSimulator : public BaseSimulator
{
public:

	ParticleSimulator(const std::string& name, ParticleSystem* target);
	~ParticleSimulator();
	void addParticle(float mass, glm::vec3 position, glm::vec3 velocity);
	void addSpring(int particleIndex1, int particleIndex2, float ks, float kd, float restLength);
	void setIntegrationMethod(IntegrationMethod method) { integrationMethod = method; }
	void setGravity(glm::vec3 gravity);
	void setGroundParameters(float ks, float kd);
	void setTimeStep(float dt);

	glm::vec3 springForce(glm::vec3 posi, glm::vec3 posj, glm::vec3 springParams);
	glm::vec3 damperForce(glm::vec3 posi, glm::vec3 veli, glm::vec3 posj, glm::vec3 velj, glm::vec3 springParams);
	glm::vec3 integrateVelocity(glm::vec3 posi, glm::vec3 veli, float dt, float time);
	int step(double time);
	int init(double time)
	{
		return 0;
	};

	int command(int argc, myCONST_SPEC char** argv);

protected:

	glm::vec3 m_pos0; // initial position
	glm::vec3 m_vel0; // initial velocity
	glm::vec3 m_pos;
	glm::vec3 m_vel;

	std::vector<Spring> springs;
	IntegrationMethod integrationMethod = FORWARD_EULER;
	float timeStep = 0.01;
	glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
	float groundKs;
	float groundKd;


	ParticleSystem* particles;

};


#endif