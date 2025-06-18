#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "http_def.h"
#include "http_handler.h"

#include <map>
#include <memory>

class Router
{
public:
    static Router* instance() {
        static Router r;
        return &r;
    }

    Router(const Router& other) = delete;
    Router operator=(const Router& other) = delete;

    bool add(const std::string& path,
             HTTP_Handler* handler);

    void dispatch(const Request& req, Response& res);

private:
    Router() {

    }

private:
    std::map<std::string, std::unique_ptr<HTTP_Handler>> routes_;
};

#endif
