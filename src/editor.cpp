#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "../include/scheduler.hpp"

int main(int argc, char* argv[]) {
        // Check if the number of arguments is correct
        if (argc < 2) {
                std::cout << "Usage: editor data_dir mode [number of threads]\n"
                        << "data_dir = The data directory to use to load the images.\n"
                        << "mode     = (s) run sequentially, (parfiles) process multiple files in parallel, (parslices) process slices of each image in parallel \n"
                        << "[number of threads] = Runs the parallel version of the program with the specified number of threads.\n";
                return 1;
        }
        
        Config config;
        config.dataDirs = argv[2];
        if (argc >= 4) {
                // Multi-threaded mode
                config.mode = argv[3];
                config.threadCount = std::stoi(argv[4]);
        } else {
                // Single-threaded mode
                config.mode = "s";
                config.threadCount = 1;
        }

        // Time the program
        auto start = std::chrono::high_resolution_clock::now();
        Schedule schedule(config);
        // Run the scheduler
        schedule.Run();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << diff.count() << "\n";

        return 0;
}
