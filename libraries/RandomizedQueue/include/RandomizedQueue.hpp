#pragma once

#include <algorithm>
#include <random>
#include <vector>

template <class T>
class RandomizedQueue {
    template <bool Const = false>
    class QueueIterator {
        friend class RandomizedQueue;

    public:
        using iterator_category = std::forward_iterator_tag;
        using difference_type   = std::ptrdiff_t;
        using value_type        = std::conditional_t<Const, const T, T>;
        using pointer           = value_type *;
        using reference         = value_type &;

        using storage = std::conditional_t<Const, const std::vector<T> *, std::vector<T> *>;

        QueueIterator() = default;

    private:
        size_t curr_ind_{};
        storage elements_{};
        std::vector<size_t> indexes_;

        explicit QueueIterator(storage elements, const size_t curr_ind = 0)
            : curr_ind_(curr_ind), elements_(elements), indexes_(elements->size()) {
            for (size_t i = 0; i < indexes_.size(); i++) {
                indexes_[i] = i;
            }

            std::shuffle(indexes_.begin(), indexes_.end(), RandomizedQueue::rnd_);
        }

    public:
        [[nodiscard]] reference operator*() const { return elements_->operator[](indexes_[curr_ind_]); }

        [[nodiscard]] pointer operator->() const { return &operator*(); }

        QueueIterator &operator++() {
            curr_ind_++;
            return *this;
        }

        QueueIterator operator++(int) {
            auto result = *this;
            ++*this;
            return result;
        }

        QueueIterator &operator--() {
            curr_ind_--;
            return *this;
        }

        QueueIterator operator--(int) {
            auto result = *this;
            --*this;
            return result;
        }

        friend bool operator==(const QueueIterator &left, const QueueIterator &right) {
            return left.curr_ind_ == right.curr_ind_;
        }

        friend bool operator!=(const QueueIterator &left, const QueueIterator &right) { return !(left == right); }
    };

    std::vector<T> elements_;

    thread_local static inline std::default_random_engine rnd_{std::random_device{}()};

    static size_t get_random_index_(const size_t from, const size_t to) {
        std::uniform_int_distribution dist(from, to - 1);
        return dist(rnd_);
    }

public:
    using Iterator      = QueueIterator<>;
    using ConstIterator = QueueIterator<true>;

    void enqueue(T element) { elements_.emplace_back(std::move(element)); }

    [[nodiscard]] const T &sample() const { return elements_[get_random_index_(0, size())]; }

    T dequeue() {
        const auto rand_index = get_random_index_(0, size());
        T result              = std::move(elements_[rand_index]);

        std::swap(elements_[rand_index], elements_.back());
        elements_.pop_back();

        return result;
    }

    [[nodiscard]] bool empty() const { return elements_.empty(); }

    [[nodiscard]] size_t size() const { return elements_.size(); }

    [[nodiscard]] Iterator begin() { return Iterator{&elements_}; }

    [[nodiscard]] Iterator end() { return Iterator{&elements_, size()}; }

    [[nodiscard]] ConstIterator begin() const { return ConstIterator{&elements_}; }

    [[nodiscard]] ConstIterator end() const { return ConstIterator{&elements_, size()}; }

    [[nodiscard]] ConstIterator cbegin() const { return begin(); }

    [[nodiscard]] ConstIterator cend() const { return end(); }
};
