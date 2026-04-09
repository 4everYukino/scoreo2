#ifndef TUSHARE_CLIENT_H
#define TUSHARE_CLIENT_H

#include "curl_client/curl_cilent.h"

#include <nlohmann/json.hpp>

class Tushare_Client
{
public:
    explicit Tushare_Client(std::string token)
        : token_(token) {

    }

public:
    /**
     * @brief 获取交易所交易日历数据
     *
     * 对应 Tushare `trade_cal` 接口，返回指定交易所在给定日期区间内的交易日历，
     * 包括日历日期、是否开市以及上一交易日等信息。
     *
     * @param exchange 交易所代码，如 `SSE`（上交所）、`SZSE`（深交所）等。
     * @param start_date 开始日期，格式为 `YYYYMMDD`。
     * @param end_date 结束日期，格式为 `YYYYMMDD`。
     * @return Tushare 接口返回的原始 JSON 结果；若网络请求发送失败，则返回空 JSON。
     * 参考文档：https://tushare.pro/document/2?doc_id=26
     */
    nlohmann::json trade_cal(const std::string& exchange,
                             const std::string& start_date,
                             const std::string& end_date);

    /**
     * @brief 获取 A 股日线行情数据
     *
     * 对应 Tushare `daily` 接口，返回指定股票在给定日期区间内的未复权日线行情。
     *
     * @param ts_code 股票代码，采用 Tushare 标准代码格式，如 `000001.SZ`。
     * @param start_date 开始日期，格式为 `YYYYMMDD`。
     * @param end_date 结束日期，格式为 `YYYYMMDD`。
     * @return Tushare 接口返回的原始 JSON 结果；若网络请求发送失败，则返回空 JSON。
     * 参考文档：https://tushare.pro/document/2?doc_id=27
     */
    nlohmann::json daily(const std::string& ts_code,
                         const std::string& start_date,
                         const std::string& end_date);

    /**
     * @brief 获取公募基金净值数据
     *
     * 对应 Tushare `fund_nav` 接口，返回指定基金在给定日期区间内的净值信息，
     * 常见字段包括单位净值、累计净值、累计分红和复权单位净值等。
     *
     * @param ts_code 基金代码，采用 Tushare 标准代码格式，如 `165509.SZ`。
     * @param start_date 净值开始日期，格式为 `YYYYMMDD`。
     * @param end_date 净值结束日期，格式为 `YYYYMMDD`。
     * @return Tushare 接口返回的原始 JSON 结果；若网络请求发送失败，则返回空 JSON。
     * 参考文档：https://tushare.pro/document/2?doc_id=119
     */
    nlohmann::json fund_nav(const std::string& ts_code,
                            const std::string& start_date,
                            const std::string& end_date);

    /**
     * @brief 获取 ETF 日线行情数据
     *
     * 对应 Tushare `fund_daily` 接口，返回指定基金在给定日期区间内的日线成交数据。
     *
     * @param ts_code 基金代码，采用 Tushare 标准代码格式，如 `510330.SH`。
     * @param start_date 开始日期，格式为 `YYYYMMDD`。
     * @param end_date 结束日期，格式为 `YYYYMMDD`。
     * @return Tushare 接口返回的原始 JSON 结果；若网络请求发送失败，则返回空 JSON。
     * 参考文档：https://tushare.pro/document/2?doc_id=127
     */
    nlohmann::json fund_daily(const std::string& ts_code,
                              const std::string& start_date,
                              const std::string& end_date);

    /**
     * @brief 获取基金复权因子数据
     *
     * 对应 Tushare `fund_adj` 接口，返回指定基金在给定日期区间内的复权因子，
     * 可用于计算基金复权行情。
     *
     * @param ts_code 基金代码，采用 Tushare 标准代码格式，如 `513100.SH`。
     * @param start_date 开始日期，格式为 `YYYYMMDD`。
     * @param end_date 结束日期，格式为 `YYYYMMDD`。
     * @return Tushare 接口返回的原始 JSON 结果；若网络请求发送失败，则返回空 JSON。
     * 参考文档：https://tushare.pro/document/2?doc_id=199
     */
    nlohmann::json fund_adj(const std::string& ts_code,
                            const std::string& start_date,
                            const std::string& end_date);

private:
    nlohmann::json post(const std::string& api_name,
                        const nlohmann::json& params);

private:
    CURL_Client curl_client_;

    std::string token_;
};

#endif
