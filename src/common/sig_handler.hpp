#ifdef _WIN32
#include <windows.h>
#endif
#include <functional>

namespace SignalHandler
{
    #ifdef _WIN32
    BOOL WINAPI HandlerRoutine(_In_ DWORD dwCtrlType);
    #endif

    void initialize();
    extern std::function<void()> ctrlC_callback;
} // namespace SignalHandler


