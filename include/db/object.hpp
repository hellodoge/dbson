#ifndef DBSON_DB_OBJECT_HPP
#define DBSON_DB_OBJECT_HPP

#include <chrono>

#include "binary_json/object.hpp"

using namespace std::chrono_literals;

namespace db {

    using clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<clock>;

    struct Object {
        explicit Object(binary_json::object_t obj);

        bool isExpired();
        void setExpiration(std::chrono::seconds da);

    private:
        binary_json::object_t &resolve(std::string_view path);

    private:
        time_point modified;
        std::chrono::seconds delete_after = 0s;
        binary_json::object_t object;
    };
}

#endif //DBSON_DB_OBJECT_HPP
