#include <fstream>
#include <algorithm>
#include <thread>
#include <opencv2/opencv.hpp>
using namespace cv;

// Image class that represents an image with function to apply convolution effect
class Image {
public:
    cv::Mat in;  //The original pixels before applying the effect
    cv::Mat out; //The updated pixels after applying the effect
    
    // Constructor that reads the image from the file path
    Image(std::string filePath); 

    // Swaps the in and out pointers
    void swapInOutPointers(); 

    // Saves the image to the file path
    void save(std::string filePath); 

    // Clamps the value of the component to be between 0 and 255
    static double clamp(double comp); 

    // Applies the convolution effect to the image
    void convolution(std::string effect); 

    // Applies the convolution effect to the image in parallel
    void convolutionParallel(std::string effect, int numThreads); 
private:
    // Applies the convolution effect to the image in a specific range
    void convolutionBatchProcessing(int start, int end, std::string effect, bool greyscale, double *kernal, int filter[9][2], int width);
};