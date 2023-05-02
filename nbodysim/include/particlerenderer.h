#ifndef PARTICLERENDERER_H
#define PARTICLERENDERER_H

#pragma once

#include <ngl/AbstractMesh.h>
#include <ngl/Mat4.h>
#include <string>
#include <memory>
#include "particleframebuffer.h"

/**
 * @brief A simple instanced renderer for the particles
 * 
 */
class ParticleRenderer
{
  public:
  /**
   * @brief Construct a new Particle Renderer object
   * 
   * @param _shader the shader program to use for drawing
   * @param _instancedGeo the geometry to instance onto each particle
   */
    ParticleRenderer(std::string_view _shader, std::unique_ptr<ngl::AbstractMesh> _instancedGeo);
    ~ParticleRenderer();
    /**
     * @brief draw the particles provided using the instanced rendering
     * 
     * @param _buffer  the particle frame buffer to render
     * @param _viewProj the view and projection matrix to use, model is not relevant as particles are in world space
     */
    void drawParticles(const ParticleFrameBuffer &_buffer, ngl::Mat4 _viewProj);

  private:
    std::string m_shader;
    std::unique_ptr<ngl::AbstractMesh> m_instancedGeo;

};

#endif