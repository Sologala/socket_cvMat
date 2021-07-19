// Server side C/C++ program to demonstrate Socket programming
#include "../SocketAdapter.hpp"
#include "../msg.pb.h"
#include "../msg_wraper.hpp"
#include <iostream>
#include <netinet/in.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define PORT 8080

int main(int argc, char const *argv[]) {
  SocketAdapter<cv::Mat, proto_cv::msgMat, msgWraper, msgdeWraper> server(
      0, 19999, "");
  while (1) {
    cv::Mat img = server.Recv();

    if (!img.empty()) {
      cout << img.size() << endl;
      cv::imshow("img", img);
    } else {
      cout << "empty" << endl;
    }
    cv::waitKey(10);
  }

  return 0;
}
