#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"

using std::cout;
using std::endl;

using namespace cv;

int main(int argc, char** argv) {
  cout << "Copper" << endl;
  // create a gui window:
  namedWindow("Output", 1);

  // initialize a 120X350 matrix of black pixels:
  Mat output = Mat::zeros(120, 350, CV_8UC3);

  // write text on the matrix:
  putText(output, "Hello World :)", Point(15, 70), FONT_HERSHEY_PLAIN, 3,
          Scalar(0, 255, 0), 4);

  // display the image:
  imshow("Output", output);

  // wait for the user to press any key:
  waitKey(0);

  return 0;
}