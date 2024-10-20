#include <iostream>

#include "Core/WinsockLibrary.hpp"
#include "Core/TcpServer.h" 

int main() {
	std::cout << "Hello, World!" << std::endl;

	NET::WinsockLibrary winsock{ std::nothrow };

	http::TcpServer server;
	server.start(8080);

	return 0;
}