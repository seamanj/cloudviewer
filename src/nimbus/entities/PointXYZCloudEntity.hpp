#pragma once

#include "nimbus/glitter.hpp"

#include "nimbus/shaders/PointXYZCloudShader.hpp"

#include "nimbus/entities/FixedShaderEntity.hpp"

struct PointXYZ {
  EIGEN_ALIGN16
  float x;
  float y;
  float z;
  float w;
};

class PointCloudInterface {
 public:
  virtual size_t size() const = 0;
  virtual const PointXYZ* data() const = 0;
};

template <typename T>
class SimplePointCloudInterface : public PointCloudInterface {
 public:
  explicit SimplePointCloudInterface(std::shared_ptr<T> cloud = nullptr)
      : _cloud(std::move(cloud)) {}

  size_t size() const override {
    if (!_cloud) {
      return 0;
    }

    return _cloud->size();
  }

  const PointXYZ* data() const override {
    if (!_cloud) {
      return nullptr;
    }

    return reinterpret_cast<const PointXYZ*>(_cloud->points.data());
  }

 private:
  std::shared_ptr<T> _cloud;
};


class NullPointCloudInterface : public PointCloudInterface {
 public:
  size_t size() const override {
    return 0;
  }

  const PointXYZ* data() const override {
    return nullptr;
  }
};


class PointXYZCloudEntity : public FixedShaderEntity<PointXYZCloudShader> {
 public:
  using PointT = PointXYZ;
  using Cloud = PointCloudInterface;
  using CloudPtr = std::shared_ptr<Cloud>;

  PointXYZCloudEntity(
      CloudPtr point_cloud =
          std::make_shared<NullPointCloudInterface>(),
      const Pose& pose = Pose());

  const Cloud& point_cloud() const;
  Cloud& point_cloud();

  void set_color(const Eigen::Vector3f& color);

  void set_color_alpha(const Eigen::Vector4f& color);

  void set_point_cloud(CloudPtr point_cloud);

  void set_point_radius(float new_radius);

  void set_point_depth_threshold(float new_depth_threshold);

  void refresh_geometry();

 protected:
  void _draw() const override;
  void _update() override;

  void recreate_gl_buffers();

  CloudPtr _point_cloud;

  GLuint _vao;
  GLuint _vbo;

  Eigen::Vector4f _color;

  bool _dirty = false;
  bool _ready = false;
};
