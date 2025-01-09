#ifndef REQUEST_HANDLER_HPP
#define REQUEST_HANDLER_HPP

#include "../router/Parser.hpp"
#include "../router/Router.hpp"

class RequestHandler  {
public:

    RequestHandler() { /* Default Constructor */ }

	RequestHandler(Router router) : mRouter{ router } {}

    // Handle HTTP request
    void handleRequest(Socket clientSocket) {

		mRouter.route(clientSocket);
    }

private:
	Router mRouter;
};

#endif // !REQUEST_HANDLER_HPP
