#pragma once
#include <exception>

NS_BEGIN

/*
    Base exception class for the ser ver side of the Trivia project.
*/
class TriviaServerException : public std::exception
{
public:
	// Not using std::exception(char *) ctor because it is not standard - see
	// https://docs.microsoft.com/en-us/cpp/standard-library/exception-class

    /*
        Creates an exception with an error message
    */
    TriviaServerException(const char *message)
		: m_message(message)
	{
        assert((message != nullptr) && (strlen(message) != 0));
    }

    /*
        Creates an exception with an error message
    */
    TriviaServerException(const std::string &message)
		: m_message(message)
	{}

    /*
        Implements std::exception's what() method for retrieving the exception's
        error message.
    */
    const char *what() const override
    {
        return m_message.c_str();
    }

    /*
        Exception dtor.
    */
	virtual ~TriviaServerException()
	{}

private:
	const std::string m_message;
};

NS_END