#include <iostream>
#include <functional> 

#include "json.hpp"

#include "Core/Socket/WinsockLibrary.hpp"
#include "Core/TCP/Router/Router.hpp"
#include "Core/TCP/TcpServer.h" 

using JSON = nlohmann::json;

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
        resp.SetHeader("Content-Type", "application/json");
        JSON w = {
        {"message", "Hello, World!"}
        };
        resp.SetBody(w.dump());
	});

	router.addRoute("/bye", [](HttpRequest& req, HttpResponse& resp) {
		resp.SetStatus(HttpResponse::StatusCode::Ok);
		resp.SetHeader("Content-Type", "application/json");
		JSON w = {
		{"message", "Bye, World!"}
		};
		resp.SetBody(w.dump());
	});

	// Create a TCP server
	http::TcpServer server{router};
	// Listen on port 8080
	server.start(8080);

	return 0;
}