#include "ParticleSimulator.h"

ParticleSimulator::ParticleSimulator(const std::string& name, ParticleSystem* target) :
	BaseSimulator(name),
	particles(target)
{
}	// ParticleSystem

ParticleSimulator::~ParticleSimulator()
{
}	// ParticleSystem::~ParticleSystem

int ParticleSimulator::step(double time)
{
	int numP = particles->getNumParticles();
	double g = -9.8;
	glm::vec3 pos;

	for (int i = 0; i < numP; i++) {
		m_pos0 = particles->getParticlePos(0);
		m_vel0 = particles->getParticleVel(0);
		animTcl::OutputMessage("Pos:  %.3f %.3f %.3f ", m_pos0.x, m_pos0.y, m_pos0.z);
		pos[0] = m_pos0[0] + m_vel0[0] * time;
		pos[1] = m_pos0[1] + m_vel0[1] * time + 0.5 * g * time * time;
		pos[2] = m_pos0[2] + m_vel0[2] * time;


		//   pos[1]= 2 * sin(2*3.14*time) ;

		particles->setParticlePos(0, pos);
	}

	return 0;

}	// ParticleSystem::step
