#pragma once
#include <atomic>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <utility>

/* ThreadSafeQueue | Fast size check | Multiple Producers - Single Consumer */

template<typename T>
class ThreadSafeQueueMPSC final
{
public:
    ThreadSafeQueueMPSC() = default;
    ThreadSafeQueueMPSC(const ThreadSafeQueueMPSC&) = delete;
    ThreadSafeQueueMPSC(ThreadSafeQueueMPSC&&) = delete;
    ThreadSafeQueueMPSC& operator=(const ThreadSafeQueueMPSC&) = delete;
    ThreadSafeQueueMPSC operator=(ThreadSafeQueueMPSC&&) = delete;

    void push(const T& val)
    {
        const std::lock_guard guard(mutex);
        container.push(val);
        size.fetch_add(1, std::memory_order_relaxed);
    }

    void push(T&& val)
    {
        const std::lock_guard guard(mutex);
        container.push(std::move(val));
        size.fetch_add(1, std::memory_order_relaxed);
    }

    [[nodiscard]] T get_front_and_pop()
    {
        const std::lock_guard guard(mutex);
        if (container.empty())
        {
            throw std::out_of_range("Queue is empty");
        }

        T ret = std::move(container.front());
        container.pop();
        size.fetch_sub(1, std::memory_order_relaxed);
        return ret;
    }

    [[nodiscard]] bool is_empty() const
    {
        return size.load(std::memory_order_relaxed) == 0;
    }

private:
    std::queue<T> container;
    std::mutex mutex;
    std::atomic_size_t size = 0;
};
