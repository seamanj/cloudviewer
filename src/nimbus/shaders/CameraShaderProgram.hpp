#pragma once

#include "nimbus/entities/Camera.hpp"
#include "nimbus/shaders/Uniform.hpp"


class CameraShaderProgram {
public:
  virtual void set_camera(const Camera& camera) = 0;
  virtual void set_model(const Pose& pose) = 0;
};
