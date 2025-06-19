#ifndef DELETER_H
#define DELETER_H

#include <utility>

template <class Deleter>
class Scoped_Deleter
{
public:
    Scoped_Deleter(Deleter d) : del(std::forward<Deleter>(d)) {

    }

    Scoped_Deleter(const Scoped_Deleter&) = delete;
    Scoped_Deleter& operator=(const Scoped_Deleter&) = delete;

    Scoped_Deleter(Scoped_Deleter&& rhs) : del(std::forward<Deleter>(rhs.del)) {
        rhs.valid = false;
    }

    ~Scoped_Deleter() {
        free();
    }

    void release() {
        valid = false;
    }

private:
    void free() {
        if (valid)
            del();

        valid = false;
    }

private:
    Deleter del;
    bool valid = true;
};

#endif
