#ifndef NBODYPARTICLESYSTEM_H
#define NBODYPARTICLESYSTEM_H

#pragma once

#include <string_view>
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
#include <ngl/ShaderLib.h>
#include "particleframebuffer.h"

class NBodyParticleSystem
{
  public:
    NBodyParticleSystem(std::string_view _updateProcess);
    ~NBodyParticleSystem();
    void processToFrame(size_t _frame, float _delta);
    void clearFrame(size_t _frame);
    void addToFrame(size_t _frame, const std::vector<Particle> &_particles);
    const ParticleFrameBuffer &getFrame(size_t _frame);
    void eraseAfterFrame(size_t _frame);

    void serialize(std::ostream &_stream);

  private:
    void processNextFrame(float _delta);
    std::vector<ParticleFrameBuffer> m_particleBuffers;
    std::string m_updateProcess; 
};

#endif