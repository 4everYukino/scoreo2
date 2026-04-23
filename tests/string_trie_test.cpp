#include <gtest/gtest.h>

#include "infra/string_trie.h"

#include <string>

TEST(StringTrieTest, InsertRequiresExistingParentsByDefault)
{
    String_Trie<int> trie;

    EXPECT_FALSE(trie.insert("alpha/beta", 7));
    EXPECT_FALSE(trie.contains("alpha"));
    EXPECT_FALSE(trie.contains("alpha/beta"));
}

TEST(StringTrieTest, InsertCreatesMissingParentsWhenRequested)
{
    String_Trie<int> trie;

    EXPECT_TRUE(trie.insert("alpha/beta/gamma", 11, true));
    EXPECT_TRUE(trie.contains("alpha"));
    EXPECT_TRUE(trie.contains("alpha/beta"));
    EXPECT_TRUE(trie.contains("alpha/beta/gamma"));
}

TEST(StringTrieTest, ReinsertExistingKeySucceeds)
{
    String_Trie<std::string> trie;

    ASSERT_TRUE(trie.insert("svc/v1/health", std::string("old"), true));
    EXPECT_TRUE(trie.insert("svc/v1/health", std::string("new")));
    EXPECT_TRUE(trie.contains("svc/v1/health"));
}

TEST(StringTrieTest, InsertSupportsLvalueValue)
{
    String_Trie<std::string> trie;
    std::string handler = "users";

    ASSERT_TRUE(trie.insert("svc/v1/users", handler, true));
    EXPECT_EQ(trie.find("svc/v1/users"), std::optional<std::string>("users"));
}

TEST(StringTrieTest, EraseRemovesOnlyExactKey)
{
    String_Trie<std::string> trie;

    ASSERT_TRUE(trie.insert("svc/v1/users", std::string("users"), true));
    ASSERT_TRUE(trie.insert("svc/v1/orders", std::string("orders"), true));

    EXPECT_TRUE(trie.erase("svc/v1/users"));
    EXPECT_FALSE(trie.contains("svc/v1/users"));
    EXPECT_TRUE(trie.contains("svc/v1/orders"));
    EXPECT_FALSE(trie.erase("svc/v1/users"));
}

TEST(StringTrieTest, ErasePrefixRemovesWholeSubtree)
{
    String_Trie<std::string> trie;

    ASSERT_TRUE(trie.insert("svc/v1/users/list", std::string("list"), true));
    ASSERT_TRUE(trie.insert("svc/v1/users/detail", std::string("detail"), true));
    ASSERT_TRUE(trie.insert("svc/v1/orders/list", std::string("orders"), true));

    EXPECT_TRUE(trie.erase_prefix("svc/v1/users"));
    EXPECT_FALSE(trie.contains("svc/v1/users"));
    EXPECT_FALSE(trie.contains("svc/v1/users/list"));
    EXPECT_FALSE(trie.contains("svc/v1/users/detail"));
    EXPECT_TRUE(trie.contains("svc/v1/orders/list"));
    EXPECT_FALSE(trie.erase_prefix("svc/v1/users"));
}

TEST(StringTrieTest, ClearRemovesAllEntries)
{
    String_Trie<int> trie;

    ASSERT_TRUE(trie.insert("alpha/beta", 1, true));
    ASSERT_TRUE(trie.insert("alpha/gamma", 2, true));

    trie.clear();

    EXPECT_FALSE(trie.contains("alpha"));
    EXPECT_FALSE(trie.contains("alpha/beta"));
    EXPECT_FALSE(trie.contains("alpha/gamma"));
}

TEST(StringTrieTest, SupportsCustomSeparator)
{
    String_Trie<int> trie('.');

    ASSERT_TRUE(trie.insert("config.server.port", 8080, true));

    EXPECT_TRUE(trie.contains("config"));
    EXPECT_TRUE(trie.contains("config.server"));
    EXPECT_TRUE(trie.contains("config.server.port"));
}

TEST(StringTrieTest, IgnoresRepeatedAndEdgeSeparators)
{
    String_Trie<int> trie;

    ASSERT_TRUE(trie.insert("//svc///v1/users//", 1, true));

    EXPECT_TRUE(trie.contains("svc/v1/users"));
    EXPECT_TRUE(trie.contains("/svc//v1/users/"));
    EXPECT_EQ(trie.find("svc/v1/users"), std::optional<int>(1));
}
