#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "http_handler.h"
#include "infra/string_trie.h"

#include <string>

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
    void clear();

    HTTP_Response dispatch(const HTTP_Request& req);

private:
    /// Path -> HTTP Handler Name
    String_Trie<std::string> routes_;
};

#endif
