#ifndef DBSON_DBSON_ERROR_HPP
#define DBSON_DBSON_ERROR_HPP

struct dbson_error : std::runtime_error {
    explicit dbson_error(const char *msg="dbson internal error") : std::runtime_error(msg) {}
};

#endif //DBSON_DBSON_ERROR_HPP
