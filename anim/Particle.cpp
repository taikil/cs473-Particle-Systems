#include "Particle.h"

Particle::Particle(const std::string& name) :
    BaseSystem(name),
    m_sx(1.0f),
    m_sy(1.0f),
    m_sz(1.0f)
{
    p_pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Particle::getState(glm::vec3& position)
{
    position = p_pos;
}

void Particle::setState(const glm::vec3& position)
{
    p_pos = position;
}

void Particle::reset(double time)
{
    p_pos = glm::vec3(0.0f, 0.0f, 0.0f);
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
        setState(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    glutPostRedisplay();
    return TCL_OK;
}

void Particle::display(GLenum mode)
{
    glEnable(GL_LIGHTING);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glTranslated(p_pos.x, p_pos.y, p_pos.z);
    glScalef(m_sx, m_sy, m_sz);

    if (m_model.numvertices > 0)
        glmDraw(&m_model, GLM_SMOOTH | GLM_MATERIAL);
    else
        glutSolidSphere(1.0, 20, 20);

    glPopMatrix();
    glPopAttrib();
}
