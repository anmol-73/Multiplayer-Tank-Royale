#include "sig_handler.hpp"
#include "stdio.h"

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

void SignalHandler::initialize()
{
    SetConsoleCtrlHandler(HandlerRoutine, TRUE);
}
#endif