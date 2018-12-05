#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "absl/strings/str_format.h"
#include "opencv2/opencv.hpp"

using std::cout;
using std::endl;

namespace {

void GenerateFrame(cv::Mat *img, const std::string &text, int frameid,
                   int timestamp_ms) {
  // clear image
  *img = cv::Scalar::all(0);
  int baseline = 0;
  int thickness = 3;
  int fontscale = 6;
  auto fontface = cv::HersheyFonts::FONT_HERSHEY_PLAIN;

  // Draw text
  cv::Size textsize =
      cv::getTextSize(text, fontface, fontscale, thickness, &baseline);

  cv::Point text_origin((img->cols - textsize.width) / 2,
                        (img->rows + textsize.height) / 2);

  putText(*img, text, text_origin, fontface, fontscale, cv::Scalar::all(255),
          thickness, cv::LINE_AA);

  // Draw frame id
  fontscale = 2;
  thickness = 1;
  const std::string frameid_text = absl::StrFormat("Frame %6d", frameid);
  cv::Size frameid_text_size =
      cv::getTextSize(frameid_text, fontface, fontscale, thickness, nullptr);
  cv::Point frameid_text_origin(img->cols - frameid_text_size.width - 10,
                                frameid_text_size.height + 10);
  putText(*img, frameid_text, frameid_text_origin, fontface, fontscale,
          cv::Scalar::all(255), thickness, cv::LINE_AA);

  // Draw timestamp
  fontscale = 1;
  thickness = 1;
  const std::string timestamp_text = absl::StrFormat("%8d ms", timestamp_ms);
  cv::Size timestamp_text_size =
      cv::getTextSize(timestamp_text, fontface, fontscale, thickness, nullptr);
  cv::Point timestamp_text_origin(img->cols - timestamp_text_size.width - 10,
                                  img->rows - 10);
  putText(*img, timestamp_text, timestamp_text_origin, fontface, fontscale,
          cv::Scalar::all(255), thickness, cv::LINE_AA);
}

void write_raw_mjpeg_video() {
  cv::Mat img = cv::Mat::zeros(480, 640, CV_8UC3);
  std::ofstream ofile("video.mjpg", std::ios::binary);

  // use simulated time for constant fps
  float fps = 30.0;
  int millis = 0;
  int duration = 20;  // seconds
  int num = int(fps * duration);
  int i = 0;

  while (i < num) {
    int ms_per_min = 1000 * 60;
    int ms_per_sec = 1000;
    int time_ms = millis;

    int time_min = time_ms / ms_per_min;
    time_ms = time_ms % ms_per_min;
    int time_sec = time_ms / ms_per_sec;
    time_ms = time_ms % ms_per_sec;

    const std::string text =
        absl::StrFormat("%02d:%02d:%03d", time_min, time_sec, time_ms);

    GenerateFrame(&img, text, i, millis);

    // encode the frame to jpeg
    std::vector<uint8_t> buffer;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(80);
    cv::imencode(".jpg", img, buffer, params);

    ofile.write((const char *)&buffer[0], buffer.size());
    i++;
    millis += int(1000 / fps);
  }
}
};