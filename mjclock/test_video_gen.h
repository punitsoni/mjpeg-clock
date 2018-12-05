#include <vector>
#include "opencv2/opencv.hpp"

class TestVideoGenerator {
 public:
  TestVideoGenerator(int width = 640, int height = 480)
      : width_(width),
        height_(height),
        img_(cv::Mat::zeros(height, width, CV_8UC3)) {}

  std::vector<uint8_t> GetNextFrame();

  int next_frameid() { return next_frameid_; }

 private:
  int width_;
  int height_;
  std::vector<uint8_t> jpeg_buffer_;
  float fps_ = 30.0;
  int millis_ = 0;
  int next_frameid_ = 0;
  cv::Mat img_;
};