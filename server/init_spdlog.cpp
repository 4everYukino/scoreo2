#include "init_spdlog.h"

#include "config.h"

#include <filesystem>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

using namespace std;
namespace fs = std::filesystem;

bool init_spdlog()
{
    fs::path dir(LOG_PREFIX);
    error_code ec;
    if (!fs::exists(dir, ec) && !fs::create_directory(dir, ec)) {
        spdlog::error("Failed to create '{}', {}", dir.c_str(), ec.message());
        return false;
    }

    fs::path dest = dir / fs::path(PROJECT_NAME);
    dest.replace_extension(".log");

    auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
        dest.c_str(),
        64 * 1024 * 1024, ///< 64MB
        10
    );

    auto rotating_logger = std::make_shared<spdlog::logger>("rotating_logger", rotating_sink);

    rotating_logger->flush_on(spdlog::level::debug);
    rotating_logger->set_level(spdlog::level::debug);
    rotating_logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");

    spdlog::set_default_logger(rotating_logger);

    return true;
}
