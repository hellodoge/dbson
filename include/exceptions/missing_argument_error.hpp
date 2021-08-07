#ifndef DBSON_MISSING_ARGUMENT_ERROR_HPP
#define DBSON_MISSING_ARGUMENT_ERROR_HPP

#include "exceptions/dbson_error.hpp"

struct missing_argument_error : dbson_error {
    explicit missing_argument_error(const char *msg="required argument not found") : dbson_error(msg) {}
};

#endif //DBSON_MISSING_ARGUMENT_ERROR_HPP
