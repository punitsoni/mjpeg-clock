#include "mainwindow.h"
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <iostream>
#include "absl/strings/str_format.h"

void generate_frame(cv::Mat* img, const std::string& text) {
  int baseline = 0;
  int thickness = 3;
  int fontscale = 6;
  auto fontface = cv::HersheyFonts::FONT_HERSHEY_PLAIN;

  cv::Size text_size =
      cv::getTextSize(text, fontface, fontscale, thickness, &baseline);

  // center the text
  cv::Point text_origin((img->cols - text_size.width) / 2,
                        (img->rows + text_size.height) / 2);

  // draw the box
  // rectangle(*img, text_origin + Point(0, baseline),
  //           text_origin + Point(text_size.width, -text_size.height),
  //           Scalar(0, 0, 255));

  // ... and the baseline first
  // line(*img, text_origin + Point(0, thickness),
  //      text_origin + Point(text_size.width, thickness), Scalar(0, 255, 0));

  // clear image
  *img = cv::Scalar::all(0);
  // then put the text itself
  putText(*img, text, text_origin, fontface, fontscale, cv::Scalar::all(255),
          thickness, cv::LINE_AA);
}

MainWindow::MainWindow() : window_(nullptr), label_(&window_), timer_(nullptr) {
  window_.resize(640, 480);
  // Disable resizing.
  window_.setFixedSize(window_.size());
  window_.setWindowTitle("Copper");

  label_.setAlignment(Qt::AlignCenter);

  img_ = cv::Mat::zeros(480, 640, CV_8UC3);

  tp_start = std::chrono::steady_clock::now();

  QObject::connect(&timer_, SIGNAL(timeout()), this, SLOT(update()));
  update();
  timer_.start(33);

  window_.show();
}

void MainWindow::update() {
  using std::chrono::duration_cast;
  using std::chrono::milliseconds;
  using std::chrono::minutes;
  using std::chrono::seconds;
  auto tp = std::chrono::steady_clock::now();
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(tp - tp_start);

  int time_min = duration_cast<minutes>(millis).count();
  int time_s = duration_cast<seconds>(millis % minutes(1)).count();
  int time_ms = duration_cast<milliseconds>(millis % seconds(1)).count();

  const std::string text =
      absl::StrFormat("%02d:%02d:%03d", time_min, time_s, time_ms);

  generate_frame(&img_, text);

  label_.setPixmap(QPixmap::fromImage(QImage(
      img_.data, img_.cols, img_.rows, img_.step, QImage::Format_RGB888)));
}
