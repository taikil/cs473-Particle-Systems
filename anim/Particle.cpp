#include "Particle.h"


Particle::Particle(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	p_pos = glm::vec3(0.0, 0.0, 0.0);
	p_vel = glm::vec3(0.0, 0.0, 0.0);
}

Particle::Particle(const std::string& name, const glm::vec3& initialPosition, const glm::vec3& initialVelocity) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f)
{
	p_pos = initialPosition;
	p_vel = initialVelocity;
}

void Particle::getState(double* p)
{
	p[0] = static_cast<double>(p_pos.x);
	p[1] = static_cast<double>(p_pos.y);
	p[2] = static_cast<double>(p_pos.z);
}

void Particle::setState(double* p)
{
	p_pos = glm::vec3(static_cast<float>(p[0]), static_cast<float>(p[1]), static_cast<float>(p[2]));
}


void Particle::reset(double time)
{
	p_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Particle::setVelocity(const glm::vec3& velocity)
{
	p_vel = velocity;
}

int Particle::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			m_model.ReadOBJ(argv[1]);
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
			return TCL_OK;
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = static_cast<float>(atof(argv[1]));
			m_sy = static_cast<float>(atof(argv[2]));
			m_sz = static_cast<float>(atof(argv[3]));
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 4)
		{
			p_pos = glm::vec3(static_cast<float>(atof(argv[1])),
				static_cast<float>(atof(argv[2])),
				static_cast<float>(atof(argv[3])));
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <x> <y> <z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
		return TCL_OK;
	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		//setState(glm::vec3(0.0f, 0.0f, 0.0f));
	}

	glutPostRedisplay();
	return TCL_OK;
}

void Particle::display(GLenum mode)
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
