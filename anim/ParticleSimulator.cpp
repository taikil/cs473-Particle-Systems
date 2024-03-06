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

void ParticleSimulator::setGravity(float g)
{
	gravity.y = g;
}


glm::vec3 ParticleSimulator::springForce(glm::vec3 posi, glm::vec3 posj, glm::vec3 springParams) {
	float restLen = springParams[0];
	float ks = springParams[1];
	float kd = springParams[2];
	// | xi - xj |
	glm::vec3 delta = posi - posj;
	float posLen = glm::length(delta);

	// Check if the length is greater than a small threshold
	glm::vec3 normalized = (posLen > 0.0001f) ? glm::normalize(delta) : glm::vec3(0.0f);

	// ks(len - | xi - xj |) * (xi - xj / |xi - xj |)
	return ks * (restLen - posLen) * normalized;
}

glm::vec3 ParticleSimulator::damperForce(glm::vec3 posi, glm::vec3 veli, glm::vec3 posj, glm::vec3 velj, glm::vec3 springParams) {
	float restLen = springParams[0];
	float ks = springParams[1];
	float kd = springParams[2];
	// | xi - xj |
	glm::vec3 delta = posi - posj;
	float velLen = glm::length(veli - velj);

	// Check if the length is greater than a small threshold
	glm::vec3 normalized = (velLen > 0.0001f) ? glm::normalize(delta) : glm::vec3(0.0f);

	// ks(len - | xi - xj |) * (xi - xj / |xi - xj |)
	return -kd * (velLen * normalized) * normalized;
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

glm::vec3 ParticleSimulator::handleSprings(int i) {
	float particleMass = particles->getParticleMass(i);
	glm::vec3 total = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int j = 0; j < springs.size(); j++) {
		int indexI = springs[j].getI();
		int indexJ = springs[j].getJ();
		animTcl::OutputMessage("index:  %.3f %.3f %.3f ", indexI, indexJ);
		//Compute fij
		if (indexI == i || indexJ == i) {
			if (i == 2) {
				animTcl::OutputMessage("2!!!!");
			}
			glm::vec3 coef = springs[j].getCoefficients();
			glm::vec3 posi = particles->getParticlePos(indexI);
			glm::vec3 veli = particles->getParticleVel(indexI);
			glm::vec3 posj = particles->getParticlePos(indexJ);
			glm::vec3 velj = particles->getParticleVel(indexJ);

			glm::vec3 fs = springForce(posi, posj, coef);
			glm::vec3 fd = damperForce(posi, veli, posj, velj, coef);
			// if fji, negate force values
			indexJ == i ? fs = -fs : void(0);
			indexJ == i ? fd = -fd : void(0);
			total += fs;
			total += fd;
		}
	}
	return total;
}

int ParticleSimulator::step(double time)
{
	std::vector<glm::vec3> newPos;
	std::vector<glm::vec3> newVel;
	int numP = particles->getNumParticles();

	for (int i = 0; i < numP; i++) {
		glm::vec3 m_pos0; // initial position
		glm::vec3 m_vel0; // initial velocity
		glm::vec3 m_pos;
		glm::vec3 m_vel;

		m_pos0 = particles->getParticlePos(i);
		m_vel0 = particles->getParticleVel(i);
		float particleMass = particles->getParticleMass(i);

		glm::vec3 totalForce = glm::vec3(0.0f, 0.0f, 0.0f);
		// Sum of Spring Forces
		totalForce += handleSprings(i);
		animTcl::OutputMessage("force:  %.3f %.3f %.3f ", totalForce.x, totalForce.y, totalForce.z);

		//Velocity
		//totalForce += integrateVelocity(m_pos0, m_vel0, timeStep, time);

		//Gravity
		totalForce += (gravity * particleMass);

		// Divide total force by particle mass to get acceleration
		glm::vec3 acceleration = totalForce / particleMass;
		glm::length(totalForce) == 0 ? acceleration = glm::vec3(0.0f, 0.0f, 0.0f) : void(0);
		animTcl::OutputMessage("accel:  %.3f %.3f %.3f ", acceleration.x, acceleration.y, acceleration.z);

		// Update velocity using the acceleration
		m_vel = m_vel0 + acceleration * timeStep;

		// Update position using the updated velocity
		m_pos = m_pos0 + m_vel * timeStep;

		// Set the new position and velocity to a temp array 
		newPos.push_back(m_pos);
		newVel.push_back(m_vel);
	}

	//Assign new pos and velocity
	for (int i = 0; i < numP; i++) {
		//animTcl::OutputMessage("Pos:  %.3f %.3f %.3f ", newPos[i].x, newPos[i].y, newPos[i].z);
		particles->setParticlePos(i, newPos[i]);
		particles->setParticleVel(i, newVel[i]);
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
	else if (strcmp(argv[0], "gravity") == 0) {
		if (argc == 2)
		{
			float g = static_cast<float>(atof(argv[1]));
			setGravity(g);
			return TCL_OK;
		}
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
			particles->addSpring(index1, index2);
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
