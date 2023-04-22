#include "particleframebuffer.h"
#include <string>
#include <sstream>
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
    getFromGpu();
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
  unloadFromGpu();
  m_particles.insert(std::end(m_particles), std::begin(_particles), std::end(_particles));
  loadToGpu();
  
}

void ParticleFrameBuffer::getFromGpu()
{
  GLint size = 0;
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glGetBufferParameteriv(GL_SHADER_STORAGE_BUFFER, GL_BUFFER_SIZE, &size);
  m_particles.reserve(size/sizeof(Particle));
  glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, size, m_particles.data());
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleFrameBuffer::serialize(std::ostream &_stream)
{
  getFromGpu();
  _stream << m_particles.size() << '\n';
  _stream.write((const char*)m_particles.data(), m_particles.size()*sizeof(Particle));
  _stream << '\n';
}

void ParticleFrameBuffer::serializeToGeo(std::ostream &_stream)
{
  getFromGpu();
  for(auto &i : m_particles)
  {
    _stream << i.m_position.m_x << " " << i.m_position.m_y << " " << i.m_position.m_z << " " << i.m_position.m_w << " ";
    _stream << "(";
    _stream << i.m_colour.m_r << " " << i.m_colour.m_g << " " << i.m_colour.m_b << " ";
    _stream << i.m_velocity.m_r << " " << i.m_velocity.m_g << " " << i.m_velocity.m_b << " ";
    _stream << i.m_mass << " ";
    _stream << i.m_radius;
    _stream << ")\n";
  }
}

void ParticleFrameBuffer::deserialize(std::istream &_stream)
{
  int size = 0;
  _stream >> size;
  std::cerr << size << std::endl;
  _stream.ignore(1, '\n');
  m_particles.clear();
  m_particles.resize(size); 
  _stream.read((char *)m_particles.data(), size*sizeof(Particle));
  _stream.ignore(1, '\n');
  generateSSBO();
  unloadFromGpu();
}

void ParticleFrameBuffer::generateSSBO()
{
  glGenBuffers(1, &m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, m_particles.size()*sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
