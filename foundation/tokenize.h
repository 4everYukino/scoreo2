#ifndef TOKENIZE_H
#define TOKENIZE_H

#include <boost/tokenizer.hpp>

template <typename T>
boost::tokenizer<boost::char_separator<T>>
tokenize(const std::basic_string<T>& input,
         const T* dropped_delims,
         const T* kept_delims = nullptr,
         boost::empty_token_policy empty_tokens = boost::drop_empty_tokens)
{
    return boost::tokenizer<boost::char_separator<T>>(
        input,
        boost::char_separator<T>(dropped_delims, kept_delims, empty_tokens)
    );
}

#endif
