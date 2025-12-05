#ifndef EXPONENTIAL_BACKOFF_ITERATOR_H
#define EXPONENTIAL_BACKOFF_ITERATOR_H

#include <memory>
#include <random>

#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>

/// \sa https://en.wikipedia.org/wiki/Exponential_backoff

class Exponential_Backoff_Iterator
    : public boost::iterator_facade<Exponential_Backoff_Iterator,
                                    unsigned,
                                    boost::single_pass_traversal_tag,
                                    unsigned>
{
public:
    explicit Exponential_Backoff_Iterator(unsigned base,
                                          unsigned max_retries,
                                          unsigned capped = UINT_MAX,
                                          bool jitter = true)
        : base_(base), max_retries_(max_retries), capped_(capped), jitter_(jitter)
    {
        if (jitter_) {
            std::random_device rd;
            rng_ = std::make_shared<std::mt19937>(
                rd()
            );
        }

        compute();
    }

    Exponential_Backoff_Iterator()
        : current_retry_(UINT_MAX)
    {

    }

private:
    friend class boost::iterator_core_access;

    unsigned dereference() const {
        return current_value_;
    }

    bool equal(const Exponential_Backoff_Iterator& other) const {
        if (current_retry_ >= max_retries_ &&
                other.current_retry_ > other.max_retries_)
            return true;

        return base_ == other.base_ &&
                   current_retry_ == other.current_retry_;
    }

    void increment() {
        if (current_retry_ < max_retries_) {
            current_retry_++;

            if (current_retry_ < max_retries_) {
                compute();
            }
        }
    }

private:
    void compute() {
        unsigned long long calculated = base_;

        if (current_retry_ < sizeof(unsigned long long) * 8) {
            calculated <<= current_retry_;
        } else {
            calculated = capped_;
        }

        if (calculated > capped_)
            calculated = capped_;

        if (jitter_ && rng_) {
            std::uniform_int_distribution<unsigned long long> dist(0, calculated);
            calculated = dist(*rng_);
        }

        current_value_ = static_cast<unsigned>(calculated);
    }

private:
    unsigned base_ = 0;
    unsigned max_retries_ = 0;
    unsigned capped_ = 0;
    bool jitter_ = false;
    std::shared_ptr<std::mt19937> rng_;

    unsigned current_retry_ = 0;
    unsigned current_value_ = 0;
};

inline boost::iterator_range<Exponential_Backoff_Iterator>
exponential_backoff(Exponential_Backoff_Iterator i)
{
    return boost::make_iterator_range(i, Exponential_Backoff_Iterator());
}

inline boost::iterator_range<Exponential_Backoff_Iterator>
exponential_backoff(unsigned base,
                    unsigned max_retries,
                    unsigned capped = UINT_MAX,
                    bool jitter = true)
{
    return exponential_backoff(
        Exponential_Backoff_Iterator(base,
                                     max_retries,
                                     capped,
                                     jitter)
    );
}

#endif
