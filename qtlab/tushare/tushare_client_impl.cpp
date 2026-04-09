#include "tushare/tushare_client.h"

using namespace std;
using namespace nlohmann;

json Tushare_Client::daily(const string& ts_code,
                           const string& start_date,
                           const string& end_date)
{
    return post(
        "daily", {
            {"ts_code", ts_code},
            {"start_date", start_date},
            {"end_date", end_date}
        }
    );
}

json Tushare_Client::fund_daily(const string& ts_code,
                                const string& start_date,
                                const string& end_date)
{
    return post(
        "fund_daily", {
            {"ts_code", ts_code},
            {"start_date", start_date},
            {"end_date", end_date}
        }
    );
}

json Tushare_Client::fund_nav(const string& ts_code,
                              const string& start_date,
                              const string& end_date)
{
    return post(
        "fund_nav", {
            {"ts_code", ts_code},
            {"start_date", start_date},
            {"end_date", end_date}
        }
    );
}

json Tushare_Client::fund_adj(const string& ts_code,
                              const string& start_date,
                              const string& end_date)
{
    return post(
        "fund_adj", {
            {"ts_code", ts_code},
            {"start_date", start_date},
            {"end_date", end_date}
        }
    );
}

json Tushare_Client::trade_cal(const string& exchange,
                               const string& start_date,
                               const string& end_date)
{
    return post(
        "trade_cal", {
            {"exchange", exchange},
            {"start_date", start_date},
            {"end_date", end_date}
        }
    );
}

json Tushare_Client::post(const string& api_name,
                          const json& params)
{
    CURL_Request req;
    req.method = CURL_HTTP_Method::POST;
    req.target = "http://tsy.xiaodefa.cn";
    req.headers = {"Accept-Type: application/json",
                   "Content-Type: application/json"};

    json body = {
        {"api_name", api_name},
        {"token", token_},
        {"params", params}
    };

    req.body = body.dump();

    CURL_Response resp;

    if (!curl_client_.send(req, resp)) {
        return json();
    }

    return json::parse(resp.body);
}
