#pragma once

// WinSock2.h must be #included before Windows.h
// ws2_32.lib must be added in linker import settings
#include <WinSock2.h>
#include <Windows.h>

NS_BEGIN

class Socket
{
public:
    Socket();
    ~Socket();

private:
    SOCKET m_socket;
};

NS_END