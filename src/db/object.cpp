#include <chrono>
#include <string_view>

#include "db/object.hpp"
#include "binary_json/object.hpp"

using namespace std::chrono_literals;

db::Object::Object(binary_json::object_t obj) {
    this->object = std::move(obj);
    this->modified = std::chrono::system_clock::now();
}

bool db::Object::isExpired() {
    if (this->delete_after == 0s)
        return false;
    auto since = db::clock::now() - this->modified;
    auto sec_since = std::chrono::duration_cast<std::chrono::seconds>(since);
    return (sec_since >= delete_after);
}

void db::Object::setExpiration(std::chrono::seconds da) {
    this->delete_after = da;
}

binary_json::object_t &db::Object::resolve(std::string_view path) {
    throw std::logic_error("not implemented");
}
