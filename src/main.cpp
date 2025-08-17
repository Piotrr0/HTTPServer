#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>

#include "HTTPRequest.h"

void getLinesFromBytes(const char* data, const size_t size, std::vector<std::string>& outLines) {
    std::string lineBuffer;
    for (size_t i = 0; i < size; i++) {
        const char c = data[i];
        if (c == '\n') {
            outLines.emplace_back(lineBuffer);
            lineBuffer.clear();
        } else {
            lineBuffer.push_back(c);
        }
    }

    if (!lineBuffer.empty())
        outLines.emplace_back(lineBuffer);
}

std::string readMessageFromFile(const std::string& path) {
    std::string message;
    std::ifstream file(path);

    std::string line;
    while (getline(file, line)) {
        message += line;
    }
    return message;
}

int main() {
    std::vector<std::string> lines;

    const int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
        return 1;

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

   if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) < 0)
       return 1;

    if (listen(serverSocket, 5) < 0)
        return 1;

    std::string message = readMessageFromFile("src/messages.txt");
    std::cout << message << std::endl;
    Request rq = HTTPRequest::parse(message);

    while (true) {
        const int clientSocket = accept(serverSocket, nullptr, nullptr);

        char buffer[1024];
        const ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0)
            continue;

        recv(clientSocket, buffer, sizeof(buffer), 0);
        getLinesFromBytes(buffer, bytesReceived, lines);
        for (const auto& line : lines) {
            std::cout << line << std::endl;
        }
    }

    return 1;
}