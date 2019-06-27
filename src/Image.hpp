#pragma once

#include <array>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <utility>
#include <vector>


struct ImageSize {
  size_t width;
  size_t height;

  bool operator==(const ImageSize& rhs) const {
    return width == rhs.width && height == rhs.height;
  }

  bool operator!=(const ImageSize& rhs) const {
    return !(rhs == *this);
  }
};


template <typename Pixel>
class Image {
public:
  using pixel_t = Pixel;
  using value_type = Pixel;

  virtual Pixel& at(size_t x, size_t y) = 0;
  virtual const Pixel& at(size_t x, size_t y) const = 0;

  virtual Pixel& at(size_t i) = 0;
  virtual const Pixel& at(size_t i) const = 0;

  virtual const Pixel* data() const = 0;
  virtual Pixel* data() = 0;

  virtual const Pixel* begin() const = 0;
  virtual Pixel* begin() = 0;

  virtual const Pixel* end() const = 0;
  virtual Pixel* end() = 0;

  virtual size_t width() const = 0;
  virtual size_t height() const = 0;

  virtual size_t pitch() const {
    return width() * sizeof(Pixel);
  }

  ImageSize video_size() const {
    return {width(), height()};
  }

  size_t size() const {
    return width() * height();
  }

  size_t size_bytes() const {
    return size() * sizeof(Pixel);
  }
};


template <typename Pixel>
bool operator==(const Image<Pixel>& l, const Image<Pixel>& r) {
  if (l.width() != r.width() || l.height() != r.height()) {
    return false;
  }

  for (size_t i = 0; i < l.height(); ++i) {
    for (size_t j = 0; j < l.width(); ++j) {
      if (l.at(j, i) != r.at(j, i)) {
        return false;
      }
    }
  }

  return true;
}


template <typename Pixel>
bool operator!=(const Image<Pixel>& l, const Image<Pixel>& r) {
  return !(l == r);
}


template <typename Pixel>
class VectorImage : public Image<Pixel> {
public:
  explicit VectorImage(ImageSize size = {0,0})
  : _size(size),
    _data(_size.width * _size.height)
  {
  }

  Pixel& at(size_t i) override {
    return _data.at(i);
  }

  const Pixel& at(size_t i) const override {
    return _data.at(i);
  }

  Pixel& at(size_t x, size_t y) override {
    return _data.at(y * _size.width + x);
  }

  const Pixel& at(size_t x, size_t y) const override {
    return _data.at(y * _size.width + x);
  }

  const Pixel* data() const override {
    return _data.data();
  }

  Pixel* data() override {
    return _data.data();
  }

  const Pixel* begin() const override {
    return &_data.at(0);
  }

  Pixel* begin() override {
    return &_data.at(0);
  }

  const Pixel* end() const override {
    return begin() + _data.size();
  }

  Pixel* end() override {
    return begin() + _data.size();
  }

  size_t width() const override {
    return _size.width;
  }

  size_t height() const override {
    return _size.height;
  }

private:
  ImageSize _size;
  std::vector<Pixel> _data;
};


template <typename Pixel, size_t _Width, size_t _Height>
class DefiniteSizedVectorImage : public VectorImage<Pixel> {
public:
  explicit DefiniteSizedVectorImage()
  : VectorImage<Pixel>({_Width, _Height})
  {
  }

  static const size_t Width = _Width;
  static const size_t Height = _Height;
  static const size_t Size = _Width * _Height;
  static const size_t SizeBytes = Size * sizeof(Pixel);
};