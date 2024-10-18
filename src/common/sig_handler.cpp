#include "sig_handler.hpp"

std::function<void()> SignalHandler::ctrlC_callback;

#ifdef _WIN32
BOOL WINAPI SignalHandler::HandlerRoutine(_In_ DWORD dwCtrlType)
{
    switch (dwCtrlType)
    {
    case CTRL_C_EVENT:
        if (ctrlC_callback != nullptr){
            ctrlC_callback();
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
        if (SignalHandler::ctrlC_callback != nullptr) {
            SignalHandler::ctrlC_callback(); 
        }
    }
}
#endif

void SignalHandler::initialize()
{
    #ifdef _WIN32
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);
    #else
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);
    #endif
}