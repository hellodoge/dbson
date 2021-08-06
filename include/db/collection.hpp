#ifndef DBSON_COLLECTION_HPP
#define DBSON_COLLECTION_HPP

#include <map>
#include <chrono>
#include <string>
#include <string_view>
#include <boost/optional.hpp>

#include "db/object.hpp"
#include "binary_json/object.hpp"

using namespace std::chrono_literals;

namespace db {

    struct Collection {

        void addObject(std::string key, Object val);

        auto getObject(std::string_view key) -> boost::optional<std::reference_wrapper<Object>>;

        void setGlobalDeleteAfter(std::chrono::seconds da);

    private:
        std::chrono::seconds global_delete_after = 0s;
        std::map<std::string, Object, std::less<>> values{};
    };

}

#endif //DBSON_COLLECTION_HPP