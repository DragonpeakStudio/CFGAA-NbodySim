#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include <iostream>
#include <ngl/Vec3.h>
#include <ngl/Vec2.h>
#include <ngl/Quaternion.h>
#include <ngl/Mat4.h>
#include <ngl/Util.h>
/**
 * @brief A struct representing a camera, also contains some utility functions
 * 
 */
struct Camera
{
  ngl::Vec3 m_pos{2,0,0};
  ngl::Vec3 m_lookAt{0,0,0};
  ngl::Vec3 m_up{0,1,0};
  float m_fov = 70.f;
  /**
   * @brief returns the cameras view matrix
   * 
   * @return ngl::Mat4 
   */
  ngl::Mat4 viewMat();
  /**
   * @brief returns the cameras projection matrix
   * 
   * @param _aspect the screens aspect ratio (width/height)
   * @return ngl::Mat4 
   */
  ngl::Mat4 projMat(float _aspect);
  /**
   * @brief pans the camera along its local plane 
   * moves by up*_pan.y and up x forward * _pan.x
   * @param _pan the movement vector
   */
  void pan(ngl::Vec2 _pan);
  /**
   * @brief zooms the camera towards the lookat point
   * moves the camera by forward*zoom
   * @param _zoom how much to zoom 
   */
  void zoom(float _zoom);
  /**
   * @brief orbit the camera around the lookat point
   * with origin as lookat rotate around up by _orbit.x and around side by orbit.y
   * @param _orbit the vector to orbit by
   */
  void orbit(ngl::Vec2 _orbit);

};

#endif // CAMERA_H
