#ifndef DBSON_DESERIALIZER_ERROR_HPP
#define DBSON_DESERIALIZER_ERROR_HPP

#include "exceptions/dbson_error.hpp"

struct deserializer_error : dbson_error {
    explicit deserializer_error(const char *msg="deserializer error") : dbson_error(msg) {}
};

#endif //DBSON_DESERIALIZER_ERROR_HPP
