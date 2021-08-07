#include <chrono>

#include "db/collection.hpp"
#include "db/object.hpp"

using namespace std::chrono_literals;

db::Object &db::Collection::getObject(std::string_view key) {
    auto [obj_iter, is_inserted] = this->values.emplace(std::make_pair(key, boost::none));
    db::Object &obj = obj_iter->second;
    if (!is_inserted && obj.isExpired()) {
        obj.set("", boost::none);
    }
    return obj;
}

db::Object &db::Collection::addObject(std::string key, db::Object val) {
    if (this->global_delete_after != 0s)
        val.setExpiration(this->global_delete_after);
    auto obj_iter = values.insert_or_assign(std::move(key), std::move(val));
    return obj_iter.first->second;
}

void db::Collection::setGlobalDeleteAfter(std::chrono::seconds da) {
    this->global_delete_after = da;
}
