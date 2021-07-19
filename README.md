# socket_cvMat
Transport cv::Mat with socket 

This is a toy project to transport matrix with socket and protobuf's encoding ability.

## demo

```shell
mkdir build 
cd build 
cmake ..
make 
```
In the folder `build`, there are two executables named `client ` and `server `. you can open two terminal and run them respectively.

Client will produce a colored cv::Mat and send it to server.
Server is in charge to show it.

### dependences

1. protobuf
2. opencv 3+ 





