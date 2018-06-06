#include "StdAfx.h"
#include "Communicator.h"
#include "TriviaServerException.h"

void main()
{
    try
    {
        Trivia::Communicator server(8080);

        server.StartListening();
        Sleep(INFINITE);
    }
    catch (const Trivia::TriviaServerException &ex)
    {
        LOG_ERROR("Network error: " << ex.what());
    }
    
}