//
// Created by ucchi_mchxyz on 2021/11/30.
//

#ifndef MOKV_SKIP_LIST_TCC
#define MOKV_SKIP_LIST_TCC

#include "../util/arena.tcc"
#include "../util/const.tcc"
#include <chrono>
#include <cstddef>
#include <functional>
#include <random>

namespace MOKV {

template <class T, class Comparator>
class SkipList {
private:
    class Node {
        T dat_;
        size_t level_ = -1;
        Node* next_[SKIP_LIST_LEVEL] = { nullptr };

    public:
        Node() = default;

        Node(const T& dat, size_t level)
            : dat_(dat)
            , level_(level)
        {
        }

        const T& dat() const
        {
            return dat_;
        }

        bool isAfterCurNode(const T& dat) const;

        friend SkipList;
    };

    Node* head_ = new Node();
    size_t max_level_ = 0;

    std::mt19937 gen_ = std::mt19937(std::chrono::steady_clock::now().time_since_epoch().count());

    Arena arena_;

    Node* findGreatOrEqual(const T& dat, Node** prev) const;

    bool equal(const T& a, const T& b) const;

    size_t generateRandomLevel();

public:
    class NodeProxy {
        const T* dat_ = nullptr;
        bool valid_ = false;

    public:
        NodeProxy() = default;

        explicit NodeProxy(const T* dat)
            : dat_(dat)
            , valid_(true)
        {
        }

        bool isValid() const
        {
            return valid_;
        }

        const T& dat() const
        {
            return *dat_;
        }
    };

    class SkipListIterator {
    private:
        Node* cur_;

    public:
        SkipListIterator() = delete;

        explicit SkipListIterator(Node* cur)
            : cur_(cur)
        {
        }

        SkipListIterator(const SkipListIterator&) = default;

        bool hasNext()
        {
            return cur_->next_[0];
        }

        T& next()
        {
            cur_ = cur_->next_[0];
            return cur_->dat_;
        }
    };

    SkipList() = default;

    void insert(const T& dat);

    NodeProxy find(const T& dat) const;

    void erase(const T& dat);

    SkipListIterator iter()
    {
        return SkipListIterator { head_ };
    }

    size_t scan(std::function<bool(const T&)> scan_func) const;
};

template <class T, class Comparator>
bool SkipList<T, Comparator>::Node::isAfterCurNode(const T& dat) const
{
    return Comparator::compare(this->dat_, dat) < 0;
}

template <class T, class Comparator>
typename SkipList<T, Comparator>::Node* SkipList<T, Comparator>::findGreatOrEqual(const T& dat, Node** prev) const
{
    Node* head = head_;
    for (int level = max_level_ - 1; level >= 0; --level) {
        while (head->next_[level]) {
            auto ptr = head->next_[level];
            if (!ptr->isAfterCurNode(dat)) {
                break;
            }
            head = ptr;
        }
        if (nullptr != prev) {
            prev[level] = head;
        }
    }

    return head->next_[0];
}

template <class T, class Comparator>
bool SkipList<T, Comparator>::equal(const T& a, const T& b) const
{
    return 0 == Comparator::compare(a, b);
}

template <class T, class Comparator>
size_t SkipList<T, Comparator>::generateRandomLevel()
{
    size_t level = 1;
    while ((gen_() & 0xffffffff) > 0x3fffffff && level < SKIP_LIST_LEVEL) {
        ++level;
    }
    return level;
}

template <class T, class Comparator>
void SkipList<T, Comparator>::insert(const T& dat)
{
    Node *prev[SKIP_LIST_LEVEL], *node = findGreatOrEqual(dat, prev);
    if (nullptr != node && equal(node->dat_, dat)) {
        return;
    }

    size_t level = generateRandomLevel();
    if (level > max_level_) {
        for (size_t i = max_level_; i < level; ++i) {
            prev[i] = head_;
        }
        max_level_ = level;
    }

    Node* new_node = new (arena_.alignedAllocate(sizeof(Node))) Node(dat, level);
    for (size_t i = 0; i < level; ++i) {
        new_node->next_[i] = prev[i]->next_[i];
        prev[i]->next_[i] = new_node;
    }
}

template <class T, class Comparator>
typename SkipList<T, Comparator>::NodeProxy SkipList<T, Comparator>::find(const T& dat) const
{
    Node* node = findGreatOrEqual(dat, nullptr);
    if (nullptr != node && equal(node->dat_, dat)) {
        return NodeProxy(&(node->dat_));
    }

    return NodeProxy();
}

// implemented, but will be not used.
template <class T, class Comparator>
void SkipList<T, Comparator>::erase(const T& dat)
{
    Node *prev[SKIP_LIST_LEVEL], *node = findGreatOrEqual(dat, prev);
    if (nullptr == node || !equal(node->dat_, dat)) {
        return;
    }

    for (ssize_t i = node->level_ - 1; i >= 0; --i) {
        prev[i]->next_[i] = node->next_[i];
    }
    delete node;
}

template <class T, class Comparator>
size_t SkipList<T, Comparator>::scan(std::function<bool(const T&)> scan_func) const
{
    Node* head = head_;
    size_t cnt = 0;
    while (head->next_[0]) {
        auto ptr = head->next_[0];
        if (scan_func(ptr->dat_)) {
            ++cnt;
        }
        head = ptr;
    }
    return cnt;
}
}

#endif // MOKV_SKIP_LIST_TCC
