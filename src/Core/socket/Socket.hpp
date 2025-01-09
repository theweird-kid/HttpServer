#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>

#include <utility>
#include <system_error>
#include <iostream>

// Define INVALID_SOCKET and SOCKET_ERROR for Linux
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

class Socket
{
public:

    // Socket types
    enum class Type
    {
        Udp,
        Tcp
    };

    // Address families
    enum class AddressFamily
    {
        IPV4 = AF_INET,
        IPV6 = AF_INET6
    };

    // Default constructor
    Socket() {}

    // Constructor
    Socket(const Type type, const AddressFamily addressFamily = AddressFamily::IPV4)
        : mSocket{ CreateSocket(type, static_cast<int>(addressFamily)) }
    {
        if (mSocket == -1)
        {
            throw std::system_error(errno, std::system_category());
        }
    }

    // Destructor
    ~Socket() noexcept
    {
        Close();
    }

    // Get the socket
    int Get() const noexcept
    {
        return mSocket;
    }

    // Bind the socket to an address and port
    bool Bind(const std::string& address, uint16_t port) const noexcept
    {
        sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        inet_pton(AF_INET, address.c_str(), &addr.sin_addr);

        return bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != -1;
    }

    // Listen for incoming connections
    bool Listen(int backlog = SOMAXCONN) const noexcept
    {
        return listen(mSocket, backlog) != -1;
    }

    // Accept an incoming connection
    Socket Accept() const noexcept
    {
        int clientSocket = accept(mSocket, nullptr, nullptr);
        return Socket{ std::move(clientSocket) };
    }

    // Send data
    int Send(const std::string& data) const noexcept
    {
        return send(mSocket, data.c_str(), static_cast<int>(data.size()), 0);
    }

    // Close the socket
    void Close() noexcept
    {
        if (mSocket != -1)
        {
            std::ignore = close(mSocket);
            mSocket = -1;
        }
    }

    // Private constructor to accept int type
    Socket(int socket) noexcept : mSocket{ socket } {}

    // disable copy constructor
    Socket(const Socket&) = delete;

    // disable copy operator
    Socket& operator = (const Socket&) = delete;

    // Allow move constructor
    Socket(Socket&& socket) noexcept :
        mSocket{ std::exchange(socket.mSocket, -1) }
    {}

    // Allow move operator
    Socket& operator = (Socket&& rhs) noexcept
    {
        // close existing socket
        if (mSocket != -1)
        {
            std::ignore = close(mSocket);
        }

        mSocket = std::exchange(rhs.mSocket, -1);

        return *this;
    }

protected:

    int mSocket{ -1 };

private:

    inline int CreateSocket(const Socket::Type type, const int af) noexcept
    {
        switch (type)
        {
        case Type::Tcp:
            mSocket = socket(af, SOCK_STREAM, IPPROTO_TCP);
            break;
        case Type::Udp:
            mSocket = socket(af, SOCK_DGRAM, IPPROTO_UDP);
            break;
        default:
            //throw std::logic_error("Invalid type.");
            break;
        }

        // Set the socket to non-blocking mode
        int flags = fcntl(mSocket, F_GETFL, 0);
        fcntl(mSocket, F_SETFL, flags | O_NONBLOCK);

        return mSocket;
    }

};

#endif // !SOCKET_HPP
