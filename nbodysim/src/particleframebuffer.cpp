#include "particleframebuffer.h"

ParticleFrameBuffer::ParticleFrameBuffer(size_t _particleCount) : m_particles{_particleCount}
{
  generateSSBO();
}

ParticleFrameBuffer::ParticleFrameBuffer(const std::vector<Particle> &_data) : m_particles{_data}
{
  generateSSBO();
}

ParticleFrameBuffer::ParticleFrameBuffer(ParticleFrameBuffer &&_old)
{
  m_particles = std::move(_old.m_particles);
  m_ssbo = _old.m_ssbo;
  _old.m_ssbo = 0;
}

ParticleFrameBuffer::~ParticleFrameBuffer()
{
    glDeleteBuffers(1, &m_ssbo);
} 
GLuint ParticleFrameBuffer::ssbo() const
{
  return m_ssbo;
}

void ParticleFrameBuffer::loadToGpu()
{
  if(m_ssbo != 0)
  {
    glBufferData(GL_SHADER_STORAGE_BUFFER, m_particles.size()*sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);
  }
  else
  {
    generateSSBO();
  }
}

void ParticleFrameBuffer::unloadFromGpu()
{
  if(m_ssbo != 0)
  {
    GLint size = 0;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
    glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
    m_particles.reserve(size/sizeof(Particle));
    glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, m_particles.data());
    glDeleteBuffers(1, &m_ssbo);
    m_ssbo=0;
  }
}

size_t ParticleFrameBuffer::particleCount() const
{
  return m_particles.size();
}

void ParticleFrameBuffer::addParticles(const std::vector<Particle> &_particles)
{
  glDeleteBuffers(1, &m_ssbo);//TODO avoid regenerating buffer
  //TODO get changes from gpu
  m_particles.insert(std::end(m_particles), std::begin(_particles), std::end(_particles));
  generateSSBO();
  
}

void ParticleFrameBuffer::generateSSBO()
{
  glGenBuffers(1, &m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, m_particles.size()*sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
