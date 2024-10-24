#include "TcpServer.h"

namespace http {

	static constexpr uint16_t threadPoolSize{ 4 };

	TcpServer::TcpServer(Router router) : mSocket{ Socket::Type::Tcp, Socket::AddressFamily::IPV4 },
		mRequestHandler{ router },
		mThreadPool{ threadPoolSize, mRequestHandler }
	{
		// Initialize Socket, Router and ThreadPool
	}

	TcpServer::~TcpServer() {
		// Stop the server
		stop();
	}

	void TcpServer::start(uint16_t port) {
		// Bind the socket to the port
		mSocket.Bind("127.0.0.1", port);

		// Listen for incoming connections
		std::printf("Listening on port %d..\n", port);
		mSocket.Listen();

		while (true) {
			// Accept incoming connections
			Socket clientSocket = mSocket.Accept();
			
            if (clientSocket.Get() != INVALID_SOCKET) {
                // Enqueue the task
				mThreadPool.Enqueue(std::move(clientSocket));
            }

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}

	void TcpServer::handleClient(Socket clientSocket) {
		// Handle Client
		//mRouter.routeRequest(std::move(clientSocket));

		constexpr int bufferSize = 1024;
		char buffer[bufferSize];
		int bytesReceived = recv(clientSocket.Get(), buffer, bufferSize - 1, 0);

		if (bytesReceived > 0)
		{
			buffer[bytesReceived] = '\0';
			std::string request(buffer);

			// Get the method and URI from the request
			std::cout << "Request Recieved" << std::endl;

			// Send a 404 response
			std::string response = "HTTP/1.1 404 Hello World\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
			send(clientSocket.Get(), response.c_str(), static_cast<int>(response.size()), 0);

			clientSocket.Close();
		}
		else
		{
			std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\nContent-Length: 9\r\n\r\nNot Found";
			send(clientSocket.Get(), response.c_str(), static_cast<int>(response.size()), 0);

			clientSocket.Close();
		}
	}

	void TcpServer::stop() {
		// Stop the server
		mSocket.Close();
	}
}