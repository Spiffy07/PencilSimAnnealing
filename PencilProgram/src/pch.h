#pragma once
#include <string>
#include <array>
#include <chrono>
#include <iostream>
#include <thread>
#include <future>
#include <mutex>

#include "Timer.h"
#include "LogExtern.h"

#define PROFILING 1
#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif