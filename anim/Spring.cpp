#include "Spring.h"

Spring::Spring() {
	index1 = 0;
	index2 = 0;
	restLength = 1.0;
	stiffness = 1.0;
	damping = 1.0;
}

Spring::Spring(int i1, int i2, float rl, float ks, float kd) {
	index1 = i1;
	index2 = i2;
	restLength = rl;
	stiffness = ks;
	damping = kd;
}

glm::vec2 Spring::getIndices() {
	glm::vec2 indices = glm::vec2(index1, index2);
	return indices;
}

// rl, ks, kd
glm::vec3 Spring::getCoefficients() {
	glm::vec3 coef = glm::vec3(restLength, stiffness, damping);
}
