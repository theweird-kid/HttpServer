#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>

class HttpRequest
{
public:
	// Constructor
	HttpRequest(const std::string& method, const std::string& uri, const std::string& version, const std::string& body)
		: mMethod{ method }, mUri{ uri }, mVersion{ version }, mBody{ body }
	{}

	// Get the method
	std::string GetMethod() const noexcept
	{
		return mMethod;
	}

	// Get the URI
	std::string GetUri() const noexcept
	{
		return mUri;
	}

	// Get the version
	std::string GetVersion() const noexcept
	{
		return mVersion;
	}

	// Get the body
	std::string GetBody() const noexcept
	{
		return mBody;
	}
private:
	// Request method
	std::string mMethod;

	// Request URI
	std::string mUri;

	// Request version
	std::string mVersion;

	// Request body
	std::string mBody;
};

#endif // !HTTP_REQUEST_HPP
