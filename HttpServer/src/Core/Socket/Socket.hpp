#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <Winsock2.h>
#include <system_error>
#include <WS2tcpip.h>
#include <iostream>

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

	// Constructor
	Socket(const Type type, const AddressFamily addressFamily = AddressFamily::IPV4)
		: mSocket{ CreateSocket(type, static_cast<int>(addressFamily)) }
	{
		if (mSocket == INVALID_SOCKET)
		{
			throw std::system_error(WSAGetLastError(), std::system_category());
		}
	}


	// Destructor
	~Socket() noexcept
	{
		Close();
	}

	// Get the socket
	SOCKET Get() const noexcept
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

		return bind(mSocket, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) != SOCKET_ERROR;
	}

	// Listen for incoming connections
	bool Listen(int backlog = SOMAXCONN) const noexcept
	{
		return listen(mSocket, backlog) != SOCKET_ERROR;
	}

	// Accept an incoming connection
	Socket Accept() const noexcept
	{
		SOCKET clientSocket = accept(mSocket, nullptr, nullptr);
		return Socket{ std::move(clientSocket) };
	}

	// Close the socket
	void Close() noexcept 
	{
		if (mSocket != INVALID_SOCKET)
		{
			std::ignore = closesocket(mSocket);
			mSocket = INVALID_SOCKET;
		}
	}
	

	// Private constructor to accept SOCKET type
	Socket(SOCKET socket) noexcept : mSocket{ socket } {}

	// disable copy constructor
	Socket(const Socket&) = delete;

	// disable copy operator
	Socket& operator = (const Socket&) = delete;

	// Allow move constructor
	Socket(Socket&& socket) noexcept :
		mSocket{ std::exchange(socket.mSocket, INVALID_SOCKET) }
	{}

	// Allow move operator
	Socket& operator = (Socket&& rhs) noexcept
	{
		// close existing socekt
		if (mSocket != INVALID_SOCKET)
		{
			// ignores any runtime warnings
			std::ignore = closesocket(mSocket);
		}

		mSocket = std::exchange(rhs.mSocket, INVALID_SOCKET);

		return *this;
	}


protected:

	SOCKET mSocket{ INVALID_SOCKET };

private:

	inline SOCKET CreateSocket(const Socket::Type type, const int af) noexcept
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
		u_long mode = 1;
		ioctlsocket(mSocket, FIONBIO, &mode);

		return mSocket;
	}

};


#endif // !SOCKET_HPP
