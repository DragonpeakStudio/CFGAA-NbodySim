#include "particlerenderer.h"
#include <ngl/ShaderLib.h>

ParticleRenderer::ParticleRenderer(std::string_view _shader, std::unique_ptr<ngl::AbstractMesh> _instancedGeo) : m_shader(_shader), m_instancedGeo(std::move(_instancedGeo))
{
}

ParticleRenderer::~ParticleRenderer()
{

}
void ParticleRenderer::drawParticles(const ParticleFrameBuffer &_buffer, ngl::Mat4 _viewProj)
{
  ngl::ShaderLib::use(m_shader);
  ngl::ShaderLib::setUniform("viewProj", _viewProj);

  m_instancedGeo->bindVAO();
  
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, _buffer.ssbo());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, _buffer.ssbo());

  glDrawArraysInstanced(GL_TRIANGLES, 0, m_instancedGeo->getMeshSize(), _buffer.particleCount());

  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  ngl::ShaderLib::useNullProgram();
}
