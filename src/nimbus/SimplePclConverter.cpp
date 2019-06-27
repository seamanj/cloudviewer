#include "SimplePclConverter.hpp"


struct IrCameraParams IrCameraParams::Default() {
  return {
    367.033f, // fx
    367.033f, // fy
    255.452f, // cx
    207.289f, // cy
  };
}


SimplePclConverter::SimplePclConverter(IrCameraParams params)
  : _params(params)
{
  for (size_t i = 0; i < DEPTH_FRAME_HEIGHT; i++) {
    _rowmap(i) = -(i - _params.cy + 0.5f) / _params.fy;
  }

  for(size_t j = 0; j < DEPTH_FRAME_WIDTH; j++) {
    _colmap(j) = (j - _params.cx + 0.5f) / _params.fx;
  }
}


bool SimplePclConverter::buffer_to_cloud(const unsigned short* data, std::vector<Eigen::Vector4f, Eigen::aligned_allocator<Eigen::Vector4f>>* cloud) const {
  const auto width = DEPTH_FRAME_WIDTH;
  const auto height = DEPTH_FRAME_HEIGHT;

  auto index = 0;
  cloud->clear();
  for (size_t i = 0; i < height; ++i) {
    const auto y_factor = _rowmap(i);

    for (size_t j = 0; j < width; ++j) {
      const auto x_factor = _colmap(j);
      const auto depth = data[index] / 1000.0f; // millimeters -> meters

      const auto x = x_factor * depth;
      const auto y = y_factor * depth;

      if( depth > 0.0001 && depth < 2.5 && y > -1.0 + 0.03)
      {
          cloud->emplace_back(Eigen::Vector4f(x,y, depth, 1.f));
      }
      ++index;

    }
  }

  return true;
}
