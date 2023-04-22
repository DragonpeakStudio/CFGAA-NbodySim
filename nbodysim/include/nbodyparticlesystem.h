#ifndef NBODYPARTICLESYSTEM_H
#define NBODYPARTICLESYSTEM_H

#pragma once

#include <string_view>
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
    void deserialize(std::istream &_stream);
    void serializeToGeo(std::ostream &_stream, size_t _frameNum);

    void setSpringCoeff(float _springCoeff);

    void setDampCoeff(float _dampCoeff);
    size_t currentFrameCount() const;
  private:
    void processNextFrame(float _delta);
    std::vector<ParticleFrameBuffer> m_particleBuffers;
    std::string m_updateProcess; 
    float m_dampCoeff = .4;
    float m_springCoeff = 20.;
};

#endif