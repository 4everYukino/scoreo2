#include "initializer.h"

#include "config.h"
#include "configure.h"

#include <fstream>
#include <filesystem>

#include <nlohmann/json.hpp>

#include <spdlog/spdlog.h>

using namespace std;
namespace fs = std::filesystem;

using namespace nlohmann;

bool Initializer::operator()()
{
    return parse_config();
}

bool Initializer::parse_config()
{
    fs::path cfg = fs::path(CONF_PREFIX) / fs::path(PROJECT_NAME);
    cfg.replace_extension(".conf");

    spdlog::debug("Using config in '{}' to start scoreo2.", cfg.c_str());

    error_code ec;
    if (!fs::exists(cfg, ec)) {
        spdlog::error("Unexcepted internal error, the conf '{}' does not exists, {}",
                      cfg.c_str(), ec.message());

        return false;
    }

    ifstream ifs(cfg.c_str());
    if (!ifs.is_open()) {
        spdlog::error("Failed to open '{}' to read config", cfg.c_str());
        return false;
    }

    json obj;
    try {
        ifs >> obj;
    } catch (const json::parse_error& e) {
        spdlog::error("Failed to parse JSON body, {}", e.what());
        return false;
    }

    if (obj.contains("threads")) {
        SCOREO2_Config::instance()->threads = obj["threads"];
    }

    return true;
}
