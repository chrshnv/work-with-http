#ifndef C_ROUTER_H
#define C_ROUTER_H
#include <functional>
#include <future>

#include "c_http_request.h"
#include "c_http_response.h"

class c_router {
public:
    std::map< std::pair< std::string, std::string >, std::function< c_http_response*( c_http_request* request ) > > routes;
    // std::vector< std::shared_ptr< std::function< c_http_request* >( c_http_request* request ) > > middlewares;

    c_router() = default;
    std::string listen(c_http_request* request) {
        std::promise<std::string> p;
        auto f = p.get_future();

        std::thread t(handle, this, request, std::move(p));
        t.detach();

        return f.get();
    }
private:
    static void handle(c_router* router, c_http_request* request, std::promise<std::string>&& p) {
        if ( const auto route = router->routes.find( std::make_pair( request->path, request->method ) ); route == router->routes.end() )
            p.set_value( c_http_response( c_http_response::NOT_FOUND ).to_string() );
        else
            p.set_value(route->second(request)->to_string());
    }
};

#endif //C_ROUTER_H
