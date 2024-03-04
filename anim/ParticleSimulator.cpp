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

glm::vec3 ParticleSimulator::computeGravity(glm::vec3 posi, glm::vec3 veli, float dt, float time)
{
	glm::vec3 pos;
	pos[0] = posi[0] + veli[0] * dt;
	pos[1] = posi[1] + veli[1] * dt + 0.5 * gravity * time * time;
	pos[2] = posi[2] + veli[2] * dt;
	return pos;
}

int ParticleSimulator::step(double time)
{

	animTcl::OutputMessage("dt: %.3f", timeStep);
	int numP = particles->getNumParticles();

	for (int i = 0; i < numP; i++) {
		m_pos0 = particles->getParticlePos(i);
		m_vel0 = particles->getParticleVel(i);
		animTcl::OutputMessage("Pos:  %.3f %.3f %.3f ", m_pos0.x, m_pos0.y, m_pos0.z);
		m_pos = computeGravity(m_pos0, m_vel0, timeStep, time + timeStep);
			
		particles->setParticlePos(i, m_pos);
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
	else
	{
		animTcl::OutputMessage("Unkown command %s", argv[0]);
	}
}
