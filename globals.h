#ifndef GLOBALS_H
#define GLOBALS_H

#include <mutex>
#include <iostream>
#include <iomanip>

inline constexpr int PAGE_SIZE = 4096;

inline std::mutex cout_mutex;

inline void printTimestamp(std::string threadName, int eventDescription){
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm* time_info = std::localtime(&time);

    // Lock the mutex to ensure thread-safe printing
    std::lock_guard<std::mutex> lock(cout_mutex);

    // Print timestamp and event
    std::cout << "[" << std::put_time(time_info, "%Y-%m-%d %H:%M:%S") << "] Thread " << threadName << ": " << eventDescription << "\n";
}

#endif
