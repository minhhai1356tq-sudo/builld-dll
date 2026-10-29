#pragma once
#include <atomic>
#include <thread>

namespace SpinBot {
    class SpinBotExploit {
    public:
        static void Start();
        static void Stop();

        static std::atomic<bool> running;
        static std::thread       thread;
    };
}