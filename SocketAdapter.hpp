#ifndef SOCKETADAPTER_H
#define SOCKETADAPTER_H
#pragma once
#include "msg.pb.h"
#include <arpa/inet.h>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#define IMG_BUFFER_SIZE 5000 * 5000 * 4 * 3
#define MSG_HEAD_SIZE 8
// enum SocketAdapter_Mode { SERVER = 0, CLIENT = 1 };

template <class DATA, class MSG, class WRAPER, class DEWRAPER>
class SocketAdapter {
public:
  SocketAdapter(int _mode, int port = 9999, const char *server_id = NULL);
  ~SocketAdapter();

  bool is_connected() { return connected_; }

  void Send(DATA data);
  DATA Recv();

protected:
  int Create_connection_server();
  int Create_connection_client();

protected:
  int port_;
  int mode_;
  int sock;
  struct sockaddr_in serv_addr;
  char target_ip[100];
  // for server
  int opt = 1;
  bool connected_ = false;

private:
  unsigned char *out_buffer;
  unsigned char *in_buffer;
};

template <class DATA, class MSG, class WRAPER, class DEWRAPER>
void SocketAdapter<DATA, MSG, WRAPER, DEWRAPER>::Send(DATA data) {
  if (!connected_) {
    printf("\n not  connected \n");
    return;
  }
  proto_cv::msgHead msg_head;
  MSG msg = WRAPER()(data);
  msg_head.set_len(msg.ByteSize());

  memset(out_buffer, 0, IMG_BUFFER_SIZE);
  msg_head.SerializeToArray(out_buffer, msg_head.ByteSize());

  while (send(sock, out_buffer, MSG_HEAD_SIZE, 0) != MSG_HEAD_SIZE)
    ;
  // send data
  assert(msg.ByteSize() <= IMG_BUFFER_SIZE);
  // cout << msg_head.ByteSize() << " " << msg.ByteSize() << endl;
  msg.SerializeToArray(out_buffer, msg.ByteSize());
  while (send(sock, out_buffer, msg.ByteSize(), 0) != msg.ByteSize())
    ;
}

template <class DATA, class MSG, class WRAPER, class DEWRAPER>
DATA SocketAdapter<DATA, MSG, WRAPER, DEWRAPER>::Recv() {
  if (!connected_) {
    printf("\n not connected \n");
    return DATA();
  }
  MSG msg;
  msg.Clear();
  proto_cv::msgHead msg_head;
  // proto_cv::Mat_Head msg_head;
  memset(in_buffer, 0, IMG_BUFFER_SIZE);
  while (recv(sock, in_buffer, MSG_HEAD_SIZE, MSG_WAITALL) != MSG_HEAD_SIZE)
    ;
  msg_head.ParseFromArray(in_buffer, MSG_HEAD_SIZE);

  while (recv(sock, in_buffer, msg_head.len(), MSG_WAITALL) != msg_head.len())
    ;
  msg.ParseFromArray(in_buffer, msg_head.len());

  DATA data;
  data = DEWRAPER()(msg);
  return data;
}

template <class DATA, class MSG, class WRAPER, class DEWRAPER>

int SocketAdapter<DATA, MSG, WRAPER, DEWRAPER>::Create_connection_server() {
  int server_fd;
  int opt = 1;
  int addrlen = sizeof(serv_addr);

  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    return -1;
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    return -1;
  }
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port_);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind failed");
    return -1;
  }
  if (listen(server_fd, 3) < 0) {
    perror("listen");
    return -1;
  }

  // server 的 accept 方法会阻塞 函数

  printf("wait for client connection\n");
  if ((sock = accept(server_fd, (struct sockaddr *)&serv_addr,
                     (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    return -1;
  }
  printf("client connected\n");
  return 0;
}

template <class DATA, class MSG, class WRAPER, class DEWARPRE>
int SocketAdapter<DATA, MSG, WRAPER, DEWARPRE>::Create_connection_client() {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    return -1;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(port_);

  // Convert IPv4 and IPv6 addresses from text to binary form
  assert(strlen(target_ip) != 0);
  if (inet_pton(AF_INET, target_ip, &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    return -1;
  }

  int retry_cnt = 1000;
  while (retry_cnt) {
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("\nConnection Failed \n");
      retry_cnt--;
      usleep(200000);
    } else {
      break;
    }
  }
  if (retry_cnt == 0) {
    printf("\n retry 100 times without success! exit");
    return -1;
  }
  printf("\n create connection success\n");

  return 0;
}

template <class DATA, class MSG, class WRAPER, class DEWRAPER>
SocketAdapter<DATA, MSG, WRAPER, DEWRAPER>::SocketAdapter(int _mode, int port,
                                                          const char *server_id)
    : port_(port), mode_(_mode) {
  bool res = 0;
  if (_mode == 0) {
    res = Create_connection_server();
  } else {
    memset(target_ip, 0, sizeof(target_ip));
    memcpy(target_ip, server_id, strlen(server_id));
    res = Create_connection_client();
  }
  if (res == 0) {
    connected_ = true;
  } else {
    connected_ = false;
  }
  out_buffer = new unsigned char[IMG_BUFFER_SIZE];
  in_buffer = new unsigned char[IMG_BUFFER_SIZE];
}

template <class DATA, class MSG, class WRAPER, class DEWRAPER>
SocketAdapter<DATA, MSG, WRAPER, DEWRAPER>::~SocketAdapter() {
  delete[] out_buffer;
  delete[] in_buffer;
}

#endif