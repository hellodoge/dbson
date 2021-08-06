#ifndef DBSON_EOF_ERROR_HPP
#define DBSON_EOF_ERROR_HPP

struct eof_error : std::runtime_error {
    explicit eof_error(const char *unnamed="unexpected eof") : runtime_error(unnamed) {}
};

#endif //DBSON_EOF_ERROR_HPP
