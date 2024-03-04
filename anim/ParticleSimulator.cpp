#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(const std::string& name, ParticleSystem* target) :
	BaseSimulator(name),
	particles(target)
{
}	// ParticleSystem

ParticleSimulator::~ParticleSimulator()
{
}	// ParticleSystem::~ParticleSystem

void ParticleSimulator::setTimeStep(float dt) {
	SIMULATION_TIME_STEP = dt; //ResourceManager.h
	timeStep = dt;
}

glm::vec3 ParticleSimulator::integrateVelocity(glm::vec3 posi, glm::vec3 veli, float dt, float time)
{

	glm::vec3 pos = posi;

	switch (integrationMethod) {
	case FORWARD_EULER:
		pos[0] = posi[0] + veli[0] * dt;
		pos[1] = posi[1] + veli[1] * dt;
		pos[2] = posi[2] + veli[2] * dt;
		break;

	case SYMPLECTIC_EULER:
		break;

	case VERLET:
		break;

	default:
		break;
	}
	return pos;
}

glm::vec3 springForce() {

}

int ParticleSimulator::step(double time)
{

	animTcl::OutputMessage("dt: %.3f", timeStep);
	int numP = particles->getNumParticles();

	for (int i = 0; i < numP; i++) {
		m_pos0 = particles->getParticlePos(i);
		m_vel0 = particles->getParticleVel(i);
		float particleMass = particles->getParticleMass(i);

		glm::vec3 totalForce = glm::vec3(0.0f, 0.0f, 0.0f);
		// Sum of Spring Forces
		for (int j = 0; i < springs.size(); i++) {
			glm::vec2 indices = springs[i].getIndices();
			//Compute fij
			if (indices[0] == i) {
				glm::vec3 coef = springs[i].getCoefficients();
			}
			// Compute fji
			else if (indices[1] == i) { 
				glm::vec3 coef = springs[i].getCoefficients();
			}
		}
		//Velocity
		totalForce += integrateVelocity(m_pos0, m_vel0, timeStep, time);
		//Gravity
		totalForce += (gravity * particleMass);

		// Divide total force by particle mass to get acceleration
		glm::vec3 acceleration = totalForce / particleMass; // Assuming particleMass is a member variable

		// Update velocity using the acceleration
		m_vel = m_vel0 + acceleration * timeStep;

		// Update position using the updated velocity
		m_pos = m_pos0 + m_vel * timeStep;

		// Set the new position and velocity back to the particle
		particles->setParticlePos(i, m_pos);
		particles->setParticleVel(i, m_vel);

	}

	return 0;

}	// ParticleSystem::step


int ParticleSimulator::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	if (argc < 2)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}

	const char* sysName = argv[1];

	if (strcmp(argv[0], "integration") == 0)
	{
		if (argc == 3)
		{
			const char* integrationType = argv[1];
			float timeStep = static_cast<float>(atof(argv[2]));

			setTimeStep(timeStep);
			if (strcmp(integrationType, "euler") == 0)
			{
				setIntegrationMethod(FORWARD_EULER);
				return TCL_OK;
			}
			else if (strcmp(integrationType, "symplectic") == 0)
			{
				setIntegrationMethod(SYMPLECTIC_EULER);
				return TCL_OK;
			}
			else if (strcmp(integrationType, "verlet") == 0)
			{
				setIntegrationMethod(VERLET);
				return TCL_OK;
			}
			else
			{
				animTcl::OutputMessage("Unknown integration method: %s", integrationType);
				return TCL_ERROR;
			}
		}
		else
		{
			animTcl::OutputMessage("Usage: integration <euler|symplectic|verlet> <time_step>");
			return TCL_ERROR;
		}

		glutPostRedisplay();
		return TCL_OK;
	}
	else if (strcmp(argv[0], "spring") == 0) {
		if (argc == 6)
		{
			int index1 = atoi(argv[1]);
			int index2 = atoi(argv[2]);
			float ks = static_cast<float>(atof(argv[3]));
			float kd = static_cast<float>(atof(argv[4]));
			float restLength = static_cast<float>(atof(argv[5]));

			springs.push_back(Spring(index1, index2, restLength, ks, kd));
			//particles[index1];

			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: simulator <sim_name> spring <index1> <index2> <ks> <kd> <restlength>");
			return TCL_ERROR;
		}
	}
	else
	{
		animTcl::OutputMessage("Unkown command %s", argv[0]);
	}
}
