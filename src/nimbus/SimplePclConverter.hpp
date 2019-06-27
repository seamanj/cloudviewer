#pragma once

#include <vector>

#include <Eigen/Core>


struct IrCameraParams {
  float fx;
  float fy;
  float cx;
  float cy;

  static IrCameraParams Default();
};


/**
 * \brief Uses a simple camera model to create a PCL point cloud from a depth image.
 */
class SimplePclConverter {
public:
  SimplePclConverter(IrCameraParams params = IrCameraParams::Default());

  static constexpr auto DEPTH_FRAME_WIDTH = 512;
  static constexpr auto DEPTH_FRAME_HEIGHT = 424;

  /**
   * \brief Generates a PCL point cloud from a depth buffer, such as IDepthFrame provides.
   * \param data A pointer to an array of points whose short value is the Z coordinate, in millimeters.
   * \param cloud The point cloud where we should store the points. Existing points will be overwritten.
   * \return True if success, false on failure.
   */
  bool buffer_to_cloud(const unsigned short* data, std::vector<Eigen::Vector4f, Eigen::aligned_allocator<Eigen::Vector4f>>* cloud) const;

private:
  IrCameraParams _params;

  Eigen::Matrix<float, DEPTH_FRAME_WIDTH, 1> _colmap;
  Eigen::Matrix<float, DEPTH_FRAME_HEIGHT, 1> _rowmap;
};
