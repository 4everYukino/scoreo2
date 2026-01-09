#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "http_handler.h"

class HTTP_Router
{
public:
    static HTTP_Router* instance() {
        static HTTP_Router r;
        return &r;
    }

private:
    HTTP_Router() = default;
    ~HTTP_Router() = default;

public:
    void add(const std::string& path, const std::string& name);

    bool dispatch(const HTTP_Request& req, HTTP_Response& res);

private:
    /// Path -> HTTP Handler Name
    std::map<std::string, std::string> routes_;
};

#endif
