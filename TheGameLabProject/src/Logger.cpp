#include "Logger.h"
#include <iostream>

#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

std::shared_ptr<spdlog::logger> Logger::m_Logger;

void Logger::Init()
{
    try {
        m_Logger = spdlog::stdout_color_mt("GameLabProject");
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }

    // Patter set globaly for all loggers
    spdlog::set_pattern("[%T:%e][%^%l%$]: %v");
    spdlog::set_level(spdlog::level::trace); // Print logs from the trace level(0) to above
}

void Logger::InitMultiSink()
{
    try {
        spdlog::set_pattern("[%T:%e][%^%l%$]: %v");

        auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        consoleSink->set_level(spdlog::level::trace);

        auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/log.txt", true);
        fileSink->set_level(spdlog::level::trace);

        spdlog::sinks_init_list sinkList = { consoleSink, fileSink };

        m_Logger = std::make_shared<spdlog::logger>("multi_sink", sinkList.begin(), sinkList.end());
        m_Logger.get()->flush_on(spdlog::level::trace); // write to disk
    }
    catch (const spdlog::spdlog_ex& ex) {
        std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    }
}

