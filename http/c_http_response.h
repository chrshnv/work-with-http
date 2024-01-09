#ifndef C_RESPONSE_H
#define C_RESPONSE_H
#include <iomanip>
#include <string>
#include <format>

class c_http_response {
public:
    enum e_http_status {
        // 2xx success
        OK,
        Created,
        Accepted,
        NON_AUTHORITATIVE_INFORMATION,
        NO_CONTENT,
        RESET_CONTENT,
        PARTIAL_CONTENT,
        MULTI_STATUS,
        ALREADY_REPORTED,
        IM_USED,

        // 4xx error
        NOT_FOUND
    };

    static std::string http_status_to_string(e_http_status status) {
        switch (status) {
            case OK: return "200 OK";
            case Created: return "201 Created";
            case Accepted: return "202 Accepted";
            case NON_AUTHORITATIVE_INFORMATION: return "203 Non-Authoritative Information";
            case NO_CONTENT: return "204 No Content";
            case RESET_CONTENT: return "205 Reset Content";
            case PARTIAL_CONTENT: return "206 Partial Content";
            case MULTI_STATUS: return "207 Multi-Status";
            case ALREADY_REPORTED: return "208 Already Reported";
            case IM_USED: return "226 IM Used";

            case NOT_FOUND: return "404 Not Found";
            default: throw std::runtime_error("unimplemented");
        }
    }

    explicit c_http_response( const e_http_status status_code = OK, const std::string& body = "", const std::map<std::string, std::string>& headers = std::map<std::string, std::string>() ) {
        this->status_code = status_code;
        this->body = body;
        this->headers = headers;
    }

    [[nodiscard]] std::string to_string() const {
        std::stringstream ss;
        ss << "HTTP/1.1 " << http_status_to_string(this->status_code) << "\r\n";
        ss << "Server: ParsleyTeaServer/1.0.0 (macOS)\r\n";
        ss << "Connection: keep-alive\r\n";
        ss << "Transfer-Encoding: chunked\r\n";

        // date
        const auto now = std::chrono::system_clock::now();
        const auto now_time = std::chrono::system_clock::to_time_t(now);
        ss << "Date: " << std::put_time( std::localtime( &now_time ), "%a, %d %b %Y %H:%M:%S %Z" ) << "\r\n";

        if (!headers.empty()) {
            for (const auto& [name, value]: headers) {
                ss << name << ": " << value << "\r\n";
            }
        }

        ss << "Content-Length: " << body.length() << "\r\n\r\n";
        ss << body;

        return ss.str();
    }
private:
    e_http_status status_code = OK;
    std::string body = "<h1>test</h1>";
    std::map<std::string, std::string> headers;
};

#endif //C_RESPONSE_H
