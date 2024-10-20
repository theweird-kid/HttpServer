#include "TcpServer.h"

namespace http {

	TcpServer::TcpServer() 
		: mSocket(Socket::Type::Tcp, Socket::AddressFamily::IPV4) ,
		   mThreadPool(4)
	{
		// Initialize Socket and ThreadPool
	}

	TcpServer::~TcpServer() {
	}

	void TcpServer::start(uint16_t port) {
		// Bind the socket to the port
		mSocket.Bind("127.0.0.1", port);

		// Listen for incoming connections
		std::printf("Listening on port %d\n", port);
		mSocket.Listen();

		while (true) {
			// Accept incoming connections
			Socket clientSocket = mSocket.Accept();
			
			if (clientSocket.Get() != INVALID_SOCKET) {
				// Enqueue the client socket
				mThreadPool.Enqueue(std::move(clientSocket));
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}

	void TcpServer::stop() {
		// Stop the server
		mSocket.Close();
	}
}