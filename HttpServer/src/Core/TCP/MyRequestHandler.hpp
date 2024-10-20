#ifndef MY_REQUEST_HANDLER_HPP
#define MY_REQUEST_HANDLER_HPP

#include "./RequestHandler.hpp"
#include "./Parser.hpp"

class MyRequestHandler  {
public:
    MyRequestHandler() {
		// default constructor
    }
   
	MyRequestHandler(Socket clientSocket) : mClientSocket{ std::move(clientSocket)} {}

    // Handle HTTP request
    void handleRequest() {
        
        constexpr int bufferSize = 1024;
        char buffer[bufferSize];
        int bytesReceived = recv(mClientSocket.Get(), buffer, bufferSize - 1, 0);

        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';
            std::string request(buffer);
            //std::cout << "Received request:\n" << request.substr(0, bytesReceived) << std::endl;

            Parser::ParseRequest parser;
            parser.parse(request);
            parser.printRequest();

            // Simple HTTP response
            std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 13\r\n\r\nHello, World!";
            send(mClientSocket.Get(), response.c_str(), static_cast<int>(response.size()), 0);

			mClientSocket.Close();
        }
    }

    // Allow move constructor
    MyRequestHandler(MyRequestHandler&& rhs) noexcept :
        mClientSocket{ std::exchange(rhs.mClientSocket, NULL) }
    {}

    // Allow move operator
    MyRequestHandler& operator=(MyRequestHandler&& rhs) noexcept
    {
        if (this != &rhs)
        {
            mClientSocket = std::exchange(rhs.mClientSocket, NULL);
        }
        return *this;
    }

private:
    Socket mClientSocket{ INVALID_SOCKET };
};

#endif // !MY_REQUEST_HANDLER_HPP
