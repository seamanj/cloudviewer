#include "nimbus/shaders/MvpShader.hpp"


MvpShader::MvpShader(GLint id)
  : ShaderProgram(id),
  _model("modelMatrix", id),
  _view("viewMatrix", id),
  _projection("projectionMatrix", id),
  _color("color", id)
{
}


std::shared_ptr<MvpShader> MvpShader::load() {
  auto vertex = *VertexShader::compile_resource("debug/mvp_color.vs");
  auto fragment = *FragmentShader::compile_resource("debug/mvp_color.fs");

  auto program = ShaderProgram::link(vertex, fragment);
  return std::make_shared<MvpShader>(program->id());
}


void MvpShader::set_camera(const Camera& camera) {
  _view.set(camera.pose().global_to_local());
  _projection.set(camera.lens().matrix());
}


void MvpShader::set_model(const Pose& pose) {
  _model.set(pose.local_to_global());
}


void MvpShader::set_color(const Color& color) {
  _color.set(color);
}
