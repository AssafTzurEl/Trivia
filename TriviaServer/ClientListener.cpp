#include "stdafx.h"
#include "ClientListener.h"
#include "SocketException.h"
#include <WS2tcpip.h>

NS_BEGIN

ClientListener::ClientListener()
    : m_isListening(false)
{
}

ClientListener::~ClientListener()
{
}

void ClientListener::StartListening(SOCKET serverSocket)
{
    // Verify that we're not already in listening mode:
    if (m_isListening || m_listenerThread.joinable())
    {
        throw TriviaServerException(
            "Error starting listening to clients: Listener is already listening");
    }

    // else...

    m_isListening = true;

    if (SOCKET_ERROR == ::listen(serverSocket, SOMAXCONN))
    {
        throw SocketException("Error listening to socket");
    }

    m_listenerThread = std::thread(&ClientListener::ListenToClients, this, serverSocket);
}

void ClientListener::StopListening()
{
    assert(m_isListening);

    m_isListening = false;
}

void ClientListener::ListenToClients(SOCKET serverSocket)
{
    sockaddr_in clientAddress;
    int sockaddr_inSize = sizeof(sockaddr_in);
    const size_t BUF_SIZE = 20;
    char addrBuf[BUF_SIZE];

    while (m_isListening)
    {
        auto clientSocket = ::accept(serverSocket,
            reinterpret_cast<sockaddr *>(&clientAddress), &sockaddr_inSize);

        if (INVALID_SOCKET == clientSocket)
        {
            throw SocketException("Error accepting connection");
        }

        // else...

        LOG("Accepted connection from "
            << inet_ntop(AF_INET, &clientAddress.sin_addr, addrBuf, BUF_SIZE) << ":"
            << ntohs(clientAddress.sin_port));
        
        HandleClient(clientSocket);
    }}

void ClientListener::HandleClient(SOCKET clientSocket)
{
    const size_t kBufferSize = 1000;
    // Read data from client
    char acReadBuffer[kBufferSize];
    int nReadBytes;
    do {
        nReadBytes = recv(clientSocket, acReadBuffer, kBufferSize, 0);
        if (nReadBytes > 0) {
            std::cout << "Received " << nReadBytes <<
                " bytes from client." << std::endl;

            int nSentBytes = 0;
            while (nSentBytes < nReadBytes) {
                int nTemp = send(clientSocket, acReadBuffer + nSentBytes,
                    nReadBytes - nSentBytes, 0);
                if (nTemp > 0) {
                    std::cout << "Sent " << nTemp <<
                        " bytes back to client." << std::endl;
                    nSentBytes += nTemp;
                }
                else if (nTemp == SOCKET_ERROR) {
                    return;// false;
                }
                else {
                    // Client closed connection before we could reply to
                    // all the data it sent, so bomb out early.
                    std::cout << "Peer unexpectedly dropped connection!" <<
                        std::endl;
                    return;// true;
                }
            }
        }
        else if (nReadBytes == SOCKET_ERROR) {
            return;// false;
        }
    } while (nReadBytes != 0);

    std::cout << "Connection closed by peer." << std::endl;
    //return true;
}

NS_END