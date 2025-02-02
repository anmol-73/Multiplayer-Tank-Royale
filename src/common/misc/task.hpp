#ifndef H_COM_TASK
#define H_COM_TASK

#include <functional>
#include <thread>
#include <stdexcept>

namespace Utils
{
    struct Task{
    public:
        void accomplish(std::function<void(const bool&)> work);
        void cancel();
        void await();
        bool is_running();

    private:
        std::thread worker;
        bool is_active = false;
        bool cancel_requested;
    };
} // namespace Utils

#endif