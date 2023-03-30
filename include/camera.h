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
  ngl::Mat4 viewMat()
  {
    return ngl::lookAt(m_pos, m_lookAt, m_up);
  }
  ngl::Mat4 projMat(float _aspect)
  {
    return ngl::perspective(m_fov, _aspect, 0.01, 1000.);
  }
  void pan(ngl::Vec2 _pan)
  {
    ngl::Vec3 side = (m_pos-m_lookAt).cross(m_up);
    side.normalize();
    ngl::Vec3 up = side.cross(m_pos-m_lookAt);
    up.normalize();
    ngl::Vec3 m = side*_pan.m_x+up*_pan.m_y;
    m_pos+=m;
    m_lookAt+=m;
  }
  void zoom(float _zoom)
  {
    ngl::Vec3 d = (m_pos-m_lookAt);
    d.normalize();
    m_pos+=d*_zoom;
  }
  void orbit(ngl::Vec2 _orbit)
  {
    ngl::Vec4 pos = ngl::Vec4(m_pos-m_lookAt, 1.);
    ngl::Vec3 side = (m_pos-m_lookAt).cross(m_up);

    ngl::Quaternion q;
    q.fromAxisAngle(m_up, -_orbit.m_x);
    pos = pos*q.toMat4();
    q.fromAxisAngle(side, _orbit.m_y);
    pos = pos*q.toMat4();

    m_pos = ngl::Vec3(pos.m_x, pos.m_y, pos.m_z)+m_lookAt;
  }

};

#endif // CAMERA_H
