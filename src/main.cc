#include <iostream>
#include <functional>

#include "json.hpp"
#include "color.hpp"

#include "Core/socket/Socket.hpp"
#include "Core/router/Router.hpp"
#include "Core/server/TcpServer.hpp"

#define PORT 8080

using JSON = nlohmann::json;

int main()
{
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
    std::cout << MAGENTA << "Listening on port " << PORT << RESET << std::endl;
    server.start(PORT);

    return 0;
}
