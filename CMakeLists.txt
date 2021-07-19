

project(socket_adapter)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fPIC  -std=c++11 -w  -O0")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -Wall -g    -w -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g   -std=c++11 -w  -O0")



find_package(Protobuf REQUIRED)
message("Protobuf "${Protobuf_VERSION} ${Protobuf_FOUND} ${Protobuf_INCLUDE_DIRS} ${PROTOBUF_LIBRARIES})
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

execute_process(COMMAND protoc --cpp_out . ./msg.proto WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(PROTO_HDRS ${CMAKE_CURRENT_LIST_DIR}/msg.pb.h)
set(PROTO_SRCS ${CMAKE_CURRENT_LIST_DIR}/msg.pb.cc)
message(${PROTO_HDRS})
message(${PROTO_SRCS})
#find opencv 

find_package(OpenCV REQUIRED)
message("opencv: " ${OpenCV_VERSION})
include_directories(${OpenCV_INCLUDE_DIRS})
message(${PROTOBUF_INCLUDE_DIR})

file(GLOB SRC_FILES "./*.c*")
message(${SRC_FILES})
add_library(
    ${PROJECT_NAME} SHARED
    ${PROTO_SRCS}
    ${SRC_FILES}
)
target_link_libraries(
    ${PROJECT_NAME}
    ${PROTOBUF_LIBRARIES}
    ${OpenCV_LIBS}
)

set(socket_adapter_INCLUDE_DIRS  
    ${CMAKE_CURRENT_SOURCE_DIR}/
    PARENT_SCOPE
)

set(socket_adapter_LIBS
    ${PROJECT_NAME}
    PARENT_SCOPE
)




# add_executable(client
#     test/client.cpp
# )
# target_link_libraries(
#     client
#     socket_adapter
# )

# add_executable(
#     server
#     test/server.cpp
# )

# target_link_libraries(
#     server
#     socket_adapter
# )
