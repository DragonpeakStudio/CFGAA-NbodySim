#include "nbodyparticlesystem.h"
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/ShaderLib.h>
#include <cassert>
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
const ParticleFrameBuffer &NBodyParticleSystem::getFrame(size_t _frame)
{
  if(_frame >= 1)
  {
    m_particleBuffers[_frame-1].unloadFromGpu();
  }
  if(_frame < m_particleBuffers.size())
  {
    if(!m_particleBuffers[_frame].ssbo())
    {
      m_particleBuffers[_frame].loadToGpu();
    }
    return m_particleBuffers[_frame];
  }
  if(!m_particleBuffers.back().ssbo())
  {
    m_particleBuffers.back().loadToGpu();
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
void NBodyParticleSystem::setSpringCoeff(float _springCoeff)
{
  m_springCoeff = _springCoeff;
}
void NBodyParticleSystem::serialize(std::ostream &_stream)
{
  _stream << m_particleBuffers.size() << "\n"; 
  for(auto &i : m_particleBuffers)
  {
    i.serialize(_stream);
  }
}
void NBodyParticleSystem::setDampCoeff(float _dampCoeff)
{
  m_dampCoeff = _dampCoeff;
}
size_t NBodyParticleSystem::currentFrameCount() const
{
  return m_particleBuffers.size();
}
void NBodyParticleSystem::deserialize(std::istream &_stream)
{
  int size = 0;
  _stream >> size;
  _stream.ignore(1, '\n');
  m_particleBuffers.clear();
  m_particleBuffers.reserve(size);
  std::cerr << size << std::endl; 
  while(_stream.peek() != EOF)
  {
    m_particleBuffers.emplace_back(0);
    m_particleBuffers.back().deserialize(_stream);
  }
}
void NBodyParticleSystem::serializeToGeo(std::ostream &_stream, size_t _frameNum)
{
  assert((_frameNum < m_particleBuffers.size()));
  ParticleFrameBuffer &buffer = m_particleBuffers[_frameNum];
  _stream << "PGEOMETRY V5\n";
  _stream << "NPoints " << buffer.particleCount() << " NPrims 1\n";
  _stream << "NPointGroups 0 NPrimGroups 0\n";
  _stream << "NPointAttrib 4  NVertexAttrib 0 NPrimAttrib 1 NAttrib 0\n";
  _stream << "PointAttrib\n";
  _stream << "Cd 3 float 1 1 1\n";
  _stream << "v 3 float 1 1 1\n";
  _stream << "mass 1 float 1\n";
  _stream << "pscale 1 float 1\n";
  buffer.serializeToGeo(_stream);
  _stream << "PrimitiveAttrib\n";
  _stream << "generator 1 index 1 papi\n";
  _stream << "Part " << buffer.particleCount() << " ";
  for(size_t i = 0; i < buffer.particleCount(); i++)
  {
    _stream << i << " ";
  }
  _stream << "[0]\n";
  _stream << "beginExtra\nendExtra\n";

}
void NBodyParticleSystem::processNextFrame(float _delta)
{
  if(!m_particleBuffers.back().ssbo())m_particleBuffers.back().loadToGpu();
  auto lastSsbo = m_particleBuffers.back().ssbo();
  auto lastSize = m_particleBuffers.back().particleCount();
  m_particleBuffers.emplace_back(lastSize);
  ngl::ShaderLib::use(m_updateProcess);
  ngl::ShaderLib::setUniform("delta", _delta);
  ngl::ShaderLib::setUniform("springCoeff", m_springCoeff);
  ngl::ShaderLib::setUniform("dampCoeff", m_dampCoeff);

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, lastSsbo);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0,lastSsbo);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_particleBuffers.back().ssbo());
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1,m_particleBuffers.back().ssbo());
  glDispatchCompute(m_particleBuffers.back().particleCount()/128, 1, 1);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
  m_particleBuffers[m_particleBuffers.size()-2].unloadFromGpu();
}
