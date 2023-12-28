#ifndef C_TCP_SERVER_H
#define C_TCP_SERVER_H
#include "c_socket.h"
#include "c_tcp_socket.h"

class c_tcp_server : public c_socket {
public:
    std::function<void(c_tcp_socket*)> new_connection_event = [](c_tcp_socket*){};

    explicit c_tcp_server(): c_socket(-1)
    {
        constexpr int opt = 1;
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(int));
        setsockopt(this->sock,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(int));
    }

    void bind(long port = 8080) {
        this->address.sin_family = PF_INET;
        this->address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        this->address.sin_port = htons(port);

        if ( ::bind( this->sock, reinterpret_cast<const sockaddr*>( &this->address ), sizeof( this->address ) ) == -1 )
            throw std::runtime_error("error when binding address");
    }

    void listen() {
        if ( ::listen( this->sock, 64 ) == -1 )
            throw std::runtime_error("error when start listening");

        std::thread t(accept, this);
        t.detach();
    }

private:
    static void accept(const c_tcp_server* server) {
        sockaddr_in client_sock_info{};
        socklen_t client_sock_info_len = sizeof(client_sock_info);

        while (true) {
            const int client_sock = ::accept( server->sock, reinterpret_cast<sockaddr *>( &client_sock_info ), &client_sock_info_len );
            if (client_sock == -1)
                throw std::runtime_error("error when accepting client");

            auto* client = new c_tcp_socket(client_sock);

            server->new_connection_event(client);
            client->listen();
        }
    }
};

#endif //C_TCP_SERVER_H
