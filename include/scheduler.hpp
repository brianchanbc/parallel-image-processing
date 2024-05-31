#include <string>
#include <list>
#include <mutex>
#include <algorithm>
#include <chrono>
#include <thread>
#include <vector>

struct Config {
	std::string dataDirs; //Represents the data directories to use to load the images.
	std::string mode; // Represents which scheduler scheme to use
	int threadCount;    // Runs parallel version with the specified number of threads
};

class Schedule {
public:
    Schedule(const Config& config);
    void Run();

private:
    void RunSequential();
    void RunParallelFiles();
    void RunParallelSlices();

    Config config_;
};
