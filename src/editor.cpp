#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "../include/scheduler.hpp"

int main(int argc, char* argv[]) {
        if (argc < 2) {
                std::cout << "Usage: editor data_dir mode [number of threads]\n"
                        << "data_dir = The data directory to use to load the images.\n"
                        << "mode     = (s) run sequentially, (parfiles) process multiple files in parallel, (parslices) process slices of each image in parallel \n"
                        << "[number of threads] = Runs the parallel version of the program with the specified number of threads.\n";
                return 1;
        }
        
        Config config;
        for (int i = 0; i < argc; ++i) {
                std::cout << "Argument " << i << ": " << argv[i] << "\n";
        }
        config.dataDirs = argv[2];
        if (argc >= 4) {
                config.mode = argv[3];
                config.threadCount = std::stoi(argv[4]);
        } else {
                config.mode = "s";
                config.threadCount = 1;
        }

        auto start = std::chrono::high_resolution_clock::now();
        Schedule schedule(config);
        schedule.Run();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end-start;
        std::cout << "Total runtime: " << diff.count() << "s\n";

        return 0;
}
