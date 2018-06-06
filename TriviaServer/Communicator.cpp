#include "stdafx.h"
#include "Communicator.h"
#include "SocketException.h"

NS_BEGIN

Communicator::Communicator(USHORT port)
    : m_socket(INVALID_SOCKET)
{
    InitWinsock();
    InitSocket();
    BindSocket(port);
}

Communicator::~Communicator()
{
    ::closesocket(m_socket);

    if (SOCKET_ERROR == ::WSACleanup())
    {
        // Can't throw in a dtor - writing to error log instead:
        LOG_ERROR(SocketException::CreateSocketErrorMessage("Error closing Winsock"));
    }
}

void Communicator::StartListening()
{
    // Listen to connections from clients:
    m_clientListener.StartListening(m_socket);
}

void Communicator::StopListening()
{
    m_clientListener.StopListening();
}

std::string Communicator::ReadString()
{
    auto numberOfBytes = ReadInt(2);

    return ReadString(numberOfBytes);
}

std::string Communicator::ReadString(size_t numberOfBytes)
{
    assert(numberOfBytes);

    if (0 == numberOfBytes)
    {
        return std::string();
    }

    auto buffer = std::make_unique<char[]>(numberOfBytes + 1);

    auto result = recv(m_socket, buffer.get(), numberOfBytes, 0);

    switch (result)
    {
        case 0:
            // Socket closed gracefully, nothing read
            return std::string();
            break;

        case INVALID_SOCKET:
            throw SocketException("Error receiving from socket");
            break;

        default:
            buffer.get()[result] = '\0';
            return buffer.get();
    }
}

int Communicator::ReadInt(size_t numberOfBytes)
{
    auto str = ReadString(numberOfBytes);
    auto result = 0;

    if (!str.empty())
    {
        std::stringstream stream;

        stream << str;
        stream >> result;
    }

    return result;
}

void Communicator::InitWinsock()
{
    WSADATA wsaData = {};
    auto errorCode = ::WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (errorCode)
    {
        throw SocketException("Winsock initialization failed", errorCode);
    }
}

void Communicator::InitSocket()
{
    m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_socket == INVALID_SOCKET)
    {
        throw SocketException("Error creating socket");
    }
}

void Communicator::BindSocket(USHORT port)
{
    ::sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = INADDR_ANY;

    if (SOCKET_ERROR == ::bind(m_socket, reinterpret_cast<::sockaddr *>(&address),
        sizeof(::sockaddr_in)))
    {
        throw SocketException("Error binding socket to IP address");
    }
}

NS_END