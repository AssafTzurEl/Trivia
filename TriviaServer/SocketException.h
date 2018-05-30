#pragma once
#include "TriviaServerException.h"
#include <sstream>

NS_BEGIN

/*
    Represents exceptions in network connection (socket).
*/
class SocketException : public TriviaServerException
{
public:
    /*
        Creates an exception with an error message.
    */
    SocketException(const char *errorMessage)
        : SocketException(errorMessage, WSAGetLastError())
    {}

    /*
        Creates an exception with an error message and an error code.
    */
    SocketException(const char *errorMessage, int errorCode)
        : TriviaServerException(CreateErrorMessage(errorMessage, errorCode)),
          m_errorCode(errorCode)
    {}

    /*
        Exception dtor.
    */
    virtual ~SocketException()
    {}

    static std::string CreateSocketErrorMessage(const char *errorMessage)
    {
        return CreateErrorMessage(errorMessage, WSAGetLastError());
    }


private:
    const int m_errorCode; // WSA error code

    static std::string CreateErrorMessage(const char *errorMessage, int errorCode)
    {
        std::stringstream stream;

        stream << errorMessage << ". Error code: " << errorCode;
        return stream.str();
    }
};

NS_END