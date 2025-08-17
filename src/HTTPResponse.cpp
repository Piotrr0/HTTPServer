#include "HTTPResponse.h"
#include <sstream>

std::string HTTPResponse::generateResponse() const {
    std::ostringstream stream;
    stream << "HTTP/1.1" << static_cast<int>(code) << " " << getResponseCodeText(code) << "\n\r";

    for (const auto& [key, value] : headers) {
        stream << key << ": " << value << "\r\n";
    }

    stream << "\r\n" << body;
    return stream.str();
}

std::string HTTPResponse::getResponseCodeText(HTTPResponseCode code) {
    if (code == HTTPResponseCode::Ok)           return "OK";
    if (code == HTTPResponseCode::Created)      return "Created";
    if (code == HTTPResponseCode::BadRequest)   return "BadRequest";
    if (code == HTTPResponseCode::NotFound)     return "NotFound";
    if (code == HTTPResponseCode::ImATeapot)    return "ImATeapot";
    throw std::runtime_error("HTTP response code not found");
}