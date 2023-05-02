// Copyright (c) 2023 Rowan Cruse Howse
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

#include "camera.h"
ngl::Mat4 Camera::viewMat()
{
  return ngl::lookAt(m_pos, m_lookAt, m_up);
}
ngl::Mat4 Camera::projMat(float _aspect)
{
  return ngl::perspective(m_fov, _aspect, 0.01, 1000.);
}
void Camera::pan(ngl::Vec2 _pan)
{
  ngl::Vec3 side = (m_pos-m_lookAt).cross(m_up);
  side.normalize();
  ngl::Vec3 up = side.cross(m_pos-m_lookAt);
  up.normalize();
  ngl::Vec3 m = side*_pan.m_x+up*_pan.m_y;
  m_pos+=m;
  m_lookAt+=m;
}
void Camera::zoom(float _zoom)
{
  ngl::Vec3 d = (m_pos-m_lookAt);
  d.normalize();
  m_pos+=d*_zoom;
}
void Camera::orbit(ngl::Vec2 _orbit)
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