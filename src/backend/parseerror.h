#ifndef PARSEERROR_H
#define PARSEERROR_H

#include <stdexcept>

class ParseError : public std::runtime_error
{
public:
    ParseError(const std::string& what_arg);
};

#endif // PARSEERROR_H
