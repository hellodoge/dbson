#ifndef DBSON_DB_ABSTRACT_HPP
#define DBSON_DB_ABSTRACT_HPP

#include <unordered_map>

#include "services/db_const.hpp"

namespace service::db_service {

    struct AbstractDBService {

        using function = std::function<binary_json::object_t(binary_json::assoc&)>;

        virtual ~AbstractDBService() = default;

        virtual binary_json::object_t execute(binary_json::object_t command) = 0;

    protected:
        virtual binary_json::object_t get(binary_json::assoc &params) = 0;
        virtual binary_json::object_t set(binary_json::assoc &params) = 0;

    protected:
        binary_json::object_t call(std::string_view name, binary_json::assoc &params) {
            auto function_iter = functions.find(name);
            if (function_iter == functions.end())
                throw std::invalid_argument("function not found");
            return (function_iter->second)(params);
        }

    private:
        std::unordered_map<std::string_view, function> functions = {
                std::make_pair(commands::get, [this](auto &params) { return this->get(params); }),
                std::make_pair(commands::set, [this](auto &params) { return this->set(params); })
        };
    };

}

#endif //DBSON_DB_ABSTRACT_HPP
