#ifndef WINSOCK_LIBRARY_HPP
#define WINSOCK_LIBRARY_HPP

#include <WinSock2.h>
#include <iostream>
#include <system_error>

namespace NET
{
	class WinsockLibrary final
	{
	public:

		inline WinsockLibrary()
		{
			const int startupResult = WSAStartup(WINSOCK_VERSION, &mWSAData);
			if (startupResult != 0)
			{
				throw std::system_error(startupResult, std::system_category());
			}

			mIsInitialized = true;
		}

		// If you want the Constructor to not throw exception
		inline explicit WinsockLibrary(std::nothrow_t) noexcept
		{
			const int startupResult = WSAStartup(WINSOCK_VERSION, &mWSAData);
			if (startupResult != 0)
			{
				return;
			}
			// never gets set if error
			mIsInitialized = true;
		}

		//delete copy Constructor
		WinsockLibrary(const WinsockLibrary&) = delete;

		//delete copy assignment operator
		WinsockLibrary& operator = (const WinsockLibrary&) = delete;

		//make it moveable to another thread, ensuring that the new thread can also do cleanup
		inline WinsockLibrary(WinsockLibrary&& lib) noexcept :
			mIsInitialized{ std::exchange(lib.mIsInitialized, false) },
			mWSAData{ lib.mWSAData }
		{}

		//move assignment operator
		inline WinsockLibrary& operator = (WinsockLibrary&& rhs) noexcept
		{
			mIsInitialized = std::exchange(rhs.mIsInitialized, false);
			mWSAData = rhs.mWSAData;

			return *this;
		}

		inline ~WinsockLibrary() noexcept
		{
			if (mIsInitialized)
			{
				std::ignore = WSACleanup();
			}
		}

		inline bool isInitialized() const noexcept
		{
			return mIsInitialized;
		}

		inline explicit operator bool() const noexcept
		{
			return mIsInitialized;
		}

		inline const WSADATA& GetWSAData() const noexcept
		{
			return mWSAData;
		}

	private:
		WSADATA mWSAData{};
		bool mIsInitialized{ false };		//If we want to move this object to another thread
	};

}

#endif
