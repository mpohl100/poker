#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <mutex>

namespace par{

class Scheduler{
public:
    using Task = std::function<void()>;

    Scheduler(size_t numThreads);
    Scheduler() = default;
    ~Scheduler() = default;
    Scheduler(Scheduler const&) = delete;
    Scheduler& operator=(Scheduler const&) = delete;
    Scheduler(Scheduler&&) = delete;
    Scheduler& operator=(Scheduler&&) = delete;

    void run(size_t i);
    void cancel();
    void appendTasks(std::vector<Task> const& tasks);

private:
    // fits together index based
    std::vector<std::jthread> threads_;
    std::vector<std::vector<Task>> tasks_;
    std::mutex mutex_;
    bool cancel_ = false;
};

}