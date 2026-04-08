#include "curl_cilent.h"

#include <spdlog/spdlog.h>

using namespace std;

static size_t header_writer(char* buff, size_t size, size_t nmemb, void* ctx)
{
    vector<string>* headers = static_cast<vector<string>*>(ctx);

    const size_t len = size * nmemb;
    string h(buff, len);

    headers->push_back(h);

    return len;
}

static size_t body_writer(char* buff, size_t size, size_t nmemb, void* ctx)
{
    string* body = static_cast<string*>(ctx);
    const size_t len = size * nmemb;
    body->append(buff, len);

    return len;
}

CURL_Client::CURL_Client()
{
    curl_ = curl_easy_init();
}

CURL_Client::~CURL_Client()
{
    if (curl_)
        curl_easy_cleanup(curl_);
}

bool CURL_Client::send(const CURL_Request& req, CURL_Response& resp)
{
    if (!curl_)
        return false;

    curl_easy_reset(curl_);

    resp.clear();

    switch (req.method) {
    case CURL_HTTP_Method::GET:
        curl_easy_setopt(curl_, CURLOPT_HTTPGET, 1L);
        break;
    case CURL_HTTP_Method::POST:
        curl_easy_setopt(curl_, CURLOPT_POST, 1L);
        break;
    }

    setup_common_options(req, resp);

    CURL_Slist_Ptr hdr = setup_headers(req);

    switch (req.method) {
    case CURL_HTTP_Method::GET:
        break;
    case CURL_HTTP_Method::POST:
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, req.body.c_str());
        curl_easy_setopt(curl_, CURLOPT_POSTFIELDSIZE, req.body.length());
        break;
    }

    return perform_request(resp);
}

void CURL_Client::setup_common_options(const CURL_Request& req, CURL_Response& resp)
{
    curl_easy_setopt(curl_, CURLOPT_URL, req.target.c_str());

    curl_easy_setopt(curl_, CURLOPT_CONNECTTIMEOUT, req.connect_timeout);
    curl_easy_setopt(curl_, CURLOPT_TIMEOUT, req.transfer_timeout);

    if (req.follow_redirects)
        curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);

    if (req.verify_ssl_peer)
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYPEER, 1L);

    if (req.verify_ssl_host)
        curl_easy_setopt(curl_, CURLOPT_SSL_VERIFYHOST, 1L);

    if (!req.user_agent.empty())
        curl_easy_setopt(curl_, CURLOPT_USERAGENT, req.user_agent.c_str());

    curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, &header_writer);
    curl_easy_setopt(curl_, CURLOPT_HEADERDATA, &resp.headers);

    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, &body_writer);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &resp.body);
}

CURL_Slist_Ptr CURL_Client::setup_headers(const CURL_Request& req)
{
    CURL_Slist_Ptr ptr;

    for (const auto& h : req.headers) {
        curl_slist* new_list = curl_slist_append(ptr.get(), h.c_str());

        ptr.release();
        ptr.reset(new_list);
    }

    curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, ptr.get());

    return ptr;
}

bool CURL_Client::perform_request(CURL_Response& resp)
{
    char errbuf[CURL_ERROR_SIZE];

    curl_easy_setopt(curl_, CURLOPT_ERRORBUFFER, errbuf);

    CURLcode rc;

    rc = curl_easy_perform(curl_);
    if (rc != CURLE_OK) {
        spdlog::error("Failed to perform cURL, error: %s", curl_easy_strerror(rc));
        return false;
    }

    rc = curl_easy_getinfo(curl_, CURLINFO_RESPONSE_CODE, &resp.status);
    if (rc != CURLE_OK) {
        spdlog::error("Failed to perform cURL, error: %s", curl_easy_strerror(rc));
        return false;
    }

    return true;
}
