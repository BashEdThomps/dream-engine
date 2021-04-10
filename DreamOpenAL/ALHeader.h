#pragma once

#if defined(__APPLE__)
#define OPENAL_DEPRECATED
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#elif defined(__ANDROID__)
#elif defined(__linux__)
#include <AL/al.h>
#include <AL/alc.h>
#endif
