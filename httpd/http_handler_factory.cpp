#include "http_handler_factory.h"

#include <spdlog/spdlog.h>

using namespace std;

void HTTP_Handler_Factory::dump()
{
    spdlog::debug("All handlers in factory: ");
    for (const auto& [n, c] : creators_) {
        spdlog::debug("  {}", n);
    }
}

void HTTP_Handler_Factory::register_handler(const string& name, Handler_Creator fn)
{
    creators_[name] = std::move(fn);
}

void HTTP_Handler_Factory::unregister_handler(const string& name)
{
    auto it = creators_.find(name);
    if (it != creators_.end()) {
        creators_.erase(it);
    }
}

bool HTTP_Handler_Factory::contains(const string& name)
{
    return creators_.count(name);
}

unique_ptr<HTTP_Handler> HTTP_Handler_Factory::create(const string& name)
{
    auto it = creators_.find(name);
    if (it == creators_.end())
        return nullptr;

    Handler_Creator& c = it->second;
    return c();
}
