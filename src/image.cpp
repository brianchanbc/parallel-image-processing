#include "../include/image.hpp"

Image::Image(std::string filePath) {
	in = cv::imread(filePath, cv::IMREAD_UNCHANGED);
	out = cv::Mat::zeros(in.size(), in.type());
}

void Image::swapInOutPointers() {
	std::swap(in, out);
}

void Image::save(std::string filePath) {
	cv::imwrite(filePath, out);
}

double Image::clamp(double comp) {
	return std::min(255.0, std::max(0.0, comp));
}

void Image::convolution(std::string effect) {
	double sharpenKernal[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	double edgeDetectionKernal[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	double blurKernal[9] = {1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0};
	double *kernal;
	int filter[9][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	bool greyscale = false;
	if (effect == "E") {
		kernal = edgeDetectionKernal;
	} else if (effect == "S") {
		kernal = sharpenKernal;
	} else if (effect == "B") {
		kernal = blurKernal;
	} else if (effect == "G") {
		greyscale = true;
	}
	for (int y = 0; y < in.rows; y++) {
		for (int x = 0; x < in.cols; x++) {
			if (greyscale) {
				cv::Vec4b pixel = in.at<cv::Vec4b>(y, x);
				double b = pixel[0];
				double g = pixel[1];
				double r = pixel[2];
				double a = pixel[3];
				double greyC = clamp((r + g + b) / 3);
				out.at<cv::Vec4b>(y, x) = cv::Vec4b(greyC, greyC, greyC, a);
			} else {
				cv::Vec4b pixel = in.at<cv::Vec4b>(y, x);
				double a = pixel[3];
				double redC = 0;
				double greenC = 0;
				double blueC = 0;
				for (int k = 0; k < 9; k++) {
					int newY = y + filter[k][0];
					int newX = x + filter[k][1];
					if (newY >= 0 && newY < in.rows && newX >= 0 && newX < in.cols) {
						cv::Vec4b pixel = in.at<cv::Vec4b>(newY, newX);
						double b = pixel[0];
						double g = pixel[1];
						double r = pixel[2];
						redC += r * kernal[k];
						greenC += g * kernal[k];
						blueC += b * kernal[k];
					}
				}
				double red = clamp(redC);
				double green = clamp(greenC);
				double blue = clamp(blueC);
				out.at<cv::Vec4b>(y, x) = cv::Vec4b(blue, green, red, a);
			}
		}
	}
}

void Image::convolutionParallel(std::string effect, int numThreads) {
	double sharpenKernal[9] = {0, -1, 0, -1, 5, -1, 0, -1, 0};
	double edgeDetectionKernal[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	double blurKernal[9] = {1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0, 1 / 9.0};
	double *kernal;
	int filter[9][2] = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 0}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};
	bool greyscale = false;
	if (effect == "E") {
		kernal = edgeDetectionKernal;
	} else if (effect == "S") {
		kernal = sharpenKernal;
	} else if (effect == "B") {
		kernal = blurKernal;
	} else if (effect == "G") {
		greyscale = true;
	}
	int numElements = in.rows * in.cols;
	int width = in.cols;
	int numElementsPerThread = ceil((double)numElements / numThreads);
	std::vector<std::thread> threads;
	for (int i = 0; i < numThreads; i++) {
		int chunkStart = numElementsPerThread * i;
		if (chunkStart > numElements) {
			chunkStart = numElements;
		}
		int chunkEnd = chunkStart + numElementsPerThread;
		if (chunkEnd > numElements) {
			chunkEnd = numElements;
		}
		threads.push_back(std::thread([this](int start, int end, std::string effect, bool greyscale, double *kernal, int filter[9][2], int width) {
			this->convolutionBatchProcessing(start, end, effect, greyscale, kernal, filter, width);
		}, chunkStart, chunkEnd, effect, greyscale, kernal, filter, width));
	}
	for (int i = 0; i < numThreads; i++) {
		threads[i].join();
	}
}

void Image::convolutionBatchProcessing(int start, int end, std::string effect, bool greyscale, double *kernal, int filter[9][2], int width) {
	for (int i = start; i < end; i++) {
		int y = i / width;
		int x = i % width;
		if (greyscale) {
			cv::Vec4b pixel = in.at<cv::Vec4b>(y, x);
			double b = pixel[0];
			double g = pixel[1];
			double r = pixel[2];
			double a = pixel[3];
			double greyC = clamp((r + g + b) / 3);
			out.at<cv::Vec4b>(y, x) = cv::Vec4b(greyC, greyC, greyC, a);
		} else {
			cv::Vec4b pixel = in.at<cv::Vec4b>(y, x);
			double a = pixel[3];
			double redC = 0;
			double greenC = 0;
			double blueC = 0;
			for (int k = 0; k < 9; k++) {
				int newY = y + filter[k][0];
				int newX = x + filter[k][1];
				if (newY >= 0 && newY < in.rows && newX >= 0 && newX < in.cols) {
					cv::Vec4b pixel = in.at<cv::Vec4b>(newY, newX);
					double b = pixel[0];
					double g = pixel[1];
					double r = pixel[2];
					redC += r * kernal[k];
					greenC += g * kernal[k];
					blueC += b * kernal[k];
				}
			}
			double red = clamp(redC);
			double green = clamp(greenC);
			double blue = clamp(blueC);
			out.at<cv::Vec4b>(y, x) = cv::Vec4b(blue, green, red, a);
		}
	}
}

