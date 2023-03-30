#ifndef PARTICLEFRAMEBUFFER_H
#define PARTICLEFRAMEBUFFER_H

#pragma once
#include <GL/gl3w.h>
#include <vector>
#include "particle.h"
class ParticleFrameBuffer
{//TODO possibly rename to avoid confusion with the concept of an opengl framebuffer
  public:
    ParticleFrameBuffer(size_t _particleCount);
    ParticleFrameBuffer(const std::vector<Particle> &_data);
    ParticleFrameBuffer(ParticleFrameBuffer &&_old);
    ParticleFrameBuffer (const ParticleFrameBuffer&) = delete;
    ParticleFrameBuffer& operator= (const ParticleFrameBuffer&) = delete;

    ~ParticleFrameBuffer();

    GLuint ssbo() const;
    void loadToGpu();
    void unloadFromGpu();

    size_t particleCount() const;
    void addParticles(const std::vector<Particle> &_particles);

  private:
    void generateSSBO();
    GLuint m_ssbo = 0;
    std::vector<Particle> m_particles;

};

#endif