#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#pragma once

#include <ngl/AbstractMesh.h>
#include <ngl/Mat4.h>
#include <string>
#include <memory>
#include "particleframebuffer.h"

class ParticleRenderer
{
  public:
    ParticleRenderer(std::string_view _shader, std::unique_ptr<ngl::AbstractMesh> _instancedGeo);
    ~ParticleRenderer();
    void drawParticles(const ParticleFrameBuffer &_buffer, ngl::Mat4 _viewProj);

  private:
    std::string m_shader;
    std::unique_ptr<ngl::AbstractMesh> m_instancedGeo;

};

#endif