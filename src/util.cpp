#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/util.hpp"
#include "../include/image.hpp"
#include "../lib/json.hpp"
using json = nlohmann::json;

void TASLock::lock() {
	while (state.exchange(true)) {}
}

void TASLock::unlock() {
	state.exchange(false);
}

void Queue::insertJobsToQueue(std::vector<ImageInstruction> instructions) {
	for (auto& instruction : instructions) {
		jobs.push_back(instruction);
	}
}

void from_json(const json& j, ImageInstruction& p) {
    j.at("inPath").get_to(p.inPath);
    j.at("outPath").get_to(p.outPath);
    j.at("effects").get_to(p.effects);
}

std::vector<ImageInstruction> ImageProcessorUtil::parseEffectsRequirements(std::string filename, std::string dataDir) {
    std::vector<ImageInstruction> instructions;
    std::vector<std::string> dataDirs;
    std::istringstream ss(dataDir);
    std::string dir;

    while (std::getline(ss, dir, '+')) {
        dataDirs.push_back(dir);
    }

    std::ifstream effectsFile(filename);
    if (!effectsFile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return instructions;
    }

    std::string line;
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
    std::string outPath = instruction.outPath;
    std::string inPath = instruction.inPath;
    std::string dataDir = instruction.inDir;

    std::string fullInPath = "../data/in/" + dataDir + "/" + inPath;
    Image img(fullInPath);

    std::string fullOutPath = "../data/out/" + dataDir + "_" + outPath;
    std::cout << "Image size: " << dataDir << std::endl;

    for (int i = 0; i < instruction.effects.size(); i++) {
        std::string effect = instruction.effects[i];

        if (effect == "G") {
            if (parallel) {
                img.convolutionParallel("G", numThreads);
            } else {
                img.convolution("G");
            }
        } else if (effect == "E") {
            if (parallel) {
                img.convolutionParallel("E", numThreads);
            } else {
                img.convolution("E");
            }
        } else if (effect == "S") {
            if (parallel) {
                img.convolutionParallel("S", numThreads);
            } else {
                img.convolution("S");
            }
        } else if (effect == "B") {
            if (parallel) {
                img.convolutionParallel("B", numThreads);
            } else {
                img.convolution("B");
            }
        }

        if (i != instruction.effects.size() - 1) {
            img.swapInOutPointers();
        }
    }

    if (instruction.effects.empty()) {
        img.swapInOutPointers();
    }

    img.save(fullOutPath);
}
