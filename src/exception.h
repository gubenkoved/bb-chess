#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <string>

class Exception
{
    std::string m_message;
public:
    Exception(std::string message);

    std::string GetMessage();
};

#endif // EXCEPTION_H
