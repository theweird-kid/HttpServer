#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../Socket/Socket.hpp"
#include "../ThreadPool/ThreadPool.hpp"
#include "./Router/Router.hpp"

#include "./RequestHandler.hpp"
#include "./HttpResponse.hpp"

namespace http {

	using handler = std::function<void(HttpRequest&, HttpResponse&)>;

	class TcpServer {
	public:
		// Constructor
		TcpServer(Router router);

		// Destructor
		~TcpServer();

		// Run the server
		void start(uint16_t port);

		// Stop the server
		void stop();

	private:
		// Handle incoming requests
		void handleClient(Socket clientSocket);

		// Socket object
		Socket mSocket;

		// Thread pool
		ThreadPool mThreadPool;	

		// RequestHandler object
		RequestHandler mRequestHandler;
	};
}

#endif // !TCP_SERVER_H