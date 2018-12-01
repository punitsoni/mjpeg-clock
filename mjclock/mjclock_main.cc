#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "absl/strings/str_format.h"
#include "gflags/gflags.h"
#include "opencv2/opencv.hpp"

#include "mongoose.h"

using std::cout;
using std::endl;

std::chrono::steady_clock::time_point tp_start;

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

std::vector<uint8_t> CreateTestJpeg() {
  cv::Mat img = cv::Mat::zeros(480, 640, CV_8UC3);

  const std::string text("Hello World.");
  int millis = 12345;
  GenerateFrame(&img, text, 0, millis);

  // encode the frame to jpeg
  std::vector<uint8_t> buffer;
  std::vector<int> params;
  params.push_back(cv::IMWRITE_JPEG_QUALITY);
  params.push_back(80);
  cv::imencode(".jpg", img, buffer, params);

  return buffer;
}

static const char *s_http_port = "8000";

void http_handle_root(struct mg_connection *conn, struct http_message *req) {
  const std::string res =
      "<h1>Hello World.</h1>"
      "<img src=\"test_img\">";

  mg_send_head(conn, 200, res.size(), "Content-Type: text/html");
  mg_printf(conn, "%.*s", (int)res.size(), res.c_str());
}

void http_handle_testimg(struct mg_connection *conn, struct http_message *req) {
  std::vector<uint8_t> jpeg = CreateTestJpeg();
  mg_send_head(conn, 200, jpeg.size(), "Content-Type: image/jpeg");
  mg_send(conn, &jpeg[0], jpeg.size());
}

static void ev_handler(struct mg_connection *conn, int ev, void *p) {
  if (ev == MG_EV_HTTP_REQUEST) {
    struct http_message *req = (struct http_message *)p;

    const std::string request_method(req->method.p, req->method.len);
    const std::string request_uri(req->uri.p, req->uri.len);
    const std::string request_query(req->query_string.p, req->query_string.len);
    const std::string proto(req->proto.p, req->proto.len);

    absl::PrintF("--------------------------------\n");
    absl::PrintF("[HTTP Request]\n");
    absl::PrintF("method: %s\n", request_method);
    absl::PrintF("uri: %s\n", request_uri);
    absl::PrintF("query: %s\n", request_query);
    absl::PrintF("--------------------------------\n");

    if (request_uri == "/") {
      return http_handle_root(conn, req);
    }
    if (request_uri == "/test_img") {
      return http_handle_testimg(conn, req);
    }
  }
}

void func() {
  struct mg_mgr mgr;
  struct mg_connection *c;

  mg_mgr_init(&mgr, NULL);
  c = mg_bind(&mgr, s_http_port, ev_handler);
  mg_set_protocol_http_websocket(c);

  cout << "Starting server." << endl;
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);
}

int main(int argc, char **argv) {
  cout << "MJPG Clock stream." << endl;
  func();
}