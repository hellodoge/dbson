#ifndef DBSON_EOF_ERROR_HPP
#define DBSON_EOF_ERROR_HPP

struct eof_error : std::runtime_error {
    explicit eof_error(const char *msg="unexpected eof") : runtime_error(msg) {}
};

#endif //DBSON_EOF_ERROR_HPP
