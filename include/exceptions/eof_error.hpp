#ifndef DBSON_EOF_ERROR_HPP
#define DBSON_EOF_ERROR_HPP

#include "exceptions/deserializer_error.hpp"

struct eof_error : deserializer_error {
    explicit eof_error(const char *msg="deserializer: unexpected eof") : deserializer_error(msg) {}
};

#endif //DBSON_EOF_ERROR_HPP
