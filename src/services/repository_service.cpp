#include <map>
#include <boost/optional.hpp>

#include "services/repository_service.hpp"
#include "services/repository_const.hpp"
#include "exceptions/value_not_found_error.hpp"
#include "exceptions/unexpected_type_error.hpp"
#include "exceptions/missing_argument_error.hpp"

using namespace service::repository;

template <typename T>
boost::optional<std::reference_wrapper<T>> get_argument_of_type(
        std::string_view label, binary_json::assoc &params);

binary_json::object_t RepositoryService::execute(binary_json::object_t command_obj) {
    binary_json::assoc *params_ptr = boost::get<binary_json::assoc>(&command_obj);
    if (params_ptr == nullptr)
        throw unexpected_type_error("command parameters must be an associative array");
    binary_json::assoc &params = *params_ptr;

    auto command_name_opt = get_argument_of_type<binary_json::string>(labels::command, params);
    if (command_name_opt == boost::none)
        throw missing_argument_error("missing command name");
    std::string_view command_name = (*command_name_opt).get();
    return this->call(command_name, params);
}

binary_json::object_t RepositoryService::execute_inner(binary_json::assoc &params) {
    auto inner_opt = get_argument_of_type<binary_json::assoc>(labels::inner, params);
    if (inner_opt == boost::none)
        throw value_not_found_error{};
    binary_json::assoc &inner_params = *inner_opt;

    auto command_name_opt = get_argument_of_type<binary_json::string>(labels::command, inner_params);
    if (command_name_opt == boost::none)
        throw missing_argument_error("missing command name");
    std::string_view command_name = (*command_name_opt).get();
    return this->call(command_name, inner_params);
}

db::Collection &RepositoryService::get_collection(binary_json::assoc &params) {
    auto collection_name_opt = get_argument_of_type<binary_json::string>(labels::collection, params);
    if (collection_name_opt == boost::none)
        throw missing_argument_error("missing collection name");
    const std::string &collection_name = (*collection_name_opt).get();
    return this->collections[collection_name];
}

db::Object &RepositoryService::get_object(binary_json::assoc &params) {
    db::Collection &collection = this->get_collection(params);
    auto object_name_opt = get_argument_of_type<binary_json::string>(labels::object_name, params);
    if (object_name_opt == boost::none)
        throw missing_argument_error("missing object name");
    std::string_view object_name = (*object_name_opt).get();
    return collection.getObject(object_name);
}

binary_json::object_t RepositoryService::get(binary_json::assoc &params) {
    db::Object &object = this->get_object(params);
    std::string_view path = "";
    auto path_opt = get_argument_of_type<binary_json::string>(labels::object_selector, params);
    if (path_opt != boost::none)
        path = (*path_opt).get();
    const binary_json::object_t &raw_object = object.get(path);
    return raw_object;
}

binary_json::object_t RepositoryService::set(binary_json::assoc &params) {
    db::Object &object = this->get_object(params);
    std::string_view path = "";
    auto path_opt = get_argument_of_type<binary_json::string>(labels::object_selector, params);
    if (path_opt != boost::none)
        path = (*path_opt).get();
    binary_json::object_t data;
    auto data_iter = params.find(labels::data);
    if (data_iter != params.end()) {
        data = std::move(data_iter->second);
    } else {
        data = this->execute_inner(params);
    }
    object.set(path, std::move(data));
    return boost::none;
}

template <typename T>
boost::optional<std::reference_wrapper<T>> get_argument_of_type(
        std::string_view label, binary_json::assoc &params) {
    auto parameter_iter = params.find(label);
    if (parameter_iter == params.end())
        return boost::none;
    binary_json::object_t &parameter_obj = parameter_iter->second;
    auto parameter_ptr = boost::get<T>(&parameter_obj);
    if (parameter_ptr == nullptr)
        return boost::none;
    return std::ref(*parameter_ptr);
}
