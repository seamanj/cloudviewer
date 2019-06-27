#include "PointXYZCloudEntity.hpp"


PointXYZCloudEntity::PointXYZCloudEntity(PointXYZCloudEntity::CloudPtr point_cloud,
                                         const Pose& pose)
  : FixedShaderEntity(pose),
  _point_cloud(point_cloud),
  _color(Eigen::Vector4f::Ones())
{
  recreate_gl_buffers();
  fixed_program()->set_radius(0.001f);
}


void PointXYZCloudEntity::recreate_gl_buffers() {
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);
  glGenBuffers(1, &_vbo);

  refresh_geometry();

  
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, sizeof(typename PointXYZCloudEntity::PointT) / sizeof(float), GL_FLOAT, false, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);

  _ready = true;
}


void PointXYZCloudEntity::refresh_geometry() {
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  size_t bytes = _point_cloud->size() * sizeof(PointT);
  glBufferData(GL_ARRAY_BUFFER, bytes, _point_cloud->data(), GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void PointXYZCloudEntity::_draw() const {
  if (!_ready) {
    return;
  }

  auto point_cloud_program = fixed_program();

  glUseProgram(*point_cloud_program);
  point_cloud_program->set_model(pose());
  point_cloud_program->set_color_alpha(_color);

  glBindVertexArray(_vao);
  glEnableVertexAttribArray(0);

  auto size = static_cast<GLsizei>(_point_cloud->size());
  glDrawArrays(GL_POINTS, 0, size);

  glDisableVertexAttribArray(0);
  glBindVertexArray(0);
}


const PointXYZCloudEntity::Cloud& PointXYZCloudEntity::point_cloud() const {
  return *_point_cloud;
}


PointXYZCloudEntity::Cloud& PointXYZCloudEntity::point_cloud() {
  _dirty = true;
  return *_point_cloud;
}


void PointXYZCloudEntity::set_color(const Eigen::Vector3f& color) {
  _color = color.homogeneous();
}


void PointXYZCloudEntity::set_color_alpha(const Eigen::Vector4f& color) {
  _color = color;
}


void PointXYZCloudEntity::set_point_cloud(CloudPtr point_cloud) {
  _dirty = true;
  _point_cloud = point_cloud;
}


void PointXYZCloudEntity::_update() {
  if (_dirty) {
    refresh_geometry();
    _dirty = false;
  }
}


void PointXYZCloudEntity::set_point_radius(float new_radius) {
  fixed_program()->set_radius(new_radius);
}


void PointXYZCloudEntity::set_point_depth_threshold(float new_depth_threshold) {
  fixed_program()->set_depth_threshold(new_depth_threshold);
}
