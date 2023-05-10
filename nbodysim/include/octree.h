#ifndef OCTREE_H
#define OCTREE_H

#pragma once
#include <vector>
#include <ngl/Vec3.h>
#include <tuple>
#include "particle.h"

class Octree
{
  public:

    Octree(unsigned int _maxDepth = 5, unsigned int _desiredParticlesPerCell = 100, unsigned int _maxCpuDepth=2);
    ~Octree();
    void generate(std::vector<Particle> &_particles);
    struct Node
    {
      GLuint m_particlesStart=0;
      GLuint m_particlesSize=0;
      GLuint m_childrenOffset=0;
      GLuint m_parentOffset=0;
      ngl::Vec3 m_minExtent=ngl::Vec3(0,0,0);
      float m_totalMass=0.f;
      ngl::Vec3 m_maxExtent=ngl::Vec3(0,0,0);
      GLuint m_shouldSplit=0;
    };

  private:
    void processNode(size_t _nodeOffset, std::vector<Particle> &_particles, unsigned int _depth);
    void divideNode(size_t _nodeOffset, std::vector<Particle> &_particles, unsigned int _depth);
    unsigned int sortAxis(size_t _start, size_t _end, std::vector<Particle> &_particles, short _axis, float _mid);
    std::tuple<ngl::Vec3, ngl::Vec3, float> calcBoundsAndMass(size_t _start, size_t _end, const std::vector<Particle> &_particles1);

    unsigned int m_maxDepth;
    unsigned int m_desiredParticlesPerCell;
    std::vector<Node> m_nodes;
    unsigned int m_maxCpuDepth;

};

#endif