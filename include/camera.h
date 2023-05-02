#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <ngl/Quaternion.h>
#include <ngl/Mat4.h>
#include <ngl/Util.h>

struct Camera
{
  ngl::Vec3 m_pos{2,0,0};
  ngl::Vec3 m_lookAt{0,0,0};
  ngl::Vec3 m_up{0,1,0};
  float m_fov = 70.f;
  ngl::Mat4 viewMat();
  ngl::Mat4 projMat(float _aspect);
  void pan(ngl::Vec2 _pan);
  void zoom(float _zoom);
  void orbit(ngl::Vec2 _orbit);

};

#endif // CAMERA_H
