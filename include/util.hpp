#include <string>
#include <vector>
#include <list>
#include <atomic>
#include <mutex>
#include <iostream>
#include <fstream>
#include <sstream>

// ImageInstruction stores where to get the file, where to store the output file, the effects
// to be applied and the input directory to get the input image from (i.e. big, mixture or small)
struct ImageInstruction {
    std::string inPath;
    std::string outPath;
    std::vector<std::string> effects;
    std::string inDir;
};

// Queue class that stores the jobs to be processed and lock
class Queue {
    // Lock to protect the queue
    std::mutex lock;
    // List of jobs to be processed
    std::list<ImageInstruction> jobs;
public:
    // function to insert jobs to the queue/
    void insertJobsToQueue(std::vector<ImageInstruction> instructions);
    // function to check if the queue is empty
	bool isEmpty() const { return jobs.empty(); }
    // function to get the front job
    ImageInstruction front() { return jobs.front(); }
    // function to pop the front job
    ImageInstruction pop_front() { ImageInstruction job = jobs.front(); jobs.pop_front(); return job; }
    // function to get the size of the queue
	int size() { return jobs.size(); }
    // function to get the list of jobs
	const std::list<ImageInstruction>& getJobs() const { return jobs; }
    // function to lock the queue
    void lockQueue() { lock.lock(); }
    // function to unlock the queue
    void unlockQueue() { lock.unlock(); }
};

// ImageProcessorUtil class that contains utility functions to parse the effects requirements
class ImageProcessorUtil {
public:
    // function to parse the effects requirements
    static std::vector<ImageInstruction> parseEffectsRequirements(std::string filename, std::string dataDir);
    // function to run the image processing
    static void runImageProcessing(ImageInstruction instruction, bool parallel, int numThreads);
};