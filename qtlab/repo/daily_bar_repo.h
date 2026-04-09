#ifndef DAILY_BAR_REPO_H
#define DAILY_BAR_REPO_H

#include "daily_bar.h"

#include <cstddef>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

class Daily_Bar_Repo
{
public:
    void store(const Daily_Bar& bar);
    void store(Daily_Bar&& bar);

    void store(const std::vector<Daily_Bar>& bars);
    void store(std::vector<Daily_Bar>&& bars);

    std::size_t store(const nlohmann::json& obj);

    std::optional<Daily_Bar> query(const std::string& ts_code,
                                   const std::string& trade_date) const;

    std::vector<Daily_Bar> query(const std::string& ts_code,
                                 const std::string& start_date,
                                 const std::string& end_date) const;

    std::vector<Daily_Bar> query(const std::string& ts_code,
                                 const std::string& start_date,
                                 std::size_t days) const;

    bool contains(const std::string& ts_code,
                  const std::string& trade_date) const;

    std::size_t size() const;

    bool empty() const;

private:
    bool parse_bar(const nlohmann::json& obj,
                   Daily_Bar& bar) const;

    std::size_t store_bar_array(const nlohmann::json& arr);

    std::size_t store_tushare_response(const nlohmann::json& obj);

private:
    using Bar_Map = std::map<std::string, Daily_Bar>;    ///< key: trade_date
    using Storage = std::map<std::string, Bar_Map>;      ///< key: ts_code

    Storage bars_;
};

#endif
