#include "initializer.h"

#include "config.h"
#include "configure.h"

#include "httpd/http_handler_factory.h"
#include "httpd/http_router.h"
#include "rtlib/detail_error.h"

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

#include <spdlog/spdlog.h>

using namespace std;
namespace fs = std::filesystem;

using namespace nlohmann;

static json parse_from_file(const char* path)
{
    json res;

    ifstream ifs(path);
    if (!ifs.is_open()) {
        spdlog::error("Failed to open '{}' to read config, {}",
                      path,
                      detail_error());

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
    return parse_global_config() &&
               parse_http_handler_config();
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

    if (obj.contains("threads")) {
        if (obj.at("threads").is_number_integer())
            SCOREO2_Config::instance()->threads = obj.at("threads").get<int>();
    }

    return true;
}

bool Initializer::parse_http_handler_config()
{
    fs::path cfg = fs::path(CONF_PREFIX) / fs::path(HANDLER_CONFIG_FILE);

    json arr = parse_from_file(cfg.c_str());
    if (arr.empty())
        return false;

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

        unique_ptr<HTTP_Handler> h = HTTP_Handler_Factory::instance()->create(handler);
        if (h && !Router::instance()->add(path, h.get())) {
            spdlog::error("Failed to add handler '{}' to Router ...", handler);
            continue;
        }

        spdlog::debug("Succeed to add HTTP handler '{}' to Router", handler);
        h.release();
    }

    return true;
}
