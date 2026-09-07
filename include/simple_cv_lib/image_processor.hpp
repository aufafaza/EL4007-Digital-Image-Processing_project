#pragma once

#include <opencv2/core.hpp>
#include <iostream> 
namespace simple_cv_lib
{

class ImageProcessor
{
public:
  ImageProcessor() = default;
  ImageProcessor(const cv::Mat & kernel){ 
    this->kernel_ = kernel;
    std::cout << " --------------------------------- \n"; 
    std::cout << kernel_ << "\n"; 
  } 


  cv::Mat toGrayscale(const cv::Mat & input) const;
  cv::Mat gaussianBlur(const cv::Mat & input, int kernel_size = 5) const;
  cv::Mat convolutionFilter(const cv::Mat & input) const;
  cv::Mat fourierTransform(const cv::Mat & input, bool shift = true) const;

private: 
  cv::Mat kernel_; 
};

} // namespace simple_cv_lib
