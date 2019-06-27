#include "nimbus/shaders/PointXYZCloudShader.hpp"


PointXYZCloudShader::PointXYZCloudShader(GLint id)
  : ShaderProgram(id),
  _model("modelMatrix", id),
  _view("viewMatrix", id),
  _projection("projectionMatrix", id),
  _color("color", id),
  _radius("radius", id),
  _depth_threshold("depth_threshold", id)
{
}


std::shared_ptr<PointXYZCloudShader> PointXYZCloudShader::load() {
  auto vertex = *VertexShader::compile_resource("point_xyz_cloud.vs");
  auto geometry = *GeometryShader::compile_resource("point_xyz_cloud.gs");
  auto fragment = *FragmentShader::compile_resource("point_xyz_cloud.fs");

  auto program = ShaderProgram::link(vertex, geometry, fragment);
  return std::make_shared<PointXYZCloudShader>(program->id());
}


void PointXYZCloudShader::set_camera(const Camera& camera) {
  _view.set(camera.pose().global_to_local());
  _projection.set(camera.lens().matrix());
}


void PointXYZCloudShader::set_model(const Pose& pose) {
  _model.set(pose.local_to_global());
}


void PointXYZCloudShader::set_color(const Eigen::Vector3f& color) {
  _color.set(color.homogeneous());
}


void PointXYZCloudShader::set_color_alpha(const Eigen::Vector4f& color) {
  _color.set(color);
}


void PointXYZCloudShader::set_radius(float radius) {
  _radius.set(radius);
}


void PointXYZCloudShader::set_depth_threshold(float depth_threshold) {
  _depth_threshold.set(depth_threshold);
}
