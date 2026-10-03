#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <ostream>
#include <thread>

#include "container/RingBuffer.hpp"

namespace sb::logger::config {

struct Async {};

class AsyncQueue {
 public:
  using Task = std::function<void(std::ostream&)>;
  static constexpr size_t kCapacity = 1024;

  explicit AsyncQueue(std::ostream& stream) : stream_(stream) {
    worker_thread_ = std::thread([this]() { run(); });
  }

  ~AsyncQueue() {
    stop();
  }

  void push(Task task) {
    while (!ring_buffer_.try_push(task)) {
      std::this_thread::yield();
    }
    {
      std::scoped_lock lock{mutex_};
      ++pending_count_;
    }
    cv_.notify_one();
  }

  void flush() {
    std::unique_lock lock{mutex_};
    cv_.wait(lock, [this]() { return pending_count_ == 0; });
    stream_ << std::flush;
  }

  void stop() {
    {
      std::scoped_lock lock{mutex_};
      if (!running_) {
        return;
      }
      running_ = false;
    }
    cv_.notify_all();
    if (worker_thread_.joinable()) {
      worker_thread_.join();
    }
  }

 private:
  void run() {
    while (true) {
      Task task;
      bool popped = false;
      {
        std::unique_lock lock{mutex_};
        cv_.wait(lock, [this, &task, &popped]() {
          popped = ring_buffer_.try_pop(task);
          return popped || !running_;
        });

        if (!running_ && !popped && pending_count_ == 0) {
          break;
        }
      }

      if (popped && task) {
        task(stream_);
        {
          std::scoped_lock lock{mutex_};
          --pending_count_;
        }
        cv_.notify_all();
      }
    }
    stream_ << std::flush;
  }

  std::ostream& stream_;
  sb::container::RingBuffer<Task, kCapacity> ring_buffer_;
  size_t pending_count_{0};
  std::mutex mutex_;
  std::condition_variable cv_;
  std::atomic<bool> running_{true};
  std::thread worker_thread_;
};

}  // namespace sb::logger::config
