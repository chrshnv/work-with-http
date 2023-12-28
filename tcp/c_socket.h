#ifndef C_SOCKET_H
#define C_SOCKET_H
#include <stdexcept>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

class c_socket {
public:
    void close() const {
        shutdown(this->sock, SHUT_RDWR);
        ::close(this->sock);
    }
protected:
    int sock = 0;
    sockaddr_in address{};

    explicit c_socket(const int sock = -1) {
        if (sock == -1) {
            this->sock = socket(AF_INET, SOCK_STREAM, 0);

            if (this->sock == -1)
                throw std::runtime_error("socket creation error");
        } else
            this->sock = sock;
    }
};

#endif //C_SOCKET_H
