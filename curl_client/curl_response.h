#ifndef CURL_RESPONSE_H
#define CURL_RESPONSE_H

#include <string>
#include <vector>

#include <curl/curl.h>

struct CURL_Response
{
public:
    void clear() {
        status = 0;
        headers.clear();
        body.clear();
    }

public:
    long status = 0;

    std::vector<std::string> headers;

    std::string body;
};

#endif
