#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "http_handler.h"
#include "http_request.h"
#include "http_response.h"

#include <map>
#include <memory>

class Router
{
public:
    static Router* instance() {
        static Router r;
        return &r;
    }

private:
    Router() = default;
    ~Router() = default;

public:
    bool add(const std::string& path, HTTP_Handler* handler);

    bool dispatch(const HTTP_Request& req, HTTP_Response& res);

private:
    std::map<std::string, std::unique_ptr<HTTP_Handler>> routes_;
};

#endif
