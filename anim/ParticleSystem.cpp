#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	p_pos = glm::vec3(0.0, 0.0, 0.0);
	p_vel = glm::vec3(0.0, 0.0, 0.0);
}

void ParticleSystem::getState(double* p)
{
	// Assuming p is a 3-element array to represent the particle position
	p[0] = static_cast<double>(p_pos.x);
	p[1] = static_cast<double>(p_pos.y);
	p[2] = static_cast<double>(p_pos.z);
}

void ParticleSystem::setState(double* p)
{
	// Assuming p is a 3-element array representing the new particle position
	p_pos = glm::vec3(static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]));
}


void ParticleSystem::reset(double time)
{
	p_pos = glm::vec3(0.0f, 0.0f, 0.0f);
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
        if (argc == 3)
        {
            numParticles = atoi(argv[2]);
            particles.resize(numParticles, Particle(sysName));
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
            int index = atoi(argv[2]);
            if (index >= 0 && index < numParticles)
            {
                double mass = atof(argv[3]);
                glm::vec3 position(static_cast<float>(atof(argv[4])),
                    static_cast<float>(atof(argv[5])),
                    static_cast<float>(atof(argv[6])));
                glm::vec3 velocity(static_cast<float>(atof(argv[7])),
                    static_cast<float>(atof(argv[8])),
                    static_cast<float>(atof(argv[9])));
                particles[index] = Particle(sysName, position, velocity);
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
        if (argc == 5)
        {
            glm::vec3 velocity(static_cast<float>(atof(argv[2])),
                static_cast<float>(atof(argv[3])),
                static_cast<float>(atof(argv[4])));
            for (int i = 0; i < numParticles; ++i)
            {
                particles[i].setVelocity(velocity);
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

void ParticleSystem::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);  // Enable point smoothing
	glEnable(GL_BLEND);         // Enable blending for smooth points
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_COLOR_MATERIAL);
	// Define material properties
	glPointSize(8);
	glBegin(GL_POINTS);

	//glTranslated(p_pos.x, p_pos.y, p_pos.z);
	//glScalef(m_sx, m_sy, m_sz);
	glColor3f(0.3, 0.7, 0.1);

	//glutSolidSphere(0.3, 20, 20);
	glVertex3f(p_pos.x, p_pos.y, p_pos.z);
	glEnd();

	glPopMatrix();
	glPopAttrib();
	glDisable(GL_POINT_SMOOTH);  // Disable point smoothing when done
	glDisable(GL_BLEND);
}

