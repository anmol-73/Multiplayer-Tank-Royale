#include "task.hpp"

void Utils::Task::accomplish(std::function<void(const bool &)> work)
{
    if (is_active) throw std::runtime_error("Cannot run new task without finishing the previous one!");
    await(); // Just to join the thread if it is joinable
    
    cancel_requested = false;
    is_active = true;
    worker = std::thread([work, this](){
        work(this->cancel_requested);
        this->is_active = false;
    });
}

void Utils::Task::cancel()
{
    cancel_requested = true;
}

void Utils::Task::await()
{
    if (worker.joinable()) worker.join();
}
bool Utils::Task::is_running(){ return is_active; }
