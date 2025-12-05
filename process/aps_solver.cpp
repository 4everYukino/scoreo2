#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

#include "rtlib/string_sprintf.h"

using namespace std;

/*
 * Used to simulate solver, consuming CPU and Memory.
 */
int main() {
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<int> time_dist(10, 30); /// 10s ~ 30s
    uniform_int_distribution<size_t> mem_dist(64, 1024); /// 64MB ~ 1024MB

    const int target_seconds = time_dist(gen);
    const size_t target_megabytes = mem_dist(gen);

    cout << string_sprintf("[Status] Begin ro run solver ...") << '\n'
         << string_sprintf("[Status]   Simulated runtime: %d seconds", target_seconds) << '\n'
         << string_sprintf("[Status]   Simulated memory: %u MB", target_megabytes) << '\n';

    try {
        vector<uint8_t> memory_buffer(target_megabytes * 1024 * 1024);

        // Use fill to prevent lazy allocation by the OS kernel
        fill(memory_buffer.begin(), memory_buffer.end(), 0xAA);

        this_thread::sleep_for(
            chrono::seconds(target_seconds)
        );

    } catch (const bad_alloc& e) {
        return -1;
    }

    cout << string_sprintf("[Status] Succeed to run solver, goodbye.") << '\n';

    return 0;
}
