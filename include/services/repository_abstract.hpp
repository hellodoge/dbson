#ifndef DBSON_REPOSITORY_ABSTRACT_HPP
#define DBSON_REPOSITORY_ABSTRACT_HPP

#include <functional>
#include <unordered_map>

#include "db/consts.hpp"
#include "binary_json/object.hpp"

namespace service::repository {

    struct AbstractRepository {

        using function = std::function<binary_json::object_t(binary_json::assoc&)>;

        virtual ~AbstractRepository() = default;

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
                std::make_pair(db::commands::get, [this](auto &params) { return this->get(params); }),
                std::make_pair(db::commands::set, [this](auto &params) { return this->set(params); })
        };
    };

}

#endif //DBSON_REPOSITORY_ABSTRACT_HPP