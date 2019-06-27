#pragma once

#include <stdio.h>

#define LOG(severity, fmt, ...) fprintf(stderr, "[%s] " fmt, #severity, __VA_ARGS__)
