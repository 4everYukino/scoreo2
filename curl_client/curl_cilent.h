#ifndef CURL_CLIENT_H
#define CURL_CLIENT_H

#include "curl_request.h"
#include "curl_response.h"
#include "curl_slist_ptr.h"

#include <curl/curl.h>

class CURL_Client
{
public:
    CURL_Client();
    ~CURL_Client();

    CURL_Client(const CURL_Client&) = delete;
    CURL_Client& operator=(const CURL_Client&) = delete;

public:
    bool send(const CURL_Request& req, CURL_Response& resp);

private:
    void setup_common_options(const CURL_Request& req, CURL_Response& resp);

    CURL_Slist_Ptr setup_headers(const CURL_Request& req);

    bool perform_request(CURL_Response& resp);

private:
    CURL* curl_ = nullptr;
};

#endif
