#include <fstream>
#include <algorithm>
#include <thread>
#include <opencv2/opencv.hpp>
using namespace cv;

class Image {
public:
    cv::Mat in;  //The original pixels before applying the effect
    cv::Mat out; //The updated pixels after applying the effect

    Image(std::string filePath);

    void swapInOutPointers();

    void save(std::string filePath);

    static double clamp(double comp);

    void convolution(std::string effect);

    void convolutionParallel(std::string effect, int numThreads);
private:
    void convolutionBatchProcessing(int start, int end, std::string effect, bool greyscale, double *kernal, int filter[9][2], int width);
};