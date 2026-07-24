#include "http_uri_parser.h"

#include "http_helper.h"

#include "foundation/path_normalize.h"
#include "foundation/tokenize.h"

#include <algorithm>

#include <boost/algorithm/string.hpp>

#include <spdlog/spdlog.h>

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
    vector<string> segments;
    split(segments,
          uri.raw_path,
          boost::is_any_of("/"));

    for (auto& seg : segments) {
        string decoded_seg;

        if (!hlpr::decode_path(seg.c_str(),
                               seg.size(),
                               decoded_seg)) {
            spdlog::trace("Failed to decode seg '{}' ...", seg);
        }

        seg = std::move(decoded_seg);
    }

    uri.decoded_path = join(segments, "/");

    return true;
}

bool HTTP_URI_Parser::parse_query(HTTP_URI& uri)
{
    string key, value;
    for (auto& token : tokenize(uri.raw_query, "&")) {
        const auto epos = token.find('=');
        if (epos == string::npos) {
            key = hlpr::decode_query(token.c_str(), token.size());
            value.clear();
        } else {
            key = hlpr::decode_query(token.c_str(), epos);
            value = hlpr::decode_query(token.c_str() + epos + 1, token.size() - epos - 1);
        }

        uri.query_params.emplace_back(key, value);
    }

    return true;
}
