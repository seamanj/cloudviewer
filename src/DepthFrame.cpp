#include "DepthFrame.hpp"

#include "Log.hpp"


template <typename T>
bool read_stream(std::istream* stream, T* result, size_t count = sizeof(T)) {
  stream->read(reinterpret_cast<char*>(result), count);
  return stream->good() ;
}


bool decode_timestamp(std::istream* istream, std::chrono::high_resolution_clock::time_point* timestamp) {
  uint64_t ticks_since_epoch = 0;
  read_stream(istream, &ticks_since_epoch);
  if (timestamp) {
    *timestamp = DepthFrame::time_point(std::chrono::microseconds(ticks_since_epoch));
  }
  return istream->good();
}


struct RvlContext {
  int* buffer;
  int* pBuffer;
  int word;
  int nibblesWritten;
};


void EncodeVLE(int value, RvlContext* context) {
  do
  {
    int nibble = value & 0x7; // lower 3 bits
    if (value >>= 3) nibble |= 0x8; // more to come
    context->word <<= 4;
    context->word |= nibble;
    if (++context->nibblesWritten == 8) // output word
    {
      *context->pBuffer++ = context->word;
      context->nibblesWritten = 0;
      context->word = 0;
    }
  } while (value);
}


int DecodeVLE(RvlContext* context) {
  unsigned int nibble;
  int value = 0, bits = 29;
  do
  {
    if (!context->nibblesWritten)
    {
      context->word = *context->pBuffer++; // load word
      context->nibblesWritten = 8;
    }
    nibble = context->word & 0xf0000000;
    value |= (nibble << 1) >> bits;
    context->word <<= 4;
    context->nibblesWritten--;
    bits -= 3;
  } while (nibble & 0x80000000);
  return value;
}


void DecompressRVL(const char *input, depth_t *output, int numPixels) {
  RvlContext context = { nullptr, nullptr, 0, 0 };
  context.word = 0;
  context.buffer = context.pBuffer = (int*)input;
  context.nibblesWritten = 0;
  depth_t current, previous = 0;
  int numPixelsToDecode = numPixels;
  while (numPixelsToDecode)
  {
    int zeros = DecodeVLE(&context); // number of zeros
    numPixelsToDecode -= zeros;
    for (; zeros; zeros--)
      *output++ = 0;
    int nonzeros = DecodeVLE(&context); // number of nonzeros
    numPixelsToDecode -= nonzeros;
    for (; nonzeros; nonzeros--)
    {
      int positive = DecodeVLE(&context); // nonzero value
      int delta = (positive >> 1) ^ -(positive & 1);
      current = static_cast<depth_t>(previous + delta);
      *output++ = static_cast<depth_t>(current);
      previous = current;
    }
  }
}


using rvl_size_t = uint32_t;


/**
 * Decodes a single frame from the stream.
 * @param istream The stream to read from.
 * @param frame The frame to write depth data into.
 * @return The number of bytes read in total (including the bytes needed to read the frame size).
 */
int rvl_decode_frame(std::istream *istream, DepthFrame* frame) {
  static std::vector<char> buffer(2 * DepthFrame::value_type::SizeBytes);
  static const unsigned max_allowed_frame_bytes = 2000000; // for safety, most bytes that will be read.

  rvl_size_t frame_bytes;
  read_stream(istream, &frame_bytes);

  if (frame_bytes > max_allowed_frame_bytes) {
    LOG(error,
        "Depth recording frame header says frame is %du bytes (> max %du); likely corrupted",
        frame_bytes, max_allowed_frame_bytes);
    return false;
  }

  buffer.resize(frame_bytes);
  read_stream(istream, buffer.data(), frame_bytes);

  DecompressRVL(buffer.data(), frame->data.data(), static_cast<int>(frame->data.size()));
  return frame_bytes + sizeof(rvl_size_t);
}


int dpv_read_depth_frame(std::istream* stream, DepthFrame* frame) {
  static const auto RVL_MAX_SIZE = 4096; // max width/height for a frame.
  decode_timestamp(stream, &frame->timestamp);

  return rvl_decode_frame(stream, frame);
}


int dpv_skip_header(std::istream* stream) {
  constexpr auto DPV_HEADER_SIZE = 434224;
  stream->seekg(DPV_HEADER_SIZE, std::ios::beg);
  return DPV_HEADER_SIZE;
}
