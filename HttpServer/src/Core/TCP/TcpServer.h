#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "../Socket/Socket.hpp"
#include "../ThreadPool/ThreadPool.hpp"

namespace http {
	class TcpServer {
	public:
		// Constructor
		TcpServer();

		// Destructor
		~TcpServer();

		// Run the server
		void start(uint16_t port);

		// Stop the server
		void stop();

	private:
		// Socket object
		Socket mSocket;

		// Thread pool
		ThreadPool mThreadPool;	
	};
}

#endif // !TCP_SERVER_H
