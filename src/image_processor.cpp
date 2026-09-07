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

cv::Mat ImageProcessor::fourierTransform(const cv::Mat & input, bool shift) const
{
  if (input.empty()) {
    throw std::invalid_argument("ImageProcessor::fourierTransform: input image is empty");
  }

  cv::Mat gray = toGrayscale(input);

  int opt_rows = cv::getOptimalDFTSize(gray.rows);
  int opt_cols = cv::getOptimalDFTSize(gray.cols);
  cv::Mat padded;
  cv::copyMakeBorder(
    gray, padded, 0, opt_rows - gray.rows, 0, opt_cols - gray.cols,
    cv::BORDER_CONSTANT, cv::Scalar::all(0));

  cv::Mat planes[] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
  cv::Mat complex_img;
  cv::merge(planes, 2, complex_img);

  cv::dft(complex_img, complex_img);

  cv::split(complex_img, planes);
  cv::magnitude(planes[0], planes[1], planes[0]);
  cv::Mat magnitude = planes[0];

  magnitude += cv::Scalar::all(1);
  cv::log(magnitude, magnitude);

  magnitude = magnitude(cv::Rect(0, 0, magnitude.cols & -2, magnitude.rows & -2));

  if (shift) {
    int cx = magnitude.cols / 2;
    int cy = magnitude.rows / 2;

    cv::Mat q0(magnitude, cv::Rect(0, 0, cx, cy));
    cv::Mat q1(magnitude, cv::Rect(cx, 0, cx, cy));
    cv::Mat q2(magnitude, cv::Rect(0, cy, cx, cy));
    cv::Mat q3(magnitude, cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp;
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
  }

  cv::normalize(magnitude, magnitude, 0, 255, cv::NORM_MINMAX);
  magnitude.convertTo(magnitude, CV_8U);

  return magnitude;
}

}  // namespace simple_cv_lib
