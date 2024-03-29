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


void ParticleSimulator::setGroundParameters(float ks, float kd) {
	groundKs = ks;
	groundKd = kd;
}

glm::vec3 ParticleSimulator::springForce(glm::vec3 posi, glm::vec3 posj, glm::vec3 springParams) {
	float restLen = springParams.x;
	float ks = springParams.y;
	float kd = springParams.z;
	// | xi - xj |
	glm::vec3 delta = posi - posj;
	float posLen = glm::length(delta);

	// Check if the length is greater than a small threshold
	glm::vec3 normalized = (posLen > 0.0001f) ? delta / posLen : glm::vec3(0.0f);

	// ks(len - | xi - xj |) * (xi - xj / |xi - xj |)
	return ks * (restLen - posLen) * normalized;
}

glm::vec3 ParticleSimulator::damperForce(glm::vec3 posi, glm::vec3 veli, glm::vec3 posj, glm::vec3 velj, glm::vec3 springParams) {
	float restLen = springParams.x;
	float ks = springParams.y;
	float kd = springParams.z;
	// | xi - xj |
	glm::vec3 delta = posi - posj;
	float posLen = glm::length(delta);
	glm::vec3 deltaVel = veli - velj;

	// Check if the length is greater than a small threshold
	glm::vec3 normalized = (posLen > 0.0001f) ? delta / posLen : glm::vec3(0.0f);

	// ks(len - | xi - xj |) * (xi - xj / |xi - xj |)
	return -kd * (glm::dot(deltaVel, normalized)) * normalized;
}


glm::vec3 ParticleSimulator::integrateAcceleration(glm::vec3 pos0, glm::vec3 vel0, glm::vec3 acci, float dt)
{

	glm::vec3 vel = vel0;

	switch (integrationMethod) {
	case FORWARD_EULER:
		vel = vel0 + (acci * dt);
		break;

	case SYMPLECTIC_EULER:
		vel = vel0 + (acci * dt);
		break;

	case VERLET:
		// I had issues with the velocity when using verlet, 
		//if (!firstVerlet) {
		vel = vel0 + (acci * dt);
		//}
		//else {
		//	glm::vec3 forwardPos = pos0 + (vel0 * dt);
		//	glm::vec3 backwardPos = pos0 - (vel0 * dt);
		//	vel = (forwardPos - backwardPos) / (2.0f * dt);
		//}
		break;

	default:
		break;
	}
	return vel;
}

glm::vec3 ParticleSimulator::integrateVelocity(glm::vec3 posi, glm::vec3 vel0, glm::vec3 velNew, glm::vec3 acci, float dt)
{

	glm::vec3 pos = posi;

	switch (integrationMethod) {
	case FORWARD_EULER:
		pos = posi + (vel0 * dt);
		break;

	case SYMPLECTIC_EULER:
		pos = posi + (velNew * dt); // new Velocity
		break;

	case VERLET: // Euler for first step
		if (!firstVerlet) {
			// Forward Euler for the first step
			animTcl::OutputMessage("First");
			pos = posi + (vel0 * dt);
			firstVerlet = true;
		}
		else {
			// Verlet integration for subsequent steps
			pos = (2.0f * posi) - (posi - vel0 * dt) + (acci * dt * dt);
		}
		//animTcl::OutputMessage("Pos:  %.3f %.3f %.3f ", pos.x, pos.y, pos.z);
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
		//Compute fij
		if (indexI == i || indexJ == i) {
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

glm::vec3 ParticleSimulator::handleGround(glm::vec3 pos, glm::vec3 vel) {
	glm::vec3 groundNormal = glm::vec3(0.0f, 1.0f, 0.0f);

	float penetrationDepth = glm::dot(pos, groundNormal);
	glm::vec3 repulsionForce = glm::vec3(0.0f);

	if (penetrationDepth < 0.0f) {
		repulsionForce = -groundKs * penetrationDepth * groundNormal - groundKd * glm::dot(vel, groundNormal) * groundNormal;

	}

	return repulsionForce;

}

int ParticleSimulator::step(double time)
{
	std::vector<glm::vec3> newPos;
	std::vector<glm::vec3> newVel;
	int numP = particles->getNumParticles();

	for (int i = 0; i < numP; i++) {
		glm::vec3 pos0; // initial position
		glm::vec3 vel0; // initial velocity
		glm::vec3 posNew;
		glm::vec3 velNew;

		pos0 = particles->getParticlePos(i);
		vel0 = particles->getParticleVel(i);
		float particleMass = particles->getParticleMass(i);

		glm::vec3 totalForce = glm::vec3(0.0f, 0.0f, 0.0f);

		//Global Drag
		totalForce += -globalKd * vel0;

		// Sum of Spring Forces
		totalForce += handleSprings(i);
		//Gravity
		totalForce += (gravity * particleMass);

		//Ground
		totalForce += handleGround(pos0, vel0);

		// Divide total force by particle mass to get acceleration
		glm::vec3 acceleration = totalForce / particleMass;
		glm::length(totalForce) == 0 ? acceleration = glm::vec3(0.0f, 0.0f, 0.0f) : void(0);

		// Update velocity using the acceleration
		velNew = integrateAcceleration(pos0, vel0, acceleration, timeStep);

		// Update position using the updated velocity
		posNew = integrateVelocity(pos0, vel0, velNew, acceleration, timeStep);


		// Set the new position and velocity to a temp array 
		newPos.push_back(posNew);
		newVel.push_back(velNew);
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
		else
		{
			animTcl::OutputMessage("Usage: simulator <sim_name> gravity <g>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "link") == 0) {
		if (argc == 3) {
			const char* systemName = argv[1];
			int numSprings = atoi(argv[2]);

			animTcl::OutputMessage("Simulator %s linked to Particle System %s with %d springs.", sysName, systemName, numSprings);
			return TCL_OK;
		}
		else {
			animTcl::OutputMessage("Usage: simulator <sim_name> link <sys name> <Number of Springs>");
			return TCL_ERROR;
		}
	}

	else if (strcmp(argv[0], "fix") == 0) {
		if (argc == 2) {
			int index = atoi(argv[1]);
			particles->fixParticle(index);
		}
		else
		{
			animTcl::OutputMessage("Usage:  simulator <sim_name> fix <index>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "ground") == 0) {

		if (argc == 3) {
			float ks = atoi(argv[1]);
			float kd = atoi(argv[2]);
			groundKs = ks;
			groundKd = kd;
			setGroundParameters(ks, kd);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: simulator <sim_name> ground <ks> <kd> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "drag") == 0) {

		if (argc == 2) {
			float kdrag = atoi(argv[1]);
			globalKd = glm::abs(kdrag);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage:  simulator <sim_name> drag <kdrag>");
			return TCL_ERROR;
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
