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

        Object &addObject(std::string key, Object val);

        Object &getObject(std::string_view key);

        void setGlobalDeleteAfter(time_units da);

    private:
        time_units global_delete_after = 0s;
        std::map<std::string, Object, std::less<>> values{};
    };

}

#endif //DBSON_COLLECTION_HPP
