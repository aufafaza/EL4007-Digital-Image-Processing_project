#include <iostream>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "simple_cv_lib/image_processor.hpp"

int main(int argc, char ** argv)
{
  cv::Mat input;

  if (argc > 1) {
    input = cv::imread(argv[1], cv::IMREAD_COLOR);
    if (input.empty()) {
      std::cerr << "Could not read image: " << argv[1] << std::endl;
      return 1;
    }
  } else {
    input = cv::Mat(480, 640, CV_8UC3, cv::Scalar(30, 30, 30));
  }

  std::cout << "make custom kernel?\n";
  int make_kernel_user;
  std::cin >> make_kernel_user;

  simple_cv_lib::ImageProcessor processor;
  if (make_kernel_user){
    std::cout << "kernel size (odd, e.g. 3, 9, 15): ";
    int kernel_size;
    std::cin >> kernel_size;
    if (kernel_size % 2 == 0) {
      kernel_size += 1;
    }
    cv::Mat kernel = cv::Mat::zeros(kernel_size, kernel_size, CV_32FC1);

    for (int i = 0; i < kernel_size; i++){
      for (int j = 0; j < kernel_size; j++){
          std::cout << "Kernel at " << i << ", " << j << ":";
          std::cin >> kernel.at<float>(i, j);
        }
    }
    float divisor;
    std::cout << "add divisor for kernel values (float) : ";
    std::cin >> divisor;
    float div_flipped = 1.00 / divisor;
    kernel = kernel * div_flipped;
    std::cout << "kernel: \n" << kernel << "\n";
    processor = simple_cv_lib::ImageProcessor(kernel);
  }
  // cv::Mat gray = processor.toGrayscale(input);
  cv::Mat blurred = processor.gaussianBlur(input, 3);
  cv::Mat spectrum = processor.fourierTransform(input);

  if (argc > 2) {
    const std::string prefix = argv[2];
    // cv::imwrite(prefix + "_gray.png", gray);
    cv::imwrite(prefix + "_blur.png", blurred);
    cv::imwrite(prefix + "_fft.png", spectrum);
    if (make_kernel_user) {
      cv::Mat filtered = processor.convolutionFilter(input);
      cv::imwrite(prefix + "_filtered.png", filtered);
      cv::Mat filtered_manual = processor.convolutionFilterManual(input);
      cv::imwrite(prefix + "_filtered_manual.png", filtered_manual);
    }
    std::cout << "Wrote output images with prefix: " << prefix << std::endl;
  }

  return 0;
}
