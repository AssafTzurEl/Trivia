#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <memory>

#define NS_BEGIN namespace Trivia {
#define NS_END }

// In real life, use a real logging framework, e.g. log4cplus or log4cpp.
// Until then:
#define LOG(msg) std::cout << msg << std::endl
#define LOG_ERROR(msg) std::cerr << msg << std::endl
// Obsolete C version, replaced by OO style above:
// #define LOG(msg, ...) printf(msg "\n", __VA_ARGS__)
