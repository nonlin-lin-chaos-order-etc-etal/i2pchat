#include <string>
#include "parseerror.h"

ParseError::ParseError(const std::string& what_arg):std::runtime_error(what_arg) {}
