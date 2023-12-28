#ifndef C_HTTP_REQUEST_H
#define C_HTTP_REQUEST_H
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../utils/string_utils.h"

class c_http_request {
public:
    explicit c_http_request(const std::string& request_body) {
        std::istringstream s(request_body);
        std::string buf;

        while ( std::getline( s, buf ) && buf != "\r" ) {
            std::string::size_type index;
            index = buf.find(':', 0);

            if (index != std::string::npos) {
                this->headers.insert( std::make_pair( trim_copy( buf.substr(0, index) ), trim_copy( buf.substr( index + 1 ) ) ) );
            }
        }

        if ( std::string::size_type index = request_body.find( '\r', 0 ); index != std::string::npos ) {
            std::string line = request_body.substr(0, index);

            s = std::istringstream(line);
            std::vector<std::string> arr;

            for ( std::string word; std::getline( s, word, ' ' ); )
                arr.push_back(word);

            if (!arr.empty() && arr.size() == 3) {
                this->method = arr.at(0);
                this->path = arr.at(1);
            }
        }

        if ( !headers.contains( "Content-Length" ) )
            return;

        if ( std::string::size_type index = request_body.find( "\r\n\r\n", 0 ); index != std::string::npos ) {
            this->body = request_body.substr( index + 4, std::stoi( headers["Content-Length"] ) );
        }
    }

    std::string path;
    std::map<std::string, std::string> headers;
    std::string method;
    std::string body;
};

#endif //C_HTTP_REQUEST_H
