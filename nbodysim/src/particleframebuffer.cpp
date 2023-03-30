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
  glDeleteBuffers(1, &m_ssbo);//TODO avoid regenerating buffer
  //TODO get changes from gpu
  m_particles.insert(std::end(m_particles), std::begin(_particles), std::end(_particles));
  generateSSBO();
  
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
  _stream << m_particles.size() << "\n{\n";
  for(const auto &i : m_particles)
  {
    _stream << i.m_position.m_x << "," << i.m_position.m_y << "," << i.m_position.m_z << "," << i.m_position.m_w << ":";
    _stream << i.m_velocity.m_x << "," << i.m_velocity.m_y << "," << i.m_velocity.m_z << ":";
    _stream << i.m_radius << ":";
    _stream << i.m_colour.m_x << "," << i.m_colour.m_y << "," << i.m_colour.m_z << ":";
    _stream << i.m_mass;
    _stream << "\n";
  }
  _stream << "\n}\n";
}

void ParticleFrameBuffer::deserialize(std::istream &_stream)
{
  int size = 0;
  _stream >> size;
  _stream.ignore(4, '{');
  _stream.ignore(1, '\n');
  std::string line;
  m_particles.clear();
  m_particles.reserve(size); 
  while(_stream.peek()!='}')
  {
    m_particles.emplace_back(Particle());
    std::getline(_stream, line);
    std::sscanf(line.c_str(), "%f,%f,%f,%f:%f,%f,%f:%f:%f,%f,%f:%f\n", &m_particles.back().m_position.m_x, &m_particles.back().m_position.m_y, &m_particles.back().m_position.m_z, &m_particles.back().m_position.m_w,
    &m_particles.back().m_velocity.m_x, &m_particles.back().m_velocity.m_y, &m_particles.back().m_velocity.m_z, 
    &m_particles.back().m_radius,
    &m_particles.back().m_colour.m_x, &m_particles.back().m_colour.m_y, &m_particles.back().m_colour.m_z,
    &m_particles.back().m_mass);
  }
  _stream.ignore(3, '\n');
}

void ParticleFrameBuffer::generateSSBO()
{
  glGenBuffers(1, &m_ssbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_ssbo);
  glBufferData(GL_SHADER_STORAGE_BUFFER, m_particles.size()*sizeof(Particle), m_particles.data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}
