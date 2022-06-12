#include "Scheduler.h"

#include <chrono>
#include <optional>

namespace par{

Scheduler::Scheduler(size_t numThreads){
    threads_ = std::vector<std::jthread>{numThreads};
    tasks_ = std::vector<std::vector<Task>>{numThreads};
    size_t index = 0;
    for(auto& thread : threads_)
    {
        size_t i = index++;
        thread = std::jthread([this, i](){ this->run(i);});
    }
}

void Scheduler::run(size_t index)
{
    for(;;){
        std::optional<Task> task = std::nullopt;
        if(!tasks_[index].empty())
        {
            std::lock_guard<std::mutex> lock(mutex_);
            task = std::make_optional(tasks_[index].front());
            tasks_[index].erase(tasks_[index].begin());
        }
        if(task)
            task->operator()();
        if(tasks_[index].empty())
        {
            using namespace std::chrono_literals;
            std::lock_guard<std::mutex> lock(mutex_);
            if(cancel_)
                break;
            else
                std::this_thread::sleep_for(250ms);
        }
    }
}

void Scheduler::cancel()
{
    cancel_ = true;
}

void Scheduler::appendTasks(std::vector<Task> const& tasks)
{
    std::lock_guard<std::mutex> lock(mutex_);
    size_t i = 0;
    for(const auto& task : tasks)
        tasks_[i++ % threads_.size()].push_back(task);
}

}