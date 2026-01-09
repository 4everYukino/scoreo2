#include "initializer.h"

#include "config.h"
#include "configure.h"

#include "httpd/http_handler_factory.h"
#include "httpd/http_router.h"

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>

using namespace std;
namespace fs = std::filesystem;

using namespace nlohmann;

static json parse_from_file(const char* path)
{
    json res;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        spdlog::error("Failed to open '{}' to read config, errno={}, {}",
                      path,
                      errno,
                      strerror(errno));

        return res;
    }

    try {
        ifs >> res;
    } catch (const json::parse_error& e) {
        spdlog::error("Failed to parse JSON in '{}', {}", path, e.what());

        res.clear();
        return res;
    }

    return res;
}

bool Initializer::operator()()
{
    return init_spdlog() &&
               parse_global_config() &&
                   parse_http_handler_config();
}

bool Initializer::init_spdlog()
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

bool Initializer::parse_global_config()
{
    fs::path cfg = fs::path(CONF_PREFIX) / fs::path(GLOBAL_CONFIG_FILE);

    json obj = parse_from_file(cfg.c_str());
    if (obj.empty())
        return false;

    if (!obj.is_object()) {
        spdlog::error("Failed to parse global config of {}, the '{}' is not an Object.",
                      PROJECT_NAME,
                      cfg.c_str());

        return false;
    }

    if (obj.contains("host")) {
        if (obj.at("host").is_string())
            Config::instance()->host = obj.at("host").get<string>();
    }

    if (obj.contains("port")) {
        if (obj.at("port").is_number_unsigned())
            Config::instance()->port = obj.at("port").get<unsigned short>();
    }

    if (obj.contains("threads")) {
        if (obj.at("threads").is_number_integer())
            Config::instance()->threads = obj.at("threads").get<int>();
    }

    return true;
}

bool Initializer::parse_http_handler_config()
{
    fs::path cfg = fs::path(CONF_PREFIX) / fs::path(HANDLER_CONFIG_FILE);

    json arr = parse_from_file(cfg.c_str());
    if (!arr.is_array()) {
        spdlog::error("Failed to load handler for {}, the '{}' is not an Object.",
                      PROJECT_NAME,
                      cfg.c_str());

        return false;
    }

    for (const auto& i : arr) {
        if (!i.is_object())
            continue;

        if (!i.contains("path") || !i.at("path").is_string() ||
                !i.contains("handler") || !i.at("handler").is_string())
            continue;

        const string& path = i.at("path").get<string>();
        const string& handler = i.at("handler").get<string>();
        if (!HTTP_Handler_Factory::instance()->contains(handler)) {
            spdlog::debug("Skipped a not registered HTTP handler '{}'", handler);
            continue;
        }

        HTTP_Router::instance()->add(path, handler);
        spdlog::debug("[HTTP_Router] Using '{}' to handler '{}'.", handler, path);
    }

    return true;
}
