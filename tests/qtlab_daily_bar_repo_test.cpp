#include "qtlab/repo/daily_bar_repo.h"

#include <gtest/gtest.h>

#include <nlohmann/json.hpp>

TEST(DailyBarRepoTest, StoresAndQueriesSingleBar)
{
    Daily_Bar_Repo repo;

    repo.store(Daily_Bar{
        .ts_code = "000001.SZ",
        .trade_date = "20180718",
        .open = 8.75,
        .high = 8.85,
        .low = 8.69,
        .close = 8.70,
        .pre_close = 8.72,
        .change = -0.02,
        .pct_chg = -0.23,
        .vol = 525152.77,
        .amount = 460697.377,
    });

    ASSERT_TRUE(repo.contains("000001.SZ", "20180718"));

    const auto bar = repo.query("000001.SZ", "20180718");
    ASSERT_TRUE(bar.has_value());
    EXPECT_DOUBLE_EQ(bar->close, 8.70);
    EXPECT_DOUBLE_EQ(bar->amount, 460697.377);
}

TEST(DailyBarRepoTest, StoresObjectArrayAndQueriesDateRange)
{
    Daily_Bar_Repo repo;

    const nlohmann::json bars = nlohmann::json::array({
        {
            {"ts_code", "000001.SZ"},
            {"trade_date", "20180716"},
            {"open", 8.85},
            {"high", 8.90},
            {"low", 8.69},
            {"close", 8.73},
            {"pre_close", 8.88},
            {"change", -0.15},
            {"pct_chg", -1.69},
            {"vol", 689845.58},
            {"amount", 603427.713}
        },
        {
            {"ts_code", "000001.SZ"},
            {"trade_date", "20180717"},
            {"open", 8.74},
            {"high", 8.75},
            {"low", 8.66},
            {"close", 8.72},
            {"pre_close", 8.73},
            {"change", -0.01},
            {"pct_chg", -0.11},
            {"vol", 375356.33},
            {"amount", 326396.994}
        },
        {
            {"ts_code", "000001.SZ"},
            {"trade_date", "20180718"},
            {"open", 8.75},
            {"high", 8.85},
            {"low", 8.69},
            {"close", 8.70},
            {"pre_close", 8.72},
            {"change", -0.02},
            {"pct_chg", -0.23},
            {"vol", 525152.77},
            {"amount", 460697.377}
        }
    });

    EXPECT_EQ(repo.store(bars), 3);

    const auto range = repo.query("000001.SZ", "20180717", "20180718");
    ASSERT_EQ(range.size(), 2);
    EXPECT_EQ(range.front().trade_date, "20180717");
    EXPECT_EQ(range.back().trade_date, "20180718");
}

TEST(DailyBarRepoTest, StoresTushareResponseAndQueriesByDays)
{
    Daily_Bar_Repo repo;

    const nlohmann::json resp = {
        {"code", 0},
        {"msg", ""},
        {"data", {
            {"fields", {
                "ts_code",
                "trade_date",
                "open",
                "high",
                "low",
                "close",
                "pre_close",
                "change",
                "pct_chg",
                "vol",
                "amount"
            }},
            {"items", nlohmann::json::array({
                nlohmann::json::array({"000001.SZ", "20180718", 8.75, 8.85, 8.69, 8.70, 8.72, -0.02, -0.23, 525152.77, 460697.377}),
                nlohmann::json::array({"000001.SZ", "20180717", 8.74, 8.75, 8.66, 8.72, 8.73, -0.01, -0.11, 375356.33, 326396.994}),
                nlohmann::json::array({"000001.SZ", "20180716", 8.85, 8.90, 8.69, 8.73, 8.88, -0.15, -1.69, 689845.58, 603427.713})
            })}
        }}
    };

    EXPECT_EQ(repo.store(resp), 3);

    const auto bars = repo.query("000001.SZ", "20180717", 2);
    ASSERT_EQ(bars.size(), 2);
    EXPECT_EQ(bars[0].trade_date, "20180717");
    EXPECT_EQ(bars[1].trade_date, "20180718");
}
