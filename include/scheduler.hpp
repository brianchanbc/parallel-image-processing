#include <string>
#include <list>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>

// Represents the configuration value for the program
struct Config {
	std::string dataDirs; //Represents the data directories to use to load the images.
	std::string mode; // Represents which scheduler scheme to use
	int threadCount; // Runs parallel version with the specified number of threads
};

// Class that schedules the image processing work based on the configuration provided
class Schedule {
public:
    // Constructor that sets up the configuration value
    Schedule(const Config& config);
    // Run the correct version based on the fields of the configuration 
    void Run();

private:
    // Run the image processing sequentially
    void RunSequential();
    // Run the image processing in parallel for multiple files
    void RunParallelFiles();
    // Run the image processing in parallel for slices of each image
    void RunParallelSlices();

    // Configuration value
    Config config_;
};
