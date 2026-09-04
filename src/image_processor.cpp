#include "simple_cv_lib/image_processor.hpp"

#include <opencv2/core.hpp>
#include <opencv2/core/base.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>

namespace simple_cv_lib
{

cv::Mat ImageProcessor::toGrayscale(const cv::Mat & input) const
{
  if (input.empty()) {
    throw std::invalid_argument("ImageProcessor::toGrayscale: input image is empty");
  }
  if (input.channels() == 1) {
    return input.clone();
  }
  cv::Mat gray;
  cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
  return gray;
}

cv::Mat ImageProcessor::gaussianBlur(const cv::Mat & input, int kernel_size) const
{
  if (input.empty()) {
    throw std::invalid_argument("ImageProcessor::gaussianBlur: input image is empty");
  }
  if (kernel_size % 2 == 0) {
    kernel_size += 1;
  }
  cv::Mat blurred;
  cv::GaussianBlur(input, blurred, cv::Size(kernel_size, kernel_size), 0);
  return blurred;
}


cv::Mat ImageProcessor::convolutionFilter(const cv::Mat & input) const
{ 
  if (input.empty()){ 
    throw std::invalid_argument("ImageProcessor::convolutionFIlter: input image is empty"); 
  }
  if (this->kernel_.empty()){ 
    throw std::runtime_error("kernel is empty"); 
  } 
  cv::Mat filtered; 
  
  /* kernel logic here */ 
  // rotate kernel 180 degrres 
  cv::rotate(kernel_, kernel_, cv::ROTATE_180); 
  
  cv::filter2D(input, filtered, -1, kernel_, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT); 

  return filtered; 

  
} 

}  // namespace simple_cv_lib
