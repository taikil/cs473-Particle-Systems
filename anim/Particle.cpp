#include "Particle.h"


Particle::Particle()
{
	p_pos = glm::vec3(0.0, 0.0, 0.0);
	p_vel = glm::vec3(0.0, 0.0, 0.0);
	p_pos0 = glm::vec3(0.0, 0.0, 0.0);
	p_vel0 = glm::vec3(0.0, 0.0, 0.0);
	mass = 1.0;
}

Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, const float m)
{
	p_pos = initialPosition;
	p_vel = initialVelocity;
	p_pos0 = initialPosition;
	p_vel0 = initialVelocity;
	mass = m;
}

void Particle::reset(double time)
{
	p_pos = p_pos0;
	p_vel = p_vel0;
}

glm::vec3 Particle::getPos() 
{
	return p_pos;
}

glm::vec3 Particle::getVel() {
	return p_vel;
}

void Particle::setPos(glm::vec3 pos)
{
	p_pos = pos;
	//animTcl::OutputMessage("Pos:  %.3f %.3f %.3f ", pos.x, pos.y, pos.z);
}

void Particle::setVel(glm::vec3 vel)
{
	p_vel = vel;
}

float Particle::getMass()
{
	return mass;
}

