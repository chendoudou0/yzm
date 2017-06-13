// Copyright 2017 (c) cloudream.
// All rights reserved.
// 
// Author: wangjw <wangjw@cloudream.com> 2017/04/10
#ifndef COMM_MONITOR_H
#define COMM_MONITOR_H 
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace comm {

class Monitor {
 public:
  Monitor() = default;
  ~Monitor() = default;

  Monitor(const Monitor&) = delete;
  Monitor& operator=(const Monitor&) = delete;

  void Wait() {
    std::unique_lock<std::mutex> l(mtx_);
    while (!notified_) {
      cond_.wait(l);
    }

    // release notify flag
    notified_ = false;
  }

  void WaitFor(int32_t ms) {
    std::unique_lock<std::mutex> l(mtx_);
    cond_.wait_for(l, std::chrono::milliseconds(ms), [this]() {
        return notified_;});

    // release notify flag
    notified_ = false;
  }

  void Notify() {
    std::unique_lock<std::mutex> l(mtx_);
    notified_ = true;
    cond_.notify_one();
  }

  void NotifyAll() {
    std::unique_lock<std::mutex> l(mtx_);
    notified_ = true;
    cond_.notify_all();
  }
 private:
  bool notified_{false};
  std::mutex mtx_;
  std::condition_variable cond_;
};

}

#endif // COMM_MONITOR_H
