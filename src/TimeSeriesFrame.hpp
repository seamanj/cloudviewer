#pragma once

#include <algorithm>
#include <chrono>


template <typename T>
struct TimeSeriesFrame {
	using clock = std::chrono::high_resolution_clock;
	using time_point = clock::time_point;
	using value_type = T;

	explicit TimeSeriesFrame(time_point timestamp = clock::now())
		: data(),
		timestamp(timestamp)
	{
	}

	explicit TimeSeriesFrame(T&& data, time_point timestamp = clock::now())
		: data(std::move(data)),
		timestamp(timestamp)
	{
	}

	T data;
	time_point timestamp;
};


template <typename T>
static bool operator==(const TimeSeriesFrame<T>& left, const TimeSeriesFrame<T>& right) {
	return is_close(left.timestamp(), right.timestamp()) && left.data() == right.data();
}
