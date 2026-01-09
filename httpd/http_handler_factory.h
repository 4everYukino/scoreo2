#ifndef HTTP_HANDLER_FACTORY_H
#define HTTP_HANDLER_FACTORY_H

#include "http_handler.h"

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

using HTTP_Handler_Creator = std::function<std::unique_ptr<HTTP_Handler>()>;

class HTTP_Handler_Factory
{
public:
    static HTTP_Handler_Factory* instance() {
        static HTTP_Handler_Factory f;
        return &f;
    }

private:
    HTTP_Handler_Factory() = default;
    ~HTTP_Handler_Factory() = default;

public:
    void dump();

    void register_handler(const std::string& name, HTTP_Handler_Creator fn);
    void unregister_handler(const std::string& name);

    bool contains(const std::string& name);

    std::unique_ptr<HTTP_Handler> create(const std::string& name);

private:
    /// HTTP Handler Name -> Creator
    std::unordered_map<std::string, HTTP_Handler_Creator> creators_;
};

#define REGISTER_HTTP_HANDLER(HANDLER) \
    namespace { \
        struct _register_##HANDLER { \
            _register_##HANDLER() { \
                HTTP_Handler_Factory::instance()->register_handler( \
                    #HANDLER, \
                    []() { \
                        return std::make_unique<HANDLER>(); \
                    } \
                ); \
            } \
        } _init_##HANDLER; \
    } \

#endif
