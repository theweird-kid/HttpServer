#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include "../socket/Socket.hpp"
#include "../threadPool/ThreadPool.hpp"
#include "../router/Router.hpp"

#include "../utils/RequestHandler.hpp"
#include "../utils/HttpResponse.hpp"

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
