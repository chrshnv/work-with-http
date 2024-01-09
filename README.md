# WORK WITH HTTP

"Work with http" is a pet-project written on C++. It's simple http server written using unix sockets and stl features.

* **JSON** support using [nlohmann/json](https://github.com/nlohmann/json)
* **Connection: close**
* **Middlewares**

## Usage

* Creating instance
```c++
c_tcp_server server;
c_router router;
```

* Adding middlewares(override http response, if null, run router function)
```c++
router.middlewares.emplace_back( []( c_http_request* request ) -> c_http_response* {
    return new c_http_response(c_http_response::Created);
} );
```

* Inserting route
```c++
router.routes.insert( std::make_pair( std::make_pair( "/authorize/loader", "POST" ), [&]( const c_http_request* request ) -> c_http_response* {
    std::map<std::string, std::string> headers;
    headers.insert( std::make_pair( "Content-Type", "application/json" ) );

    return new c_http_response(c_http_response::OK, request->body, headers);
} ) );
```

* Parsing request
```c++
server.new_connection_event = [&](c_tcp_socket* client) {
    client->message_received_event = [client, &router](const std::string& request) {
        const auto http_req = new c_http_request(request);

        const auto response = router.listen(http_req);
        client->send(response);
    };
};
```

* Running server
```c++
server.bind(8080);

server.listen();

while (true) {

}

server.close();
```

## Model mapping
Due to the fact that only JSON is currently supported, serialization is performed using [nlohmann/json "Arbitrary Type Conversions"](https://json.nlohmann.me/features/arbitrary_types/)

## Used libraries
* **JSON** support using [nlohmann/json](https://github.com/nlohmann/json)