#include <utility>
#include "../Socket/Socket.hpp"

class RequestHandler {
public:
    // disable copy constructor
    RequestHandler(const Socket&) = delete;

    // disable copy operator
    RequestHandler& operator = (const Socket&) = delete;

    RequestHandler(RequestHandler&& other) noexcept
        : mClientSocket(std::exchange(other.mClientSocket, INVALID_SOCKET)) {
    }
    RequestHandler& operator=(const RequestHandler&) = default;
    RequestHandler& operator=(RequestHandler&& other) noexcept {
        if (this != &other) {
            mClientSocket = std::exchange(other.mClientSocket, INVALID_SOCKET);
        }
        return *this;
    }
    virtual void handleRequest() = 0;
    virtual ~RequestHandler() = default;
private:
    Socket mClientSocket{ INVALID_SOCKET };
};
