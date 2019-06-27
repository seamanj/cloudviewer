#include <utility>

#include "Entity.hpp"

#include <Eigen/Geometry>


Pose::Pose()
  : _position(Eigen::Vector3f::Zero()),
  _orientation(1, 0, 0, 0)
{
  update_matrix();
}


Pose::Pose(const Eigen::Vector3f& position,
           const Eigen::Quaternionf& orientation)
  : _position(position),
  _orientation(orientation)
{
  update_matrix();
}


const Eigen::Vector3f& Pose::position() const {
  return _position;
}


void Pose::move_to(const Eigen::Vector3f& new_position) {
  _position = new_position;
  update_matrix();
}


const Eigen::Quaternionf& Pose::orientation() const {
  return _orientation;
}


void Pose::update_matrix() {
  _matrix = (_orientation * Eigen::Translation3f(-_position)).matrix();
  _inverse_matrix = _matrix.inverse();
}


Pose Pose::LookAt(const Eigen::Vector3f& eye,
                  const Eigen::Vector3f& target,
                  const Eigen::Vector3f& up) {
  Eigen::Vector3f f = (target - eye).normalized();
  Eigen::Vector3f s = f.cross(up).normalized();
  Eigen::Vector3f u = s.cross(f).normalized();

  Eigen::Matrix3f rotation;
  rotation.row(0) = s;
  rotation.row(1) = u;
  rotation.row(2) = -f;

  Eigen::Quaternionf r(rotation);

  return {eye, r};
}


void Pose::move(const Eigen::Vector3f& translation) {
  _position += translation;
  update_matrix();
}


void Pose::look_at(const Eigen::Vector3f& target, const Eigen::Vector3f& up) {
  *this = LookAt(position(), target, up);
}


const Eigen::Matrix4f& Pose::global_to_local() const {
  return _matrix;
}


const Eigen::Matrix4f& Pose::local_to_global() const {
  return _inverse_matrix;
}


Eigen::Vector4f Pose::local_x() const {
  return _matrix.col(0);
}


Eigen::Vector4f Pose::local_y() const {
  return _matrix.col(1);
}


Entity::Entity(const Pose& pose)
  : _pose(pose)
{
}


const Pose& Entity::pose() const {
  return _pose; 
}


Pose& Entity::pose() { 
  return _pose; 
}


ShaderEntity::ShaderEntity(const Pose& pose,
                           std::shared_ptr<ShaderProgram> program)
        : Entity(pose),
        _program(std::move(program))
{
}


const std::shared_ptr<ShaderProgram>& ShaderEntity::program() const {
  return _program;
}
