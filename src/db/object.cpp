#include <chrono>
#include <string_view>
#include <algorithm>
#include <boost/variant.hpp>

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
    auto sec_since = std::chrono::duration_cast<db::time_units>(since);
    return (sec_since >= delete_after);
}

void db::Object::setExpiration(std::chrono::seconds da) {
    this->delete_after = da;
}

binary_json::object_t &db::Object::resolve(std::string_view path) {
    const auto end = path.end();
    auto cursor = path.begin();
    binary_json::object_t *obj = &this->object;
    while (cursor != end) {
        auto assoc_ptr = boost::get<binary_json::assoc>(obj);
        if (assoc_ptr == nullptr) {
            auto null_ptr = boost::get<binary_json::none>(obj);
            if (null_ptr == nullptr)
                throw std::domain_error("resolve: not an assoc");
            *obj = binary_json::assoc{};
            assoc_ptr = boost::get<binary_json::assoc>(obj);
        }
        binary_json::assoc &assoc = *assoc_ptr;
        auto colon = std::find(cursor, end, ':');
        std::string_view key {&*cursor, static_cast<size_t>(colon-cursor)};
        auto [resolved_iter, _] = assoc.emplace(key, boost::none);
        obj = &resolved_iter->second;
        cursor = colon;
        if (cursor != end)
            cursor++;
    }
    return std::ref(*obj);
}

void db::Object::set(std::string_view path, binary_json::object_t value) {
    auto &obj = this->resolve(path);
    obj = std::move(value);
}
