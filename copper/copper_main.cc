#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using std::cout;
using std::endl;

using namespace cv;

void hello_opencv() {
  namedWindow("Output", 1);
  Mat output = Mat::zeros(480, 640, CV_8UC3);
  // write text on the matrix:
  putText(output, "Hello World :)", Point(15, 70), FONT_HERSHEY_PLAIN, 3,
          Scalar(0, 255, 0), 4);
  // display the image:
  imshow("Output", output);
  // wait for the user to press any key:
  waitKey(0);
}

int main(int argc, char **argv) {
  cout << "Copper." << endl;
  QApplication app(argc, argv);
  QWidget window;
  window.resize(640, 480);
  // Disable resizing.
  window.setFixedSize(window.size());
  window.setWindowTitle(QApplication::translate("toplevel", "Copper"));

  QLabel label(&window);
  // label.setFrameStyle(QFrame::Panel | QFrame::Sunken);
  label.setAlignment(Qt::AlignCenter);

  // label.setPixmap(QPixmap::fromImage(img).scaled(
  //     label.size(), Qt::KeepAspectRatio, Qt::FastTransformation));

  // opencv image
  Mat mat = Mat::zeros(480, 640, CV_8UC3);
  // write text on the matrix:
  putText(mat, "Hello World :)", Point(15, 70), FONT_HERSHEY_PLAIN, 3,
          Scalar(0, 255, 0), 4);

  label.setPixmap(QPixmap::fromImage(
      QImage(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888)));

  window.show();
  return app.exec();
}