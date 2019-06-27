#pragma once

#include "nimbus/shaders/Shader.hpp"
#include "nimbus/shaders/Uniform.hpp"

#include "nimbus/shaders/CameraShaderProgram.hpp"


class PointXYZCloudShader : public ShaderProgram, public CameraShaderProgram {
public:
  static std::shared_ptr<PointXYZCloudShader> load();

  void set_camera(const Camera& camera) override;
  void set_model(const Pose& pose) override;

  void set_color(const Eigen::Vector3f& color);
  void set_color_alpha(const Eigen::Vector4f& color);

  void set_radius(float radius);

  void set_depth_threshold(float depth_threshold);

  PointXYZCloudShader(GLint id);

private:
  Uniform<Eigen::Matrix4f> _model;
  Uniform<Eigen::Matrix4f> _view;
  Uniform<Eigen::Matrix4f> _projection;

  Uniform<Eigen::Vector4f> _color;

  Uniform<float> _radius;
  Uniform<float> _depth_threshold;
};
