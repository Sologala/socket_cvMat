
#ifndef _MSG_WRAPER_
#define _MSG_WRAPER_
#include "../msg.pb.h"
#include <netinet/in.h>
#include <opencv2/core.hpp>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#pragma once

class msgdeWraper {
public:
  cv::Mat operator()(proto_cv::msgMat &msg) {
    cv::Mat img;
    img.create(msg.rows(), msg.cols(), msg.elt_type());
    unsigned long dataSize = img.rows * img.cols * img.elemSize();
    // std::cout << "ned " << dataSize << std::endl;
    assert(dataSize <= IMG_BUFFER_SIZE);
    std::copy(reinterpret_cast<unsigned char *>(
                  const_cast<char *>(msg.mat_data().data())),
              reinterpret_cast<unsigned char *>(
                  const_cast<char *>(msg.mat_data().data()) + dataSize),
              img.data);
    return img.clone();
  }
};

class msgWraper {
public:
  proto_cv::msgMat operator()(cv::Mat img) const {
    proto_cv::msgMat msg;
    msg.set_rows(img.rows);
    msg.set_cols(img.cols);
    msg.set_elt_type(img.type());
    msg.set_elt_size((int)img.elemSize());
    unsigned long dataSize = img.rows * img.cols * img.elemSize();
    msg.set_mat_data(img.data, dataSize);
    return msg;
  }
};

#endif