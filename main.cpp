#include <functional>

#include "http/c_http_request.h"
#include "http/c_http_response.h"
#include "http/c_router.h"
#include "models/c_user.h"
#include "tcp/c_tcp_server.h"
#include "utils/json.h"

int main() {
    c_tcp_server server;
    c_router router;

    router.routes.insert( std::make_pair( std::make_pair( "/authorize/loader", "POST" ), [&]( const c_http_request* request ) -> c_http_response* {
        std::map<std::string, std::string> headers;
        headers.insert( std::make_pair( "Content-Type", "application/json" ) );

        const nlohmann::ordered_json j = ns::c_user("12341test", "tes");

        return new c_http_response(c_http_response::OK, j.dump(), headers);
    } ) );

    server.new_connection_event = [&](c_tcp_socket* client) {
        client->message_received_event = [client, &router](const std::string& request) {
            const auto http_req = new c_http_request(request);

            const auto response = router.listen(http_req);
            client->send(response);
        };
    };

    server.bind(8080);

    server.listen();

    while (true) {

    }

    server.close();

    return 0;
}
