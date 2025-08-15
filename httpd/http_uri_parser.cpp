#include "http_uri_parser.h"

#include "http_helper.h"

#include "rtlib/tokenize.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
using boost::join;
using boost::split;

bool HTTP_URI_Parser::parse(beast::string_view tgt, HTTP_URI& uri)
{
    uri.clear();

#if BOOST_VERSION >= 107600
    // Boost >= 1.76: string_view has implicit conversion to std::string
    uri.target = string(tgt);
#else
    // Boost < 1.76: must call to_string()
    uri.target = tgt.to_string();
#endif

    return parse_i(uri);
}

bool HTTP_URI_Parser::parse_i(HTTP_URI& uri)
{
    if (uri.target.empty())
        return true;

    const auto qpos = uri.target.find('?');
    if (qpos == string::npos) {
        uri.raw_path = uri.target;
    } else {
        uri.raw_path = uri.target.substr(0, qpos);
        uri.raw_query = uri.target.substr(qpos + 1);
    }

    if (!uri.raw_path.empty() && !parse_path(uri))
        return false;

    if (!uri.raw_query.empty() && !parse_query(uri))
        return false;

    return true;
}

bool HTTP_URI_Parser::parse_path(HTTP_URI& uri)
{
    split(uri.path_segments,
          uri.raw_path,
          boost::is_any_of("/"));

    string decoded;
    for (auto& path : uri.path_segments) {
        if (!hlpr::decode_path(path.c_str(),
                               path.size(),
                               decoded)) {
            // Trace
        }

        path = move(decoded);
    }

    uri.decoded_path = join(uri.path_segments, "/");

    // Normalize ...

    return true;
}

bool HTTP_URI_Parser::parse_query(HTTP_URI& uri)
{
    auto tokens = tokenize(uri.raw_query, "&");
    transform(
        tokens.begin(),
        tokens.end(),
        back_inserter(uri.query_params),
        [](const string& t) {
            const auto epos = t.find('=');
            if (epos == string::npos)
                return make_pair(hlpr::decode_query(t.c_str(), t.size()),
                                 string());

            return make_pair(hlpr::decode_query(t.c_str(), epos),
                             hlpr::decode_query(t.c_str() + epos + 1, t.size() - epos - 1));
        }
    );

    return true;
}
