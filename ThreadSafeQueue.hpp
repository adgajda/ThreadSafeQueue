#pragma once
#include <atomic>
#include <deque>
#include <mutex>
#include <stdexcept>
#include <utility>

template <typename T> class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;

  void push(const T &val) {
    const std::lock_guard guard(mtx);
    container.push_back(val);
    size.fetch_add(1, std::memory_order_relaxed);
  }

  void push(T &&val) {
    const std::lock_guard guard(mtx);
    container.push_back(std::move(val));
    size.fetch_add(1, std::memory_order_relaxed);
  }

  [[nodiscard]] T pop() {
    const std::lock_guard guard(mtx);
    if (container.empty()) {
      throw std::out_of_range("Queue is empty");
    }

    T ret = std::move(container.front());
    container.pop_front();
    size.fetch_sub(1, std::memory_order_relaxed);
    return ret;
  }

  [[nodiscard]] bool empty() const { return size == 0; }

private:
  std::deque<T> container;
  std::mutex mtx;
  std::atomic_size_t size = 0;
};
