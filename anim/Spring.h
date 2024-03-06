#pragma once

#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include "shared/opengl.h"

class Spring
{
public:
	Spring();
	Spring(int i1, int i2, float rl, float ks, float kd);
	int getI();
	int getJ();
	glm::vec3 getCoefficients();
protected:
	int index1;
	int index2;
	float restLength;
	float stiffness;
	float damping;
};

