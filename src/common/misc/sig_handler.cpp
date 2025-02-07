#include "sig_handler.hpp"

std::function<void()> SignalHandler::ctrl_c_callback;

#ifdef _WIN32
BOOL WINAPI SignalHandler::win_handler_routine(_In_ DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
        if (ctrl_c_callback != nullptr){
            ctrl_c_callback();
        }
        // Signal is handled - don't pass it on to the next handler
        return true;
    default:
        // Pass signal on to the next handler
        return false;
    }
}
#else
void SignalHandler::my_handler(int signum)
{
     if (signum == SIGINT) {
        if (SignalHandler::ctrl_c_callback != nullptr) {
            SignalHandler::ctrl_c_callback(); 
        }
    }
}
#endif

void SignalHandler::initialize()
{
    #ifdef _WIN32
    SetConsoleCtrlHandler(win_handler_routine, TRUE);
    #else
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = unix_handler_routine;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    #endif
}