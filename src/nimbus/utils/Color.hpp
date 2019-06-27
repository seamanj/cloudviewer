#pragma once

#include <cstdint>

#include "nimbus/glitter.hpp"
#include "nimbus/utils/math.hpp"


class Color {
public:
  using channel_t = uint8_t;

  channel_t r() const {
    return _r;
  }

  channel_t g() const {
    return _g;
  }

  channel_t b() const {
    return _b;
  }

  channel_t a() const {
    return _a;
  }

  operator uint32_t() const {
    return *(const uint32_t*)(this);
  }

  operator Eigen::Vector4f() const {
    return toOpenGl();
  }

  Eigen::Vector4f toOpenGl() const {
    return {
      _r / 255.0f,
      _g / 255.0f,
      _b / 255.0f,
      _a / 255.0f,
    };
  }

  operator Eigen::Vector3f() const {
    return {
      _r / 255.0f,
      _g / 255.0f,
      _b / 255.0f,
    };
  }

  // h in [0.0, 360.0)
  // s in [0.0, 1.0]
  // v in [0.0, 1.0]
  static Color Hsv(double h, double s, double v);
  static Color Rgb(channel_t r, channel_t g, channel_t b, channel_t a = 255);
  static Color Random(double s = 1.0, double v = 1.0);
  static Color FromFloat(double r, double g, double b, double a = 1.0);

  Color& operator=(const Eigen::Vector3f& v);
  Color& operator=(const Eigen::Vector4f& v);

  Color();

private:
  Color(channel_t r, channel_t g, channel_t b, channel_t a = 255);

  template <typename T>
  static channel_t clamp(T value) {
    return static_cast<channel_t>(::clamp<T>(value, static_cast<T>(0), static_cast<T>(255)));
  }

  channel_t _r;
  channel_t _g;
  channel_t _b;
  channel_t _a;
};
