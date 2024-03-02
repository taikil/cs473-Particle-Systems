#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(const std::string& name, BaseSystem* target) :
	BaseSimulator(name),
	m_object(target)
{
}	// ParticleSystem

ParticleSimulator::~ParticleSimulator()
{
}	// ParticleSystem::~ParticleSystem

int ParticleSimulator::step(double time)
{
	double g = -9.8;
	Vector pos;
	m_object->getState(pos);
	pos[0] = m_pos0[0] + m_vel0[0] * time;
	pos[1] = m_pos0[1] + m_vel0[1] * time + 0.5 * g * time * time;
	pos[2] = m_pos0[2] + m_vel0[2] * time;

	//   pos[1]= 2 * sin(2*3.14*time) ;

	m_object->setState(pos);

	return 0;

}	// ParticleSystem::step
