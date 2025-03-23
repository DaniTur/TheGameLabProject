#pragma once

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

#include "spdlog/spdlog.h"

#include <memory>

class Logger
{
public:

	static void Init();

	//Sends the log to the console and to the file
	static void InitMultiSink();

	inline static std::shared_ptr<spdlog::logger>& GetLogger() { return m_Logger; }

private:

	static std::shared_ptr<spdlog::logger> m_Logger;
};

#define LOG_TRACE(...)	Logger::GetLogger()->trace(__VA_ARGS__)
#define LOG_DEBUG(...)	Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...)	Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)	Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)	Logger::GetLogger()->error(__VA_ARGS__)
#define LOG_FATAL(...)	Logger::GetLogger()->critical(__VA_ARGS__)
