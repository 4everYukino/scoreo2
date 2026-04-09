#ifndef DAILY_BAR_H
#define DAILY_BAR_H

#include <string>

struct Daily_Bar
{
    std::string ts_code;     ///< Tushare 标准证券代码，例如 `000001.SZ`
    std::string trade_date;  ///< 交易日期，格式为 `YYYYMMDD`

    double open = 0.0;       ///< 当日开盘价
    double high = 0.0;       ///< 当日最高价
    double low = 0.0;        ///< 当日最低价
    double close = 0.0;      ///< 当日收盘价
    double pre_close = 0.0;  ///< 昨日收盘价

    double change = 0.0;     ///< 涨跌额，等于当日收盘价减前收盘价
    double pct_chg = 0.0;    ///< 涨跌幅，单位为百分比

    double vol = 0.0;        ///< 成交量
    double amount = 0.0;     ///< 成交额，一般情况下单位为千元
};

#endif
