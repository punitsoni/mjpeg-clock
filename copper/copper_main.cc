#include <QtWidgets/QApplication>
#include <iostream>
#include "absl/strings/str_format.h"
#include "mainwindow.h"

using std::cout;
using std::endl;

int gen_mjpeg_main() { return 0; }

int main(int argc, char **argv) {
  cout << "Copper." << endl;
  QApplication app(argc, argv);
  MainWindow win;
  return app.exec();
}