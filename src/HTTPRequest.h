#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <string>
#include <unordered_map>

enum class HTTPMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    PATCH,
};

enum class HTTPVersion {
    HTTP_1_0,
    HTTP_1_1,
};

struct RequestLine {
    HTTPMethod method;
    HTTPVersion version;
    std::string target;
};

struct Request {
    RequestLine requestLine;
    std::unordered_map<std::string, std::string> headers;
    std::string body;
};

class HTTPRequest {
public:

    static HTTPMethod stringToMethod(const std::string& method);
    static HTTPVersion stringToVersion(const std::string& version);
    static std::string trim(const std::string& s);

    static RequestLine parseRequestLine(const std::string& requestLine);
    static std::unordered_map<std::string, std::string> parseRequestHeaders(const std::string& headersBlock);
    static Request parse(const std::string& rawRequest);
};

#endif //HTTPREQUEST_H
