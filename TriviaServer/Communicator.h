#pragma once

#include "ClientListener.h"

NS_BEGIN

// Forward declarations:
class Communicator;

class Communicator
{
public:
    Communicator(USHORT port);
    ~Communicator();

    void StartListening();
    void StopListening();

    std::string ReadString();
    std::string ReadString(size_t numberOfBytes);
    int ReadInt(size_t numberOfBytes);

private:
    void InitWinsock();
    void InitSocket();
    void BindSocket(USHORT port);

    ClientListener m_clientListener;
    SOCKET m_socket;

    // Disable copying:
    Communicator(const Communicator &) = delete;
    Communicator &operator=(const Communicator &) = delete;
};

NS_END