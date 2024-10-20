#ifndef HTTP_RESPONSE_HPP
#define HTTP_RESPONSE_HPP

#include <string>

class HttpResponse
{
public:
	// Status codes
	enum class StatusCode
	{
		Ok = 200,
		BadRequest = 400,
		NotFound = 404,
		InternalServerError = 500
	};

	// Constructor
	HttpResponse(const StatusCode statusCode, const std::string& body)
		: mStatusCode{ statusCode }, mBody{ body }
	{}

	// Get the response
	std::string GetResponse() const noexcept
	{
		std::string response;
		response += "HTTP/1.1 " + std::to_string(static_cast<int>(mStatusCode)) + " ";
		switch (mStatusCode)
		{
		case StatusCode::Ok:
			response += "OK";
			break;
		case StatusCode::BadRequest:
			response += "Bad Request";
			break;
		case StatusCode::NotFound:
			response += "Not Found";
			break;
		case StatusCode::InternalServerError:
			response += "Internal Server Error";
			break;
		}
		response += "\r\n";
		response += "Content-Length: " + std::to_string(mBody.size()) + "\r\n";
		response += "Content-Type: text/html\r\n";
		response += "Connection: close\r\n";
		response += "\r\n";
		response += mBody;
		return response;
	}

private:
	// Status code
	StatusCode mStatusCode;

	// Response body
	std::string mBody;
};

#endif // !HTTP_RESPONSE_HPP
