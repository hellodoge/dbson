#ifndef DBSON_VALUE_NOT_FOUND_ERROR_HPP
#define DBSON_VALUE_NOT_FOUND_ERROR_HPP

#include "exceptions/dbson_error.hpp"

struct value_not_found_error : dbson_error {
    explicit value_not_found_error(const char *msg="value not found") : dbson_error(msg) {}
};

#endif //DBSON_VALUE_NOT_FOUND_ERROR_HPP
