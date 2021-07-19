// Client side C/C++ program to demonstrate Socket programming

#include "../SocketAdapter.hpp"
#include "../msg.pb.h"
#include "../msg_wraper.hpp"
#include <arpa/inet.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
using namespace std;

#define PORT 8080

int main(int argc, char const *argv[]) {
  SocketAdapter<cv::Mat, proto_cv::msgMat, msgWraper, msgdeWraper> client(
      1, 19999, "127.0.0.1");
  // const string video_path = "/home/wen/Desktop/VID_20210404_175703.mp4";
  // cv::VideoCapture cap(video_path);
  cv::Mat mat;
  for (int i = 1; i != 0; ++i) {
    cv::Mat ned_send = cv::Mat::ones(500, 500, CV_8UC3) * (i % 256);
    client.Send(ned_send);
  }

  return 0;
}
