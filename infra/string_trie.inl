#include "string_trie.h"

template <typename T>
String_Trie<T>::String_Trie()
{
    root_ = std::make_unique<Node>();
}

template <typename T>
String_Trie<T>::String_Trie(char separator)
    : sep_(separator)
{
    root_ = std::make_unique<Node>();
}

template <typename T>
bool String_Trie<T>::insert(std::string_view key, const T& value, bool create_parent)
{
    const auto segments = split_key(key);

    Node* n = root_.get();
    for (const auto& seg : segments) {
        auto it = n->children.find(seg);
        if (it != n->children.end()) {
            n = it->second.get();
            continue;
        }

        if (!create_parent)
            return false;

        auto child = std::make_unique<Node>();
        Node* next = child.get();
        n->children.emplace(seg, std::move(child));
        n = next;
    }

    n->value = value;

    return true;
}

template <typename T>
bool String_Trie<T>::insert(std::string_view key, T&& value, bool create_parent)
{
    const auto segments = split_key(key);

    Node* n = root_.get();
    for (const auto& seg : segments) {
        auto it = n->children.find(seg);
        if (it != n->children.end()) {
            n = it->second.get();
            continue;
        }

        if (!create_parent)
            return false;

        auto child = std::make_unique<Node>();
        Node* next = child.get();
        n->children.emplace(seg, std::move(child));
        n = next;
    }

    n->value = std::move(value);

    return true;
}

template <typename T>
bool String_Trie<T>::contains(std::string_view key) const
{
    return find_internal(key) != nullptr;
}

template <typename T>
std::optional<T> String_Trie<T>::find(std::string_view key) const
{
    const Node* n = find_internal(key);
    if (n) {
        return n->value;
    }

    return std::nullopt;
}

template <typename T>
std::vector<T> String_Trie<T>::find_chains(std::string_view key) const
{
    std::vector<T> res;

    const auto segments = split_key(key);

    const Node* n = root_.get();
    for (const auto& seg : segments) {
        const auto it = n->children.find(seg);
        if (it == n->children.end())
            break;

        n = it->second.get();
        if (n->value) {
            res.push_back(*n->value);
        }
    }

    return res;
}

template <typename T>
bool String_Trie<T>::erase(std::string_view key)
{
    const auto segments = split_key(key);
    std::vector<Node*> nodes;
    nodes.reserve(segments.size() + 1);
    nodes.push_back(root_.get());

    Node* n = root_.get();
    for (const auto& seg : segments) {
        const auto it = n->children.find(seg);
        if (it == n->children.end())
            return false;

        n = it->second.get();
        nodes.push_back(n);
    }

    if (!n->value)
        return false;

    n->value.reset();

    for (size_t i = segments.size(); i > 0; --i) {
        Node* child = nodes[i];
        if (child->value || !child->children.empty())
            break;

        nodes[i - 1]->children.erase(segments[i - 1]);
    }

    return true;
}

template <typename T>
bool String_Trie<T>::erase_prefix(std::string_view key)
{
    const auto segments = split_key(key);
    std::vector<Node*> nodes;
    nodes.reserve(segments.size() + 1);
    nodes.push_back(root_.get());

    Node* n = root_.get();
    for (const auto& seg : segments) {
        const auto it = n->children.find(seg);
        if (it == n->children.end())
            return false;

        n = it->second.get();
        nodes.push_back(n);
    }

    if (!n->value && n->children.empty())
        return false;

    if (segments.empty()) {
        clear();
        return true;
    }

    nodes[segments.size() - 1]->children.erase(segments.back());

    for (size_t i = segments.size() - 1; i > 0; --i) {
        Node* child = nodes[i];
        if (child->value || !child->children.empty())
            break;

        nodes[i - 1]->children.erase(segments[i - 1]);
    }

    return true;
}

template <typename T>
void String_Trie<T>::clear()
{
    if (root_) {
        root_->value.reset();
        root_->children.clear();
    }
}

template <typename T>
std::vector<std::string> String_Trie<T>::split_key(std::string_view key) const
{
    std::vector<std::string> res;
    if (key.empty())
        return res;

    size_t pos = 0;
    const auto size = key.size();
    while (pos < size) {
        while (pos < size && key[pos] == sep_)
            ++pos;

        if (pos >= size)
            break;

        size_t end = pos;
        while (end < size && key[end] != sep_)
            ++end;

        res.emplace_back(key.substr(pos, end - pos));
        pos = end;
    }

    return res;
}

template <typename T>
String_Trie<T>::Node* String_Trie<T>::find_internal(std::string_view key)
{
    const auto segments = split_key(key);

    Node* n = root_.get();
    for (const auto& seg : segments) {
        const auto it = n->children.find(seg);
        if (it == n->children.end())
            return nullptr;

        n = it->second.get();
    }

    return n;
}

template <typename T>
const typename String_Trie<T>::Node* String_Trie<T>::find_internal(std::string_view key) const
{
    const auto segments = split_key(key);

    const Node* n = root_.get();
    for (const auto& seg : segments) {
        const auto it = n->children.find(seg);
        if (it == n->children.end())
            return nullptr;

        n = it->second.get();
    }

    return n;
}
