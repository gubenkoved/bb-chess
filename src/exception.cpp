#include <QDebug>

#include "exception.h"

Exception::Exception(std::string message)
{
    m_message = message;

    // for see stack trace on Exceptions constructed
    Q_ASSERT(false);
}

std::string Exception::GetMessage()
{
    return m_message;
}
