#include <gtest/gtest.h>

#include "httpd/http_uri_parser.h"

#include <string>
#include <vector>

using namespace std;

class HTTP_URI_Parser_Fixture : public ::testing::Test
{
protected:
    struct Case {
        std::string input; ///< path + query
        std::string exp_path;
        std::vector<std::pair<std::string,std::string>> exp_query;
    };

    std::vector<Case> common_cases() const {
        return {
            {"/index.html", "/index.html", {}},

            {"/search?q=test", "/search", {{"q", "test"}}},

            {"/search?q=test&lang=en", "/search", {{"q", "test"}, {"lang", "en"}}},

            {"/submit?name=", "/submit", {{"name", ""}}},

            {"/flag?debug", "/flag", {{"debug", ""}}},

            {"/path%20with%20space?q=hello%20world", "/path with space", {{"q", "hello world"}}},

            {"/check?param1=1%262&param2=a%2Fb", "/check", {{"param1", "1&2"}, {"param2", "a/b"}}},

            {"/folder/", "/folder/", {}},
        };
    }

    std::vector<Case> edge_cases() const {
        return {
            {"", "", {}},

            {"?only=param", "", {{"only", "param"}}},

            {"/repeat?key=1&key=2", "/repeat", {{"key", "1"}, {"key", "2"}}},

            {"/complex?data=a=b=c", "/complex", {{"data", "a=b=c"}}},

            {"/encoded%2Fslash?q=value", "/encoded/slash", {{"q", "value"}}},

            {"/weird?=value", "/weird", {{"", "value"}}},

            {"/nothing?", "/nothing", {}},
        };
    }

    void run(const Case& c) {
        HTTP_URI uri;
        EXPECT_TRUE(parser.parse(c.input, uri));
        EXPECT_EQ(c.exp_path, uri.decoded_path);
        ASSERT_EQ(c.exp_query.size(), uri.query_params.size())
            << "Query param size mismatch for input: " << c.input;

        for (size_t i = 0; i < c.exp_query.size(); ++i) {
            EXPECT_EQ(c.exp_query[i].first, uri.query_params[i].first)
                << "Key mismatch at index " << i;
            EXPECT_EQ(c.exp_query[i].second, uri.query_params[i].second)
                << "Value mismatch at index " << i;
        }
    }

private:
    HTTP_URI_Parser parser;
};

TEST_F(HTTP_URI_Parser_Fixture, common)
{
    for (const auto& i : common_cases()) {
        run(i);
    }
}

TEST_F(HTTP_URI_Parser_Fixture, edge)
{
    for (const auto& i : edge_cases()) {
        run(i);
    }
}
