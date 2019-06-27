#pragma once

#include "Entity.hpp"


template <typename ShaderT>
class FixedShaderEntity : public ShaderEntity {
public:
  FixedShaderEntity(const Pose& pose = Pose())
    : ShaderEntity(pose, ShaderT::load())
  {
  }

  FixedShaderEntity(const Pose& pose, std::shared_ptr<ShaderT> program)
    : ShaderEntity(pose, program)
  {
  }

  virtual ~FixedShaderEntity() = default;

  std::shared_ptr<ShaderT> fixed_program() const {
    return std::static_pointer_cast<ShaderT>(_program);
  }
};
