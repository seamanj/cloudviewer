#pragma once

#include <memory>

#include "nimbus/glitter.hpp"

#include "nimbus/shaders/Shader.hpp"


class Pose {
public:
  Pose();

  Pose(const Eigen::Vector3f& position,
       const Eigen::Quaternionf& orientation);

  const Eigen::Vector3f& position() const;
  const Eigen::Quaternionf& orientation() const;

  const Eigen::Matrix4f& global_to_local() const;
  const Eigen::Matrix4f& local_to_global() const;

  Eigen::Vector4f local_x() const;
  Eigen::Vector4f local_y() const;
  Eigen::Vector4f local_z() const;

  void move(const Eigen::Vector3f& translation);
  void move_to(const Eigen::Vector3f& new_position);
  void look_at(const Eigen::Vector3f& target, const Eigen::Vector3f& up = Eigen::Vector3f::UnitY());

  static Pose LookAt(const Eigen::Vector3f& eye,
                     const Eigen::Vector3f& target,
                     const Eigen::Vector3f& up = Eigen::Vector3f::UnitY());

private:
  void update_matrix();

  Eigen::Vector3f _position;
  Eigen::Quaternionf _orientation;

  Eigen::Matrix4f _matrix;
  Eigen::Matrix4f _inverse_matrix;
};


class Entity {
public:
  using Ptr = std::shared_ptr<Entity>;

  Entity(const Pose& pose = Pose());
  virtual ~Entity() = default;

  const Pose& pose() const;
  Pose& pose();

  void draw() const {
    if (_visible) {
      _draw();
    }
  };

  void update() { _update(); };

  void show() {
    _visible = true;
  }

  void hide() {
    _visible = false;
  }

  bool visible() const {
    return _visible;
  }

  bool* visible_ptr() {
    return &_visible;
  }

protected:
  virtual void _draw() const {};
  virtual void _update() {};

  Pose _pose;

  bool _visible = true;
};


class ShaderEntity : public Entity {
public:
    using Ptr = std::shared_ptr<ShaderEntity>;

    ShaderEntity(const Pose& pose = Pose(),
                 std::shared_ptr<ShaderProgram> program = nullptr);

    const std::shared_ptr<ShaderProgram>& program() const;

protected:
    std::shared_ptr<ShaderProgram> _program;
};
