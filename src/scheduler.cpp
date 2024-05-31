#include "../include/scheduler.hpp"
#include "../include/util.hpp"

// Set up the configuration value
Schedule::Schedule(const Config& config) : config_(config) {}

// Run the correct version based on the Mode field of the configuration value
void Schedule::Run() {
    if (config_.mode == "s") {
		RunSequential();
	} else if (config_.mode == "parfiles") {
		RunParallelFiles();
	} else if (config_.mode == "parslices") {
		RunParallelSlices();
	} else {
		throw std::invalid_argument("Invalid scheduling scheme given.");
	}
}

void Schedule::RunSequential() {
    // This function runs image convolution processing sequentially
    std::string dataDir = config_.dataDirs;
    // Parse requirements from the file and get the instructions based on the data directories requested
    auto instructions = ImageProcessorUtil::parseEffectsRequirements("../data/effects.txt", dataDir);
    for (const auto& instruction : instructions) {
        // Run image processing for each image, false means no parallelization and 1 means 1 thread
        ImageProcessorUtil::runImageProcessing(instruction, false, 1);
    }
}

void Schedule::RunParallelFiles() {
    // This function run image convolution in multiple images in parallel but only have 1 thread on each image
    int maxThreads = config_.threadCount;
    std::string dataDir = config_.dataDirs;
    // Parse instructions slice
    std::vector<ImageInstruction> instructions = ImageProcessorUtil::parseEffectsRequirements("../data/effects.txt", dataDir);
    // Insert jobs to doubly linked list queue sequentially
	Queue queue;
    queue.insertJobsToQueue(instructions);
    // Set up threads
    std::vector<std::thread> threads;
    // Spawn the threads
    for (int i = 0; i < maxThreads; i++) {
        // Launch thread concurrently
        threads.push_back(std::thread([&queue]() { 
            while (!queue.isEmpty()) {
                // Try to acquire lock
                queue.lockQueue();
                // If success, take the job out of the queue
                if (!queue.isEmpty()) {
                    ImageInstruction task = queue.pop_front();
                    queue.unlockQueue();
                    // Run image processing for each image, true means parallelization and 1 means 1 thread
                    ImageProcessorUtil::runImageProcessing(task, false, 1);
                } else {
                    // If the queue is empty, unlock the queue and return
                    queue.unlockQueue();
                }
            }
        }));
    }
    // Wait for all threads to finish
    for (int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }
}

void Schedule::RunParallelSlices() {
    // This function runs image convolution sequentially but parallelize in each image processing
    // Get thread count and data directory we concerned about
    int numThreads = config_.threadCount;
    std::string dataDir = config_.dataDirs;
    // Parse instructions slice
    auto instructions = ImageProcessorUtil::parseEffectsRequirements("../data/effects.txt", dataDir);
    // Insert jobs to doubly linked list queue sequentially
	Queue queue;
    queue.insertJobsToQueue(instructions);
    // Iterate the queue
    for (auto& job : queue.getJobs()) {
        // Run a job on each element in the queue
        ImageProcessorUtil::runImageProcessing(job, true, numThreads);
    }
}