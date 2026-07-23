#ifndef CURL_REQUEST_H
#define CURL_REQUEST_H

#include "config.h"

#include <string>
#include <vector>

enum class CURL_HTTP_Method
{
    GET,
    POST,
};

struct CURL_Request
{
    CURL_HTTP_Method method;

    std::string target;

    std::vector<std::string> headers;

    std::string body;

    long connect_timeout = 10L;
    long transfer_timeout = 30L;

    bool follow_redirects = true;

    bool verify_ssl_peer = true;
    bool verify_ssl_host = true;

    std::string user_agent = PROJECT_NAME;
};

#endif
