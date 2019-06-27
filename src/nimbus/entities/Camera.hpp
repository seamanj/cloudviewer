#pragma once

#include "nimbus/entities/Entity.hpp"
#include "nimbus/shaders/Shader.hpp"


class Lens {
public:
  Lens();
  Lens(const Eigen::Matrix4f& matrix);

  const Eigen::Matrix4f matrix() const;

  static Lens Orthographic(float left, float right, float top, float bottom, float near_z, float far_z);
  static Lens Perspective(float aspect, float fovy_deg, float near_z, float far_z);
  static Lens Default(float aspect = 16.0f/9.0f);

private:
  Eigen::Matrix4f _projection;
};


class Camera : public Entity {
public:
  Camera(const Pose& pose = Pose(),
         const Lens& lens = Lens::Default());

  const Lens& lens() const;
  void set_lens(const Lens& new_lens);

private:
  Lens _lens;
};
