#include "Camera.hpp"

#include "nimbus/glitter.hpp"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h> // for M_PI

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif


Lens::Lens()
  : _projection(Eigen::Matrix4f::Identity())
{
}


Lens::Lens(const Eigen::Matrix4f& matrix)
  : _projection(matrix)
{
}


const Eigen::Matrix4f Lens::matrix() const {
  return _projection;
}


Lens Lens::Orthographic(float left, float right, float top, float bottom, float near, float far) {
  Eigen::Matrix4f matrix;

  matrix << 2.0f / (right - left), 0, 0, 0,
            0, 2.0f / (top - bottom), 0, 0, 0,
            0, 0, 2.0f / (far - near), 0,
            -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near);

  return {matrix};
}


Lens Lens::Perspective(float aspect, float fovy_deg, float zNear, float zFar) {
    typedef Eigen::Matrix4f Matrix4;

    assert(aspect > 0);
    assert(zFar > zNear);

    double radf = M_PI / 180.0f * fovy_deg;

    double tanHalfFovy = tan(radf / 2.0);
    Matrix4 res = Matrix4::Zero();
    res(0,0) = static_cast<float>(1.0 / (aspect * tanHalfFovy));
    res(1,1) = static_cast<float>(1.0 / (tanHalfFovy));
    res(2,2) = static_cast<float>(- (zFar + zNear) / (zFar - zNear));
    res(3,2) = static_cast<float>(- 1.0);
    res(2,3) = static_cast<float>(- (2.0 * zFar * zNear) / (zFar - zNear));
    return {res};
}


Lens Lens::Default(float aspect) {
  return Perspective(aspect, 90.0f, 0.1f, 1000.0f);
}



Camera::Camera(const Pose& pose,
               const Lens& lens)
  : Entity(pose),
  _lens(lens)
{
}


const Lens& Camera::lens() const {
  return _lens;
}


void Camera::set_lens(const Lens& new_lens) {
  _lens = new_lens;
}
