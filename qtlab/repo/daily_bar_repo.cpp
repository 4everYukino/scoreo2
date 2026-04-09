#include "daily_bar_repo.h"
#include "../utils/inline_func.h"

#include <algorithm>
#include <utility>

#include <spdlog/spdlog.h>

using namespace std;

using namespace nlohmann;

void Daily_Bar_Repo::store(const Daily_Bar& bar)
{
    if (bar.ts_code.empty() || bar.trade_date.empty()) {
        spdlog::error("Failed to store daily bar, ts_code or trade_date is empty.");
        return;
    }

    bars_[bar.ts_code][bar.trade_date] = bar;
}

void Daily_Bar_Repo::store(Daily_Bar&& bar)
{
    if (bar.ts_code.empty() || bar.trade_date.empty()) {
        spdlog::error("Failed to store daily bar, ts_code or trade_date is empty.");
        return;
    }

    bars_[bar.ts_code][bar.trade_date] = std::move(bar);
}

void Daily_Bar_Repo::store(const vector<Daily_Bar>& bars)
{
    for (const auto& bar : bars) {
        store(bar);
    }
}

void Daily_Bar_Repo::store(vector<Daily_Bar>&& bars)
{
    for (auto& bar : bars) {
        store(std::move(bar));
    }
}

size_t Daily_Bar_Repo::store(const json& obj)
{
    if (obj.is_null())
        return 0;

    if (obj.is_array())
        return store_bar_array(obj);

    if (!obj.is_object()) {
        spdlog::error("Failed to store daily bars, unsupported JSON type.");
        return 0;
    }

    if (obj.contains("data"))
        return store_tushare_response(obj);

    Daily_Bar bar;
    if (!parse_bar(obj, bar))
        return 0;

    store(std::move(bar));
    return 1;
}

optional<Daily_Bar> Daily_Bar_Repo::query(const string& ts_code,
                                          const string& trade_date) const
{
    const auto sec_it = bars_.find(ts_code);
    if (sec_it == bars_.end())
        return nullopt;

    const auto bar_it = sec_it->second.find(trade_date);
    if (bar_it == sec_it->second.end())
        return nullopt;

    return bar_it->second;
}

vector<Daily_Bar> Daily_Bar_Repo::query(const string& ts_code,
                                        const string& start_date,
                                        const string& end_date) const
{
    vector<Daily_Bar> res;

    if (start_date.empty() || end_date.empty() || start_date > end_date)
        return res;

    const auto sec_it = bars_.find(ts_code);
    if (sec_it == bars_.end())
        return res;

    const auto begin = sec_it->second.lower_bound(start_date);
    const auto end = sec_it->second.upper_bound(end_date);
    for (auto it = begin; it != end; ++it) {
        res.push_back(it->second);
    }

    return res;
}

vector<Daily_Bar> Daily_Bar_Repo::query(const string& ts_code,
                                        const string& start_date,
                                        size_t days) const
{
    vector<Daily_Bar> res;

    if (start_date.empty() || days == 0)
        return res;

    const auto sec_it = bars_.find(ts_code);
    if (sec_it == bars_.end())
        return res;

    auto it = sec_it->second.lower_bound(start_date);
    for (; it != sec_it->second.end() && res.size() < days; ++it) {
        res.push_back(it->second);
    }

    return res;
}

bool Daily_Bar_Repo::contains(const string& ts_code,
                              const string& trade_date) const
{
    return query(ts_code, trade_date).has_value();
}

size_t Daily_Bar_Repo::size() const
{
    size_t n = 0;

    for (const auto& [ts_code, bars] : bars_) {
        n += bars.size();
    }

    return n;
}

bool Daily_Bar_Repo::empty() const
{
    return size() == 0;
}

bool Daily_Bar_Repo::parse_bar(const json& obj,
                               Daily_Bar& bar) const
{
    if (!obj.is_object()) {
        spdlog::error("Failed to parse daily bar, JSON is not an object.");
        return false;
    }

    if (!get_string(obj, "ts_code", bar.ts_code) ||
            !get_string(obj, "trade_date", bar.trade_date) ||
            !get_number(obj, "open", bar.open) ||
            !get_number(obj, "high", bar.high) ||
            !get_number(obj, "low", bar.low) ||
            !get_number(obj, "close", bar.close) ||
            !get_number(obj, "pre_close", bar.pre_close) ||
            !get_number(obj, "change", bar.change) ||
            !get_number(obj, "pct_chg", bar.pct_chg) ||
            !get_number(obj, "vol", bar.vol) ||
            !get_number(obj, "amount", bar.amount))
        return false;

    if (bar.ts_code.empty() || bar.trade_date.empty()) {
        spdlog::error("Failed to parse daily bar, 'ts_code' or 'trade_date' is empty.");
        return false;
    }

    return true;
}

size_t Daily_Bar_Repo::store_bar_array(const json& arr)
{
    size_t n = 0;

    for (const auto& i : arr) {
        Daily_Bar bar;
        if (!parse_bar(i, bar))
            continue;

        store(std::move(bar));
        ++n;
    }

    return n;
}

size_t Daily_Bar_Repo::store_tushare_response(const json& obj)
{
    if (!obj.contains("data") || !obj.at("data").is_object()) {
        spdlog::error("Failed to store tushare response, field 'data' is invalid.");
        return 0;
    }

    const auto& data = obj.at("data");

    if (!data.contains("fields") || !data.at("fields").is_array() ||
            !data.contains("items") || !data.at("items").is_array()) {
        spdlog::error("Failed to store tushare response, fields or items is invalid.");
        return 0;
    }

    const auto& fields = data.at("fields");
    const auto& items = data.at("items");

    vector<string> field_names;
    field_names.reserve(fields.size());
    for (const auto& field : fields) {
        if (!field.is_string()) {
            spdlog::error("Failed to store tushare response, field name is not a string.");
            return 0;
        }

        field_names.push_back(field.get<string>());
    }

    size_t n = 0;
    for (const auto& item : items) {
        if (!item.is_array()) {
            spdlog::error("Failed to store tushare response, item row is not an array.");
            continue;
        }

        json bar_obj = json::object();

        const size_t count = min(field_names.size(), item.size());
        for (size_t i = 0; i < count; ++i) {
            bar_obj[field_names[i]] = item.at(i);
        }

        Daily_Bar bar;
        if (!parse_bar(bar_obj, bar))
            continue;

        store(std::move(bar));
        ++n;
    }

    return n;
}
