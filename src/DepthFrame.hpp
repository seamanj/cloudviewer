#pragma once

#include <vector>

#include "TimeSeriesFrame.hpp"
#include "Image.hpp"


using depth_t = unsigned short;

constexpr depth_t DEPTH_INVALID = 0u;

using DepthFrame = TimeSeriesFrame<DefiniteSizedVectorImage<depth_t, 512, 424>>;

/**
 * Skips the header at the start of the .dpv file format, so you can get to the frames.
 * @param stream An istream to read from (e.g. ifstream of a .dpv file)
 * @return The number of bytes skipped.
 */
int dpv_skip_header(std::istream* stream);

/**
 * Reads a single depth frame from a file (written by rvl_write_frame())
 * @param stream The stream to read from.
 * @param frame The frame that will be overwritten with the depth data in the file.
 * @return The number of bytes read.
 */
int dpv_read_depth_frame(std::istream* stream, DepthFrame* frame);
