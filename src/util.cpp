#include "../include/util.hpp"
#include "../include/image.hpp"
#include "../lib/json.hpp"
using json = nlohmann::json;

// Inserts jobs to the queue
void Queue::insertJobsToQueue(std::vector<ImageInstruction> instructions) {
	for (auto& instruction : instructions) {
		jobs.push_back(instruction);
	}
}

// Parses the JSON file and returns a vector of ImageInstruction
void from_json(const json& j, ImageInstruction& p) {
    j.at("inPath").get_to(p.inPath);
    j.at("outPath").get_to(p.outPath);
    j.at("effects").get_to(p.effects);
}

// Parses the effects requirements from the file
std::vector<ImageInstruction> ImageProcessorUtil::parseEffectsRequirements(std::string filename, std::string dataDir) {
    std::vector<ImageInstruction> instructions;
    std::vector<std::string> dataDirs;
    std::istringstream ss(dataDir);
    std::string dir;

    // Split the data directory string by '+'
    while (std::getline(ss, dir, '+')) {
        dataDirs.push_back(dir);
    }

    // Open the file
    std::ifstream effectsFile(filename);
    if (!effectsFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return instructions;
    }

    std::string line;
    // Parse the JSON file
    while (std::getline(effectsFile, line)) {
        json j = json::parse(line, nullptr, false);
        if (j.is_discarded()) {
            std::cerr << "Error parsing JSON: " << line << std::endl;
            continue;
        }

        for (const std::string dir : dataDirs) {
            ImageInstruction imgInst = j.get<ImageInstruction>();
            imgInst.inDir = dir;
            instructions.push_back(imgInst);
        }
    }

    return instructions;
}

void ImageProcessorUtil::runImageProcessing(ImageInstruction instruction, bool parallel, int numThreads) {
    // Set the input and output paths
    std::string outPath = instruction.outPath;
    std::string inPath = instruction.inPath;
    std::string dataDir = instruction.inDir;

    std::string fullInPath = "../data/in/" + dataDir + "/" + inPath;
    Image img(fullInPath);

    std::string fullOutPath = "../data/out/" + dataDir + "_" + outPath;

    // Apply the effects according to the instruction
    for (int i = 0; i < instruction.effects.size(); i++) {
        std::string effect = instruction.effects[i];
        // Apply greyscale effect
        if (effect == "G") {
            if (parallel) {
                img.convolutionParallel("G", numThreads);
            } else {
                img.convolution("G");
            }
        // Apply edge detection effect
        } else if (effect == "E") {
            if (parallel) {
                img.convolutionParallel("E", numThreads);
            } else {
                img.convolution("E");
            }
        // Apply sharpen effect
        } else if (effect == "S") {
            if (parallel) {
                img.convolutionParallel("S", numThreads);
            } else {
                img.convolution("S");
            }
        // Apply blur effect
        } else if (effect == "B") {
            if (parallel) {
                img.convolutionParallel("B", numThreads);
            } else {
                img.convolution("B");
            }
        }

        // If there are more effects, continue to swap the in and out pointers
        if (i != instruction.effects.size() - 1) {
            img.swapInOutPointers();
        }
    }

    // Make sure to swap the in and out pointers if there are no effects
    if (instruction.effects.empty()) {
        img.swapInOutPointers();
    }

    // Save the image
    img.save(fullOutPath);
}
