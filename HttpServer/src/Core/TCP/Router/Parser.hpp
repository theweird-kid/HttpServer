#ifndef PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "../HttpRequest.hpp"

class RequestParser
{
public:
    HttpRequest parse(const std::string& request) {
        std::istringstream stream(request);
        std::string line;
        std::getline(stream, line);

        // Parse the request line
        std::istringstream requestLine(line);
        requestLine >> mMethod >> mUri >> mVersion;

        // Parse the headers
        while (std::getline(stream, line) && line != "\r") {
            std::size_t colon = line.find(':');
            if (colon != std::string::npos) {
                std::string key = line.substr(0, colon);
                std::string value = line.substr(colon + 2); // Skip ': '
                mHeaders[key] = value;
            }
        }

        // Parse the body
        if (std::getline(stream, line)) {
            mBody = line;
            while (std::getline(stream, line)) {
                 mBody += "\n" + line;
            }

        }

        HttpRequest req{ mMethod, mUri, mVersion, mHeaders, mBody };
        return req;
    }

    void printRequest() const {
         std::cout << "Recieved Request:" << std::endl;
         std::cout << "Method: " << mMethod << std::endl;
         std::cout << "URI: " << mUri << std::endl;
         std::cout << "Version: " << mVersion << std::endl;
         std::cout << "Headers:" << std::endl;
         for (const auto& pair : mHeaders) {
            std::cout << pair.first << ": " << pair.second << std::endl;
         }
         std::cout << "Body: " << mBody << std::endl;
         std::cout << std::endl;
    }

private:
    std::string mMethod;
    std::string mUri;
    std::string mVersion;
    std::map<std::string, std::string> mHeaders;
    std::string mBody;
};

#endif // !PARSER_HPP
