#ifndef HTTP_URI_H
#define HTTP_URI_H

#include <string>
#include <vector>

class HTTP_URI
{
public:
    void clear() {
        target.clear();

        raw_path.clear();
        decoded_path.clear();

        raw_query.clear();
        query_params.clear();
    }

public:
    std::string target; ///< raw

    std::string raw_path;
    std::string decoded_path; ///< normalized

    std::string raw_query;
    std::vector<std::pair<std::string, std::string>> query_params; ///< decoded
};

#endif
