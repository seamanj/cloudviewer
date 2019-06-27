#pragma once

#include <vector>

#include "nimbus/glitter.hpp"

#include "nimbus/entities/FixedShaderEntity.hpp"
#include "nimbus/shaders/MvpShader.hpp"


class AxesEntity : public FixedShaderEntity<MvpShader> {
public:
  AxesEntity(float radius = 10.0f, size_t subdivisions = 10);

protected:
  void _draw() const override;

private:
  GLuint vao;
  GLuint vbo;

  std::vector<Eigen::Vector3f> _positions;
};
