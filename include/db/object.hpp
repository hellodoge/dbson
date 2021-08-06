#ifndef DBSON_DB_OBJECT_HPP
#define DBSON_DB_OBJECT_HPP

#include <chrono>

#include "binary_json/object.hpp"
#include "binary_json/serialize.hpp"

using namespace std::chrono_literals;

namespace db {

    using clock = std::chrono::system_clock;
    using time_point = std::chrono::time_point<clock>;
    using time_units = std::chrono::seconds;

    struct Object {
        explicit Object(binary_json::object_t obj);

        bool isExpired();
        void setExpiration(std::chrono::seconds da);

        void set(std::string_view path, binary_json::object_t);

        template <typename Writer>
        size_t getSerialized(Writer w, std::string_view path) noexcept(false) {
            auto &obj = this->resolve(path);
            size_t bytes_written = binary_json::serialize_object(w, obj);
            return bytes_written;
        }

    private:
        binary_json::object_t &resolve(std::string_view path);

    private:
        time_point modified;
        time_units delete_after = 0s;
        binary_json::object_t object;
    };
}

#endif //DBSON_DB_OBJECT_HPP
