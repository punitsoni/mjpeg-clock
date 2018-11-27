#include <QtCore/QTimer>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <iostream>
#include "absl/strings/str_format.h"
#include "mainwindow.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using std::cout;
using std::endl;

using namespace cv;

int hello_opencv_qt(int argc, char **argv) {
  QApplication app(argc, argv);

  MainWindow win;

  // QWidget window;
  // window.resize(640, 480);
  // // Disable resizing.
  // window.setFixedSize(window.size());
  // window.setWindowTitle(QApplication::translate("toplevel", "Copper"));

  // QLabel label(&window);
  // label.setAlignment(Qt::AlignCenter);

  // // opencv image
  // Mat mat = Mat::zeros(480, 640, CV_8UC3);

  // generate_frame(&mat, std::chrono::milliseconds(1234));

  // label.setPixmap(QPixmap::fromImage(
  //     QImage(mat.data, mat.cols, mat.rows, mat.step,
  //     QImage::Format_RGB888)));

  // QTimer timer(nullptr);

  // window.show();
  return app.exec();
}

int main(int argc, char **argv) {
  cout << "Copper." << endl;
  return hello_opencv_qt(argc, argv);
}