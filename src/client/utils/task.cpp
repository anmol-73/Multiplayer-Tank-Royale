#include "task.hpp"

void Communication::Task::accomplish(std::function<void(const bool &)> work)
{
    if (is_active) throw std::runtime_error("Cannot run new task without finishing the previous one!");
    cancel_requested = false;
    is_active = true;
    worker = std::thread([&work, this](){
        work(this->cancel_requested);
        this->is_active = false;
    });
}

void Communication::Task::cancel()
{
    cancel_requested = true;
}

void Communication::Task::await()
{
    if (!(is_active && worker.joinable())) return;
    worker.join();
}
inline bool Communication::Task::is_running(){ return is_active; }
