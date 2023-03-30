#include "nbodyparticlesystem.h"

NBodyParticleSystem::NBodyParticleSystem(std::string_view _updateProcess) : m_updateProcess(_updateProcess)
{
  m_particleBuffers.emplace_back(0);//blank frame 0 for now
}

NBodyParticleSystem::~NBodyParticleSystem()
{

}

void NBodyParticleSystem::processToFrame(size_t _frame, float _delta)
{
  if(_frame < m_particleBuffers.size())
  {
    return;
  }
  while(m_particleBuffers.size()+1 <= _frame)
  {
    processNextFrame(_delta);
  }
}
void NBodyParticleSystem::clearFrame(size_t _frame)
{
  eraseAfterFrame(_frame);
  m_particleBuffers.pop_back();
  m_particleBuffers.emplace_back(0);
}
void NBodyParticleSystem::addToFrame(size_t _frame, const std::vector<Particle> &_particles)
{
  eraseAfterFrame(_frame);
  m_particleBuffers[_frame].addParticles(_particles);
}
const ParticleFrameBuffer &NBodyParticleSystem::getFrame(size_t _frame) const
{
  if(_frame < m_particleBuffers.size())
  {
    return m_particleBuffers[_frame];
  }
  return m_particleBuffers.back();
}
void NBodyParticleSystem::eraseAfterFrame(size_t _frame)
{
  while(m_particleBuffers.size() >_frame+1)
  {
    m_particleBuffers.pop_back();
  }
}
void NBodyParticleSystem::processNextFrame(float _delta)
{
  auto lastSsbo = m_particleBuffers.back().ssbo();
  auto lastSize = m_particleBuffers.back().particleCount();
  m_particleBuffers.emplace_back(lastSize);
  ngl::ShaderLib::use(m_updateProcess);
  ngl::ShaderLib::setUniform("delta", _delta);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, lastSsbo);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0,lastSsbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleBuffers.back().ssbo());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1,m_particleBuffers.back().ssbo());
  glDispatchCompute(m_particleBuffers.back().particleCount()/100, 1, 1);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
