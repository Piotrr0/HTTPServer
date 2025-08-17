#include "HTTPRequest.h"
#include <sstream>

HTTPMethod HTTPRequest::stringToMethod(const std::string &method) {
    if (method == "GET")     return HTTPMethod::GET;
    if (method == "HEAD")    return HTTPMethod::HEAD;
    if (method == "POST")    return HTTPMethod::POST;
    if (method == "PUT")     return HTTPMethod::PUT;
    if (method == "DELETE")  return HTTPMethod::DELETE;
    if (method == "CONNECT") return HTTPMethod::CONNECT;
    if (method == "OPTIONS") return HTTPMethod::OPTIONS;
    if (method == "TRACE")   return HTTPMethod::TRACE;
    if (method == "PATCH")   return HTTPMethod::PATCH;

    throw std::invalid_argument("Unsupported HTTP method: " + method);
}

HTTPVersion HTTPRequest::stringToVersion(const std::string &version) {
    if (version == "HTTP/1.0") return HTTPVersion::HTTP_1_0;
    if (version == "HTTP/1.1") return HTTPVersion::HTTP_1_1;

    throw std::invalid_argument("Unsupported HTTP version: " + version);
}

std::string HTTPRequest::trim(const std::string &s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    const auto end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";

    return s.substr(start, end - start + 1);
}

RequestLine HTTPRequest::parseRequestLine(const std::string &requestLine) {
    std::istringstream stream(requestLine);
    std::string method, target, version;

    if (!(stream >> method >> target >> version)) {
        throw std::invalid_argument("Malformed request line: " + requestLine);
    }

    RequestLine rl;
    rl.method = stringToMethod(trim(method));
    rl.target = trim(target);
    rl.version = stringToVersion(trim(version));

    return rl;
}

std::unordered_map<std::string, std::string> HTTPRequest::parseRequestHeaders(const std::string &headersBlock) {
    std::unordered_map<std::string, std::string> headers;
    std::istringstream stream(headersBlock);
    std::string line;

    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        if (line.empty()) break;

        const auto colonPos = line.find(':');
        if (colonPos == std::string::npos) {
            throw std::invalid_argument("Malformed header line: " + line);
        }

        const std::string key = trim(line.substr(0, colonPos));
        const std::string value = trim(line.substr(colonPos + 1));

        headers[key] = value;
    }

    return headers;
}

std::string normalizeLineEndings(const std::string &input) {
    std::string output = input;
    size_t pos = 0;
    while ((pos = output.find("\\r\\n", pos)) != std::string::npos) {
        output.replace(pos, 4, "\r\n");
        pos += 2;
    }
    return output;
}

Request HTTPRequest::parse(const std::string &rawRequest) {
    std::string normalized = normalizeLineEndings(rawRequest);
    std::istringstream stream(normalized);
    std::string line;

    if (!std::getline(stream, line))
        throw std::invalid_argument("Empty request");

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    Request req;
    req.requestLine = parseRequestLine(line);

    std::ostringstream headersBuf;
    while (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) break;
        headersBuf << line << "\n";
    }
    req.headers = parseRequestHeaders(headersBuf.str());

    std::ostringstream bodyBuf;
    bodyBuf << stream.rdbuf();
    req.body = bodyBuf.str();

    return req;
}