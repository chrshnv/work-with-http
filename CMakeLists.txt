cmake_minimum_required(VERSION 3.27)
project(http_server)

set(CMAKE_CXX_STANDARD 23)

add_executable(http_server main.cpp
        tcp/c_socket.h
        tcp/c_socket.h
        tcp/c_tcp_socket.h
        tcp/c_tcp_server.h
        http/c_http_request.h
        utils/string_utils.h
        http/c_router.h
        http/c_http_response.h
        http/c_http_response.h
        utils/json.h
        models/c_user.h
)
