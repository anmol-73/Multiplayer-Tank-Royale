#ifndef H_COM_SIG_HANDLER
#define H_COM_SIG_HANDLER

#ifdef _WIN32
#include <windows.h>
#undef near
#undef far
#undef min
#undef max
#endif
#include <functional>
#include <csignal>
#include <iostream>

namespace SignalHandler{
    #ifdef _WIN32
    BOOL WINAPI win_handler_routine(_In_ DWORD dwCtrlType);
    #else
    void unix_handler_routine(int signum);
    #endif

    /**
     * Does platform specific setup
     */
    void initialize();
    extern std::function<void()> ctrl_c_callback;
} // namespace SignalHandler

#endif