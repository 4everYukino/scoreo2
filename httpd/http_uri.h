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
        path_segments.clear();
        decoded_path.clear();

        raw_query.clear();
        query_params.clear();

        fragment.clear();
    }

public:
    std::string target; ///< raw

    std::string raw_path;
    std::vector<std::string> path_segments; ///< raw
    std::string decoded_path;

    std::string raw_query;
    std::vector<std::pair<std::string, std::string>> query_params; ///< decoded

    std::string fragment; ///< dose NOT included
};

#endif
