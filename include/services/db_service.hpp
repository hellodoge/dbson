#ifndef DBSON_DB_SERVICE_HPP
#define DBSON_DB_SERVICE_HPP

#include <boost/container/flat_map.hpp>

#include "db/collection.hpp"
#include "services/db_abstract.hpp"

namespace service::db_service {

    using boost::container::flat_map;

    struct DBService final : AbstractDBService {

        binary_json::object_t execute(binary_json::object_t command) override;

    private:
        binary_json::object_t get(binary_json::assoc &params) override;
        binary_json::object_t set(binary_json::assoc &params) override;

    private:
        db::Collection &get_collection(binary_json::assoc &params);
        db::Object &get_object(binary_json::assoc &params);
        binary_json::object_t execute_inner(binary_json::assoc &params);

    private:
        flat_map<std::string, db::Collection, std::less<>> collections;
    };

}

#endif //DBSON_DB_SERVICE_HPP
