#pragma once

#ifndef NDEBUG
#define ENABLE_LOGGING
#endif

#ifdef ENABLE_LOGGING
	#include <spdlog/spdlog.h>
	#if defined(__ANDROID__)
		#include "spdlog/sinks/android_sink.h"
	#else
		#include <spdlog/sinks/stdout_color_sinks.h>
	#endif
		#define LOG_LEVEL(x) spdlog::set_level(x)
		#define LOG_TRACE(...) spdlog::trace(__VA_ARGS__)
		#define LOG_DEBUG(...) spdlog::debug(__VA_ARGS__)
		#define LOG_INFO(...) spdlog::info(__VA_ARGS__)
		#define LOG_CRITICAL(...) spdlog::critical(__VA_ARGS__)
		#define LOG_WARN(...) spdlog::warn(__VA_ARGS__)
		#define LOG_ERROR(...) spdlog::error(__VA_ARGS__)
		#define LOG_LEVEL_TRACE spdlog::level::trace
		#define LOG_LEVEL_DEBUG spdlog::level::debug
		#define LOG_LEVEL_CRITICAL spdlog::level::critical
		#define LOG_LEVEL_INFO  spdlog::level::info
		#define LOG_LEVEL_WARN  spdlog::level::warn
		#define LOG_LEVEL_ERROR spdlog::level::err
		#define LOG_LEVEL_OFF spdlog::level::off
		#define LOG_GET_LEVEL() spdlog::get_level()
#else
	#define LOG_LEVEL(x)
	#define LOG_TRACE(...)
	#define LOG_DEBUG(...)
	#define LOG_INFO(...)
	#define LOG_CRITICAL(...)
	#define LOG_WARN(...)
	#define LOG_ERROR(...)
	#define LOG_LEVEL_TRACE
	#define LOG_LEVEL_DEBUG
	#define LOG_LEVEL_INFO
	#define LOG_LEVEL_CRITICAL
	#define LOG_LEVEL_WARN
	#define LOG_LEVEL_ERROR
	#define LOG_LEVEL_OFF
#endif
