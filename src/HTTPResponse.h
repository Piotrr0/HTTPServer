#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H
#include <string>
#include <unordered_map>

enum class HTTPResponseCode {
    Continue = 100,
    SwitchingProtocols = 101,
    EarlyHints = 103,
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NonAuthoritativeInformation = 203,
    NoContent = 204,
    ResetContent = 205,
    PartialContent = 206,
    MultipleChoices = 300,
    MovedPermanently = 301,
    Found = 302,
    NotModified = 304,
    BadRequest = 400,
    Unauthorized = 401,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    ImATeapot = 418,
    InternalServerError = 500,
    NotImplemented = 501,
    BadGateway = 502,
    ServiceUnavailable = 503,
    GatewayTimeout = 504,
    HttpVersionNotSupported = 505
};

class HTTPResponse {
    HTTPResponseCode code;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

public:
    explicit HTTPResponse(const HTTPResponseCode code) : code(code) {};
    std::string generateResponse() const;

    static std::string getResponseCodeText(HTTPResponseCode code);
};

#endif //HTTPRESPONSE_H
