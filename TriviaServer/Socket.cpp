#include "stdafx.h"
#include "Socket.h"
#include "TriviaServerException.h"

NS_BEGIN

Socket::Socket()
{
	WSADATA wsaData = {};
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        throw TriviaServerException("Winsock initialization failed");
    }
}


Socket::~Socket()
{
    try
    {
        if (SOCKET_ERROR == WSACleanup())
        {
            int errorCode = WSAGetLastError();

            std::cerr << "Error closing Winsock. Error #" << errorCode << std::endl;
        }
    }
    catch (std::exception &ex)
    {
        std::cerr << "Error closing Winsock: " << ex.what() << std::endl;
    }
    catch (...) // exceptions must not escape dtors
    {        std::cerr << "Unknown error closing Winsock" << std::endl;

    }

}

NS_END