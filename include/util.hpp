#include <string>
#include <vector>
#include <list>
#include <atomic>
#include <mutex>

// ImageInstruction stores where to get the file, where to store the output file, the effects
// to be applied and the input directory to get the input image from (i.e. big, mixture or small)
struct ImageInstruction {
    std::string inPath;
    std::string outPath;
    std::vector<std::string> effects;
    std::string inDir;
};

class TASLock {
    std::atomic<bool> state;

public:
    void lock();
    void unlock();
};

class Queue {
    TASLock lock;
    std::list<ImageInstruction> jobs;

public:
    void insertJobsToQueue(std::vector<ImageInstruction> instructions);
	bool isEmpty() const { return jobs.empty(); }
    ImageInstruction front() { return jobs.front(); }
    ImageInstruction pop_front() { ImageInstruction job = jobs.front(); jobs.pop_front(); return job; }
	int size() { return jobs.size(); }
	const std::list<ImageInstruction>& getJobs() const { return jobs; }
    void lockQueue() { lock.lock(); }
    void unlockQueue() { lock.unlock(); }
};

class ImageProcessorUtil {
public:
    static std::vector<ImageInstruction> parseEffectsRequirements(std::string filename, std::string dataDir);
    static void runImageProcessing(ImageInstruction instruction, bool parallel, int numThreads);
};