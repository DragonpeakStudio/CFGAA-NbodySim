#pragma once
#ifndef PARTICLE_H
#define PARTICLE_H

#include <ngl/Vec3.h>
#include <ngl/Vec4.h>
struct Particle
{
  ngl::Vec4 m_position = ngl::Vec4(0,0,0,0);//Vec4 as to align to multiples of vec4 size in memory for ssbo
  ngl::Vec3 m_velocity = ngl::Vec3(0,0,0);
  ngl::Vec3 m_colour = ngl::Vec3(0,0,0);
  float m_radius = -1;
  float m_mass = -1;
};

#endif // PARTICLE_H
