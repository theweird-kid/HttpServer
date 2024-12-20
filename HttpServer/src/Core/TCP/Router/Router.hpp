#ifndef ROUTER_HPP
#define ROUTER_HPP

#include "Parser.hpp"
#include "../../Socket/Socket.hpp"
#include "../HttpRequest.hpp"
#include "../HttpResponse.hpp"

#include "color.hpp"

#include <iostream>
#include <chrono>
#include <map>
#include <functional>

class Router {
public:
    // Register Routes
    void addRoute(const std::string& path, std::function<void(HttpRequest&, HttpResponse&)> handler) {
        routes[path] = handler;
    }

	// Route the request
    void route(Socket& clientSocket) {
		
        // extract request from clientSocket
        constexpr int bufferSize = 1024;
        char buffer[bufferSize];
        int bytesReceived = recv(clientSocket.Get(), buffer, bufferSize - 1, 0);

        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';
			std::string request(buffer);

			HttpRequest req = RequestParser().parse(request);
            HttpResponse resp;

            // Add CORS headers
            resp.SetHeader("Access-Control-Allow-Origin", "*");
            resp.SetHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
            resp.SetHeader("Access-Control-Allow-Headers", "Content-Type");


            log(req);

            if (req.GetMethod() == "OPTIONS") {
                resp.SetStatus(HttpResponse::StatusCode::Ok);
                resp.SetHeader("Access-Control-Allow-Origin", "*");
                resp.SetHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
                resp.SetHeader("Access-Control-Allow-Headers", "Content-Type");
                resp.SetHeader("Access-Control-Max-Age", "86400");
                resp.SetHeader("Content-Length", "0");
                resp.SetBody("");
                return;
            }

            if (routes.find(req.GetUri()) != routes.end())
            {
				routes[req.GetUri()](req, resp);

				// Encode the response
                std::string response = resp.GetResponse();
                sendResponse(clientSocket, response, true); // Close the connection   
            }
            else
            {
                std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
                sendResponse(clientSocket, response, true);
            }

        }
        else
        {
			std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
			sendResponse(clientSocket, response, true);
            
        }
    }

private:
    std::map<std::string, std::function<void(HttpRequest&, HttpResponse&)>> routes;

    static void sendResponse(Socket& clientSocket, const std::string& response, bool closeConnection) {
        if (clientSocket.Get() != INVALID_SOCKET) {
            clientSocket.Send(response);
			if (closeConnection) {
				clientSocket.Close();
			}
        }
        else {
			std::cerr << "Failed to send response" << std::endl;
        }
    }

    void log(const HttpRequest& req) {
        // Get the current time
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        // Format the time
        std::tm timeInfo;
        localtime_s(&timeInfo, &now_time);
        std::stringstream timeStream;
        timeStream << std::put_time(&timeInfo, "%Y-%m-%d %H:%M:%S");

        // Log the time and request details
		auto headers = req.GetHeaders();
        std::cout << timeStream.str() << " ";
		if (req.GetMethod() == "GET") {
			std::cout << GREEN << req.GetMethod() << RESET << " ";
		}
		else if (req.GetMethod() == "POST") {
			std::cout << BLUE << req.GetMethod() << RESET << " ";
		}
		else if (req.GetMethod() == "PUT") {
			std::cout << YELLOW << req.GetMethod() << RESET << " ";
		}
		else if (req.GetMethod() == "DELETE") {
			std::cout << RED << req.GetMethod() << RESET << " ";
		}
		else {
			std::cout << req.GetMethod() << " ";
		}
		std::cout << req.GetUri() << std::endl;
    }
};


#endif // !ROUTER_HPP
