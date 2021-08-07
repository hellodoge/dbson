#ifndef DBSON_UNEXPECTED_TYPE_ERROR_HPP
#define DBSON_UNEXPECTED_TYPE_ERROR_HPP

#include "exceptions/dbson_error.hpp"

struct unexpected_type_error : dbson_error {
    explicit unexpected_type_error(const char *msg="unexpected type") : dbson_error(msg) {}
};

#endif //DBSON_UNEXPECTED_TYPE_ERROR_HPP
