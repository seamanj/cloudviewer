#pragma once

#include <vector>

#include <Eigen/Core>

#include <nimbus/entities/PointXYZCloudEntity.hpp>


struct PointCloud : public PointCloudInterface {
  using Point = Eigen::Vector4f;
  using Points = std::vector<Eigen::Vector4f, Eigen::aligned_allocator<Eigen::Vector4f>>;

  Points points;

  PointCloud()
//    : points(512 * 424, Eigen::Vector4f::UnitW())
  {
      points.reserve(512 * 424);
  }


  size_t size() const override {
    return points.size();
  }


  const PointXYZ *data() const override {
    return reinterpret_cast<const PointXYZ*>(points.data());
  }
};
