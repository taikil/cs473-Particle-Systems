#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const std::string& name) :
	BaseSystem(name),
	//m_sx(0.5f),
	//m_sy(0.5f),
	//m_sz(0.5f)
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	verticesi(),
	verticesj()
{
	p_pos = glm::vec3(0.0, 0.0, 0.0);
	p_vel = glm::vec3(0.0, 0.0, 0.0);
}

void ParticleSystem::getState(double* p)
{
	p[0] = static_cast<double>(p_pos.x);
	p[1] = static_cast<double>(p_pos.y);
	p[2] = static_cast<double>(p_pos.z);
}

void ParticleSystem::setState(double* p)
{
	p_pos = glm::vec3(static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]));
}

int ParticleSystem::getNumParticles() {
	return numParticles;
}

glm::vec3 ParticleSystem::getParticlePos(int index) {
	return particles[index].getPos();
}

glm::vec3 ParticleSystem::getParticleVel(int index) {
	return particles[index].getVel();
}

float ParticleSystem::getParticleMass(int index) {
	return particles[index].getMass();
}

void ParticleSystem::setParticlePos(int index, glm::vec3 pos)
{
	particles[index].setPos(pos);
}

void ParticleSystem::setParticleVel(int index, glm::vec3 vel)
{
	particles[index].setVel(vel);
}


void ParticleSystem::reset(double time)
{
	for (int i = 0; i < numParticles; i++) {
		particles[i].reset(0);
	}
}

void ParticleSystem::addSpring(int i, int j)
{
	verticesi.push_back(i);
	verticesj.push_back(j);
}

int ParticleSystem::command(int argc, myCONST_SPEC char** argv)
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

	if (strcmp(argv[0], "dim") == 0)
	{
		if (argc == 2)
		{
			particles.clear();
			numParticles = atoi(argv[1]);
			for (int i = 0; i < numParticles; i++) {
				//std::string particleName = "Particle " + std::to_string(i);
				Particle pn = Particle();
				particles.push_back(pn);
			}
			glutPostRedisplay();
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: %s %s <Number of Particles>", argv[0], sysName);
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "particle") == 0)
	{
		if (argc == 9)
		{
			int index = atoi(argv[1]);
			if (index >= 0 && index < numParticles)
			{
				double mass = atof(argv[2]);
				glm::vec3 position(static_cast<float>(atof(argv[3])),
					static_cast<float>(atof(argv[4])),
					static_cast<float>(atof(argv[5])));
				glm::vec3 velocity(static_cast<float>(atof(argv[6])),
					static_cast<float>(atof(argv[7])),
					static_cast<float>(atof(argv[8])));
				particles[index] = Particle(position, velocity, mass);
				glutPostRedisplay();
				return TCL_OK;
			}
			else
			{
				animTcl::OutputMessage("Invalid particle index %d", index);
				return TCL_ERROR;
			}
		}
		else
		{
			animTcl::OutputMessage("Usage: %s %s <index> <mass> <x y z vx vy vz>", argv[0], sysName);
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0)
	{
		if (argc == 4)
		{
			glm::vec3 velocity(static_cast<float>(atof(argv[1])),
				static_cast<float>(atof(argv[2])),
				static_cast<float>(atof(argv[3])));
			for (int i = 0; i < numParticles; ++i)
			{
				particles[i].setVel(velocity);
			}
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: %s %s <vx> <vy> <vz>", argv[0], sysName);
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		//setState(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	glutPostRedisplay();
	return TCL_OK;
}

void ParticleSystem::displaySpring()
{
	glLineWidth(3);
	for (int i = 0; i < verticesi.size(); i++) {
		glBegin(GL_LINE_STRIP);
		glm::vec3 posi = particles[verticesi[i]].getPos();
		glm::vec3 posj = particles[verticesj[i]].getPos();
		glVertex3f(posi.x, posi.y, posi.z);
		glVertex3f(posj.x, posj.y, posj.z);
		glEnd();
	}
}

void ParticleSystem::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.4, 0.1, 0.7);
	glPointSize(8);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	for (int i = 0; i < numParticles; i++) {
		glPushMatrix();
		glm::vec3 pos = particles[i].getPos();
		//glBegin(GL_POINTS);
		//glVertex3f(pos.x, pos.y, pos.z);
		//glEnd();
		glTranslated(pos.x, pos.y, pos.z);
		glScalef(m_sx, m_sy, m_sz);
		glutSolidSphere(0.3, 20, 20);

		glPopMatrix();
	}
	glColor3f(0.1, 0.3, 0.8);
	displaySpring();
	glPopAttrib();
	glPushMatrix();
}

