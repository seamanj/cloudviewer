#pragma once

#include "nimbus/shaders/Shader.hpp"
#include "nimbus/shaders/Uniform.hpp"

#include "nimbus/shaders/CameraShaderProgram.hpp"
#include "nimbus/shaders/GlobalColorShaderProgram.hpp"


class MvpShader : public ShaderProgram, public CameraShaderProgram, public GlobalColorShaderProgram {
public:
  static std::shared_ptr<MvpShader> load();

  void set_camera(const Camera& camera) override;
  void set_model(const Pose& pose) override;

  void set_color(const Color& color) override;

  MvpShader(GLint id);

private:
  Uniform<Eigen::Matrix4f> _model;
  Uniform<Eigen::Matrix4f> _view;
  Uniform<Eigen::Matrix4f> _projection;

  Uniform<Color> _color;
};
