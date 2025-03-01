#ifndef H_COM_TASK
#define H_COM_TASK

#include <functional>
#include <thread>
#include <stdexcept>

#ifndef _WIN32
#define jthread thread
#endif

namespace Utils
{
    struct Task{
    public:
        void accomplish(std::function<void(const bool&)> work);
        void cancel();
        void await();
        bool is_running();

    private:
        std::jthread worker;
        bool is_active = false;
        bool cancel_requested;
    };
} // namespace Utils

#endif