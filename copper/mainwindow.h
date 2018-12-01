
#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtWidgets/QLabel>

#include <chrono>
#include <iostream>
#include "opencv2/opencv.hpp"

class MainWindow : public QObject {
  Q_OBJECT
 public:
  MainWindow();
  ~MainWindow(){};

 public slots:
  void update();

 private:
  QTimer timer_;
  QWidget window_;
  QLabel label_;
  cv::Mat img_;
  std::chrono::steady_clock::time_point tp_start;
};
