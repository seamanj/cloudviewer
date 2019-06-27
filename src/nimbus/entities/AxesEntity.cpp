#include "AxesEntity.hpp"

#include <cassert>


AxesEntity::AxesEntity(float radius, size_t subdivisions)
  : FixedShaderEntity(Pose())
{
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);

  _positions = {
    Eigen::Vector3f::Zero(),
    radius * Eigen::Vector3f::UnitX(),
    Eigen::Vector3f::Zero(),
    radius * Eigen::Vector3f::UnitY(),
    Eigen::Vector3f::Zero(),
    radius * Eigen::Vector3f::UnitZ(),
  };

  float increment = radius / subdivisions;
  for (size_t i = 0; i < subdivisions; ++i) {
    float v = (1 + i) * increment;
    _positions.emplace_back(Eigen::Vector3f(v, 0, 0));
    _positions.emplace_back(Eigen::Vector3f(v, 0, radius));

    _positions.emplace_back(Eigen::Vector3f(0, 0, v));
    _positions.emplace_back(Eigen::Vector3f(radius, 0, v));

    _positions.emplace_back(Eigen::Vector3f(0, v, 0));
    _positions.emplace_back(Eigen::Vector3f(0, v, radius));

    _positions.emplace_back(Eigen::Vector3f(0, 0, v));
    _positions.emplace_back(Eigen::Vector3f(0, radius, v));

    _positions.emplace_back(Eigen::Vector3f(v, 0, 0));
    _positions.emplace_back(Eigen::Vector3f(v, radius, 0));

    _positions.emplace_back(Eigen::Vector3f(0, v, 0));
    _positions.emplace_back(Eigen::Vector3f(radius, v, 0));
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, _positions.size() * sizeof(Eigen::Vector3f), _positions.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void AxesEntity::_draw() const {
  auto mvp_program = std::dynamic_pointer_cast<MvpShader>(program());
  assert(mvp_program);

  mvp_program->set_model(pose());

  glBindVertexArray(vao);
  glEnableVertexAttribArray(0);

  mvp_program->set_color(Color::Rgb(255, 0, 0));
  glDrawArrays(GL_LINES, 0, 2);

  mvp_program->set_color(Color::Rgb(0, 255, 0));
  glDrawArrays(GL_LINES, 2, 2);

  mvp_program->set_color(Color::Rgb(0, 0, 255));
  glDrawArrays(GL_LINES, 4, 2);

  mvp_program->set_color(Color::Rgb(128, 128, 128, 128));
  glDrawArrays(GL_LINES, 6, static_cast<GLsizei>(_positions.size() - 6));

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}
