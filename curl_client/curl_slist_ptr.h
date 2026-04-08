#ifndef CURL_SLIST_PTR_H
#define CURL_SLIST_PTR_H

#include <memory>

#include <curl/curl.h>

struct CURL_Slist_Deleter
{
    void operator()(curl_slist* p) const noexcept {
        if (p != nullptr) {
            curl_slist_free_all(p);
        }
    }
};

using CURL_Slist_Ptr = std::unique_ptr<curl_slist, CURL_Slist_Deleter>;

#endif
