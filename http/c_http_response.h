#ifndef C_RESPONSE_H
#define C_RESPONSE_H
#include <iomanip>
#include <sstream>
#include <string>

class c_http_response {
public:
    enum e_http_status {
        // 1xx information
        CONTINUE = 100,
        SWITCHING_PROTOCOLS,
        PROCESSING,
        EARLY_HINTS,

        // 2xx success
        OK = 200,
        CREATED,
        ACCEPTED,
        NON_AUTHORITATIVE_INFORMATION,
        NO_CONTENT,
        RESET_CONTENT,
        PARTIAL_CONTENT,
        MULTI_STATUS,
        ALREADY_REPORTED,
        IM_USED = 226,

        // 4xx error
        BAD_REQUEST = 400,
        UNAUTHORIZED,
        PAYMENT_REQUIRED,
        FORBIDDEN,
        NOT_FOUND,
        METHOD_NOT_ALLOWED,
        NOT_ACCEPTABLE,
        PROXY_AUTHENTICATION_REQUIRED,
        REQUEST_TIMEOUT,
        CONFLICT,
        GONE,
        LENGTH_REQUIRED,
        PRECONDITION_FAILED,
        PAYLOAD_TOO_LARGE,
        URI_TOO_LONG,
        UNSUPPORTED_MEDIA_TYPE,
        RANGE_NOT_SATISFIABLE,
        EXPECTATION_FAILED,
        I_M_TEAPOT,
        MISDIRECTED_REQUEST = 421,
        UNPROCESSABLE_CONTENT,
        LOCKED,
        FAILED_DEPENDENCY,
        TOO_EARLY,
        UPGRADE_REQUIRED,
        PRECONDITION_REQUIRED = 428,
        TOO_MANY_REQUESTS,
        REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
        UNAVAILABLE_FOR_LEGAL_REASONS = 451,

        // 5xx server errors
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED,
        BAD_GATEWAY,
        SERVICE_UNAVAILABLE,
        GATEWAY_TIMEOUT,
        HTTP_VERSION_NOT_SUPPORTED,
        VARIANT_ALSO_NEGOTIATES,
        INSUFFICIENT_STORAGE,
        LOOP_DETECTED,
        NOT_EXTENDED = 510,
        NETWORK_AUTHENTICATION_REQUIRED
    };

    static std::string http_status_to_string(const e_http_status status) {
        switch (status) {
            // 1xx
            case CONTINUE: return "100 Continue";
            case SWITCHING_PROTOCOLS: return "101 Switching Protocols";
            case PROCESSING: return "102 Processing";
            case EARLY_HINTS: return "103 Early Hints";

            // 2xx
            case OK: return "200 OK";
            case CREATED: return "201 Created";
            case ACCEPTED: return "202 Accepted";
            case NON_AUTHORITATIVE_INFORMATION: return "203 Non-Authoritative Information";
            case NO_CONTENT: return "204 No Content";
            case RESET_CONTENT: return "205 Reset Content";
            case PARTIAL_CONTENT: return "206 Partial Content";
            case MULTI_STATUS: return "207 Multi-Status";
            case ALREADY_REPORTED: return "208 Already Reported";
            case IM_USED: return "226 IM Used";

            //4xx
            case BAD_REQUEST: return "400 Bad Request";
            case UNAUTHORIZED: return "401 Unauthorized";
            case PAYMENT_REQUIRED: return "402 Payment Required";
            case FORBIDDEN: return "403 Forbidden";
            case NOT_FOUND: return "404 Not Found";
            case METHOD_NOT_ALLOWED: return "405 Method Not Allowed";
            case NOT_ACCEPTABLE: return "406 Not Acceptable";
            case PROXY_AUTHENTICATION_REQUIRED: return "407 Proxy Authentication Required";
            case REQUEST_TIMEOUT: return "408 Request Timeout";
            case CONFLICT: return "409 Conflict";
            case GONE: return "410 Gone";
            case LENGTH_REQUIRED: return "411 Length Required";
            case PRECONDITION_FAILED: return "412 Precondition Failed";
            case PAYLOAD_TOO_LARGE: return "413 Payload Too Large";
            case URI_TOO_LONG: return "414 URI Too Long";
            case UNSUPPORTED_MEDIA_TYPE: return "415 Unsupported Media Type";
            case RANGE_NOT_SATISFIABLE: return "416 Range Not Satisfiable";
            case EXPECTATION_FAILED: return "417 Expectation Failed";
            case I_M_TEAPOT: return "418 I'm a teapot";
            case MISDIRECTED_REQUEST: return "421 Misdirected Request";
            case UNPROCESSABLE_CONTENT: return "422 Unprocessable Content";
            case LOCKED: return "423 Locked";
            case FAILED_DEPENDENCY: return "424 Failed Dependency";
            case TOO_EARLY: return "425 Too Early";
            case UPGRADE_REQUIRED: return "426 Upgrade Required";
            case PRECONDITION_REQUIRED: return "428 Precondition Required";
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
        ss << "Connection: close\r\n";

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
    std::string body;
    std::map<std::string, std::string> headers;
};

#endif //C_RESPONSE_H
