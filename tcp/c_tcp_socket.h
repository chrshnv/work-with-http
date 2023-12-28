#ifndef C_TCP_SOCKET_H
#define C_TCP_SOCKET_H
#include <functional>
#include <string>
#include <thread>

#include "c_socket.h"

#define BUFSIZE 512

class c_tcp_socket : public c_socket {
public:
    std::function<void(std::string)> message_received_event;

    explicit c_tcp_socket(const int sock = -1) : c_socket(sock) {}

    void listen() {
        std::thread t(receive, this);
        t.detach();
    }

    void send(const std::string& body) const {
        if ( ::send( sock, body.c_str(), body.length(), 0 ) == -1 )
            throw std::runtime_error("error when sending response over socket");
    }
private:
    static void receive(const c_tcp_socket* socket) {
        char tempBuffer[BUFSIZE+1];
        ssize_t messageLength;

        while ( ( messageLength = recv( socket->sock, tempBuffer, BUFSIZE, 0 ) ) > 0 )
        {
            tempBuffer[messageLength] = '\0';
            socket->message_received_event( std::string( tempBuffer, messageLength ) );
        }

        socket->close();
        delete socket;
    }
};

#endif //C_TCP_SOCKET_H
