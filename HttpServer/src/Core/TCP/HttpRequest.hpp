#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include <string>
#include <map>

class HttpRequest {
public:
	// Constructor
	HttpRequest(const std::string& method, const std::string& uri, const std::string& version, const std::map<std::string, std::string>& headers, const std::string& body)
		: mMethod{ method }, mUri{ uri }, mVersion{ version }, mHeaders{ headers }, mBody{ body }
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

	// Get the headers
	std::map<std::string, std::string> GetHeaders() const noexcept
	{
		return mHeaders;
	}

	// Get the body
	std::string GetBody() const noexcept
	{
		return mBody;
	}

	std::string mMethod;
	std::string mUri;
	std::string mVersion;
	std::map<std::string, std::string> mHeaders;
	std::string mBody;
};

#endif // !HTTP_REQUEST_HPP
