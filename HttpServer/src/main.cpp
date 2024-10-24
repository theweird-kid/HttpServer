#include <iostream>
#include <functional> // Include this header for std::function

#include "Core/Socket/WinsockLibrary.hpp"
#include "Core/TCP/Router/Router.hpp"
#include "Core/TCP/TcpServer.h" 

int main()
{
	// Enable memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::cout << "Hello, World!" << std::endl;

	NET::WinsockLibrary winsock{ std::nothrow };

	Router router{};

	// Register the route with a lambda function
	router.addRoute("/hello", [](HttpRequest& req, HttpResponse& resp) {
		resp.SetStatus(HttpResponse::StatusCode::Ok);
		resp.SetHeader("Content-Type", "text/plain");
		resp.SetBody("Hello, World!");
	});

	router.addRoute("/bye", [](HttpRequest& req, HttpResponse& resp) {
		resp.SetStatus(HttpResponse::StatusCode::Ok);
		resp.SetHeader("Content-Type", "text/plain");
		resp.SetBody("Bye, World!");
	});

	// Create a TCP server
	http::TcpServer server{router};

	server.start(8080);

	return 0;
}