#pragma once

// WinSock2.h must be #included before Windows.h
// ws2_32.lib must be added in linker import settings
#include <WinSock2.h>
#include <Windows.h>
#include <thread>

NS_BEGIN

class ClientListener
{
public:
    ClientListener();
    ~ClientListener();

    void StartListening(SOCKET serverSocket);
    void StopListening();

private:
    SOCKET m_serverSocket;
    bool m_isListening;
    std::thread m_listenerThread;

    /*
     * Listens to client connections in a dedicated thread.
     */
    void ListenToClients(SOCKET serverSocket);
    void HandleClient(SOCKET clientSocket);
};

NS_END