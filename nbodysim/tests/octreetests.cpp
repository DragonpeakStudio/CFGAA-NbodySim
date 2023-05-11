#include <gtest/gtest.h>
#include <ngl/Random.h>
#include "octree.h"

TEST(octree,generate)
{
  Octree o(8, 100);
  std::vector<Particle> p(100000);
  for(auto &i : p)
  {
    i.m_position = ngl::Random::getRandomVec3()*100;
  }
  o.generate(p);
  int marker = 0;
}