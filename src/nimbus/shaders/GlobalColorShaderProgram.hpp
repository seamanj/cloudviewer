#pragma once

#include "nimbus/utils/Color.hpp"

#include "nimbus/entities/Camera.hpp"
#include "nimbus/shaders/Uniform.hpp"


class GlobalColorShaderProgram {
public:
  virtual void set_color(const Color& color) = 0;
};
