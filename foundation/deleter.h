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

    /// Allow `make_deleter` to create instance
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

template <class Deleter>
inline Scoped_Deleter<Deleter> make_deleter(Deleter&& del)
{
    return Scoped_Deleter<Deleter>(std::forward<Deleter>(del));
}

#endif
