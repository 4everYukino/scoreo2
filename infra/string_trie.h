#ifndef STRING_TRIE_H
#define STRING_TRIE_H

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

template <typename T>
class String_Trie
{
private:
    struct Node {
        std::optional<T> value;

        std::unordered_map<std::string, std::unique_ptr<Node>> children;
    };

public:
    String_Trie();

    String_Trie(char separator);

    bool insert(std::string_view key, const T& value, bool create_parent = false);

    bool insert(std::string_view key, T&& value, bool create_parent = false);

    bool contains(std::string_view key) const;

    std::optional<T> find(std::string_view key) const;

    std::vector<T> find_chains(std::string_view key) const;

    bool erase(std::string_view key);

    bool erase_prefix(std::string_view key);

    void clear();

private:
    std::vector<std::string> split_key(std::string_view key) const;

    Node* find_internal(std::string_view key);

    const Node* find_internal(std::string_view key) const;

private:
    std::unique_ptr<Node> root_;

    char sep_ = '/';
};

#include "string_trie.inl"

#endif
