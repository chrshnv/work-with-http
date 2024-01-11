#ifndef C_HTTP_REQUEST_H
#define C_HTTP_REQUEST_H
#include <map>
#include <string>
#include <vector>

#include "../utils/string_views_utils.h"

class c_http_request {
public:
    explicit c_http_request(const std::string& request_body) {
        const std::string_view body_view { request_body.c_str(), request_body.length() };
        auto split_body = tstl::views::split(body_view, "\r");

        if (split_body.empty())
            throw std::runtime_error("invalid request");

        const auto http_section = tstl::views::split(split_body.at(0), " ");

        method = http_section.at(0);
        path = http_section.at(1);

        split_body.erase(split_body.begin());
        bool headers_end_passed = false;

        for (auto token: split_body ) {
            if ( token == "\n" )
                headers_end_passed = true;
            else if (!headers_end_passed) {
                token.remove_prefix(1);

                auto header_split = tstl::views::split(token, ": ");

                headers.insert( std::pair( header_split.at(0), header_split.at(1) ) );
            }
            else {
                if (token.starts_with("\n"))
                    token.remove_prefix(1);

                body += token;
            }
        }
    }

    std::string path;
    std::map<std::string, std::string> headers;
    std::string method;
    std::string body;
};

#endif //C_HTTP_REQUEST_H
