#ifndef INLINE_FUNC_H
#define INLINE_FUNC_H

#include <string>

#include <nlohmann/json.hpp>

#include <spdlog/spdlog.h>

inline bool get_string(const nlohmann::json& obj,
                       const char* key,
                       std::string& value)
{
    value.clear();

    if (!obj.contains(key) || obj.at(key).is_null())
        return true;

    const auto& jv = obj.at(key);
    if (jv.is_string()) {
        value = jv.get<std::string>();
        return true;
    }

    if (jv.is_number_integer()) {
        value = std::to_string(jv.get<long long>());
        return true;
    }

    if (jv.is_number_unsigned()) {
        value = std::to_string(jv.get<unsigned long long>());
        return true;
    }

    spdlog::error("Failed to parse field '{}', value is not string-compatible.", key);
    return false;
}

inline bool get_number(const nlohmann::json& obj,
                       const char* key,
                       double& value)
{
    value = 0.0;

    if (!obj.contains(key) || obj.at(key).is_null())
        return true;

    const auto& jv = obj.at(key);
    if (jv.is_number()) {
        value = jv.get<double>();
        return true;
    }

    if (jv.is_string()) {
        const auto& s = jv.get_ref<const std::string&>();
        if (s.empty())
            return true;

        try {
            value = std::stod(s);
            return true;
        } catch (...) {
            spdlog::error("Failed to parse field '{}', value '{}' is not numeric.", key, s);
            return false;
        }
    }

    spdlog::error("Failed to parse field '{}', value is not numeric-compatible.", key);
    return false;
}

#endif
