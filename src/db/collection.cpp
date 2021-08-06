#include <chrono>

#include "db/collection.hpp"
#include "db/object.hpp"

using namespace std::chrono_literals;

auto db::Collection::getObject(std::string_view key) -> boost::optional<std::reference_wrapper<Object>> {
    auto obj_iter = this->values.find(key);
    if (obj_iter == this->values.end())
        return boost::none;
    Object &obj = obj_iter->second;
    if (obj.isExpired()) {
        this->values.erase(obj_iter);
        return boost::none;
    }
    return std::ref(obj);
}

void db::Collection::addObject(std::string key, db::Object val) {
    if (this->global_delete_after != 0s)
        val.setExpiration(this->global_delete_after);
    values.insert_or_assign(std::move(key), std::move(val));
}

void db::Collection::setGlobalDeleteAfter(std::chrono::seconds da) {
    this->global_delete_after = da;
}
