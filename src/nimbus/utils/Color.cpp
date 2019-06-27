#include "Color.hpp"

#include <random>


Color::Color()
  : _r(0),
  _g(0),
  _b(0),
  _a(255)
{
}


Color::Color(channel_t r, channel_t g, channel_t b, channel_t a)
  : _r(r),
  _g(g),
  _b(b),
  _a(a)
{
}


Color Color::Hsv(double h, double s, double v) {
  double      hh, p, q, t, ff;
  long        i;

  if(s <= 0.0) {
    return FromFloat(v, v, v);
  }

  hh = h;
  if(hh >= 360.0) hh = 0.0;
  hh /= 60.0;
  i = (long)hh;
  ff = hh - i;
  p = v * (1.0 - s);
  q = v * (1.0 - (s * ff));
  t = v * (1.0 - (s * (1.0 - ff)));

  switch(i) {
  case 0:
    return FromFloat(v, t, p);
  case 1:
    return FromFloat(q, v, p);
  case 2:
    return FromFloat(p, v, t);
  case 3:
    return FromFloat(p, q, v);
  case 4:
    return FromFloat(t, p, v);
  case 5:
  default:
    return FromFloat(v, p, q);
  }
}


Color Color::Rgb(channel_t r, channel_t g, channel_t b, channel_t a) {
  return {r, g, b, a};
}


Color Color::FromFloat(double r, double g, double b, double a) {
  return {
    clamp(r * 255),
    clamp(g * 255),
    clamp(b * 255),
    clamp(a * 255),
  };
}


Color& Color::operator=(const Eigen::Vector3f& v) {
  _r = clamp(v.x() * 255);
  _g = clamp(v.y() * 255);
  _b = clamp(v.z() * 255);

  return *this;
}


Color& Color::operator=(const Eigen::Vector4f& v) {
  _r = clamp(v.x() * 255);
  _g = clamp(v.y() * 255);
  _b = clamp(v.z() * 255);
  _a = clamp(v.w() * 255);

  return *this;
}


Color Color::Random(double s, double v) {
  static std::random_device rd;
  std::mt19937 e2(rd());
  std::uniform_real_distribution<> dist(0, 360);

  return Hsv(dist(e2), s, v);
}
