#include <map>
#include <boost/optional.hpp>

#include "db/consts.hpp"
#include "services/repository.hpp"
#include "exceptions/value_not_found_error.hpp"
#include "exceptions/unexpected_type_error.hpp"
#include "exceptions/missing_argument_error.hpp"

using namespace service::repository;

template <typename T>
boost::optional<std::reference_wrapper<T>> get_argument_of_type(
        std::string_view label, binary_json::assoc &params);

binary_json::object_t Repository::execute(binary_json::object_t command_obj) {
    binary_json::assoc *params_ptr = boost::get<binary_json::assoc>(&command_obj);
    if (params_ptr == nullptr)
        throw unexpected_type_error("command parameters must be an associative array");
    binary_json::assoc &params = *params_ptr;

    auto command_name_opt = get_argument_of_type<binary_json::string>(db::labels::command, params);
    if (command_name_opt == boost::none)
        throw missing_argument_error("missing command name");
    std::string_view command_name = (*command_name_opt).get();
    return this->call(command_name, params);
}

binary_json::object_t Repository::execute_inner(binary_json::assoc &params) {
    auto inner_opt = get_argument_of_type<binary_json::assoc>(db::labels::inner, params);
    if (inner_opt == boost::none) {
        auto inner_arr_opt = get_argument_of_type<binary_json::array>(db::labels::inner, params);
        if (inner_arr_opt == boost::none)
            return boost::none;
        binary_json::array results;
        for (auto &el : (*inner_arr_opt).get()) {
            results.push_back(this->execute(std::move(el)));
        }
        return std::move(results);
    }
    binary_json::assoc &inner_params = *inner_opt;

    auto command_name_opt = get_argument_of_type<binary_json::string>(db::labels::command, inner_params);
    if (command_name_opt == boost::none)
        throw missing_argument_error("missing command name");
    std::string_view command_name = (*command_name_opt).get();
    return this->call(command_name, inner_params);
}

db::Collection &Repository::get_collection(binary_json::assoc &params) {
    auto collection_name_iter = params.find(db::labels::collection);
    if (collection_name_iter == params.end())
        throw missing_argument_error("missing object name");
    binary_json::object_t collection_name_obj = std::move((*collection_name_iter).second);
    // assoc is an inner command here
    if (boost::get<binary_json::assoc>(&collection_name_obj) != nullptr)
        collection_name_obj = this->execute(std::move(collection_name_obj));
    auto collection_name_ptr = boost::get<binary_json::string>(&collection_name_obj);
    if (collection_name_ptr == nullptr)
        throw unexpected_type_error("object name must be of string type");
    binary_json::string &collection_name = *collection_name_ptr;
    return this->collections[collection_name];
}

db::Object &Repository::get_object(binary_json::assoc &params) {
    auto object_name_iter = params.find(db::labels::object_name);
    if (object_name_iter == params.end())
        throw missing_argument_error("missing object name");
    binary_json::object_t object_name_obj = std::move((*object_name_iter).second);
    // assoc is an inner command here
    if (boost::get<binary_json::assoc>(&object_name_obj) != nullptr)
        object_name_obj = this->execute(std::move(object_name_obj));
    auto object_name_ptr = boost::get<binary_json::string>(&object_name_obj);
    if (object_name_ptr == nullptr)
        throw unexpected_type_error("object name must be of string type");
    std::string_view object_name = *object_name_ptr;
    db::Collection &collection = this->get_collection(params);
    return collection.getObject(object_name);
}

binary_json::object_t Repository::get(binary_json::assoc &params) {
    db::Object &object = this->get_object(params);
    std::string_view path = "";
    auto path_opt = get_argument_of_type<binary_json::string>(db::labels::object_selector, params);
    if (path_opt != boost::none)
        path = (*path_opt).get();
    const binary_json::object_t &raw_object = object.get(path);
    return raw_object;
}

binary_json::object_t Repository::set(binary_json::assoc &params) {
    db::Object &object = this->get_object(params);
    std::string_view path = "";
    auto path_opt = get_argument_of_type<binary_json::string>(db::labels::object_selector, params);
    if (path_opt != boost::none)
        path = (*path_opt).get();
    binary_json::object_t data;
    auto data_iter = params.find(db::labels::data);
    if (data_iter != params.end()) {
        data = std::move(data_iter->second);
    } else {
        data = this->execute_inner(params);
    }
    object.set(path, std::move(data));
    return boost::none;
}

binary_json::object_t Repository::ping(binary_json::assoc &params) {
    return binary_json::string { "pong" };
}

binary_json::object_t Repository::sum(binary_json::assoc &params) {
    binary_json::object_t sum;
    bool sum_initialized = false;

    auto sum_element = [&](binary_json::object_t el) {
        if (!sum_initialized) {
            sum = std::move(el);
            sum_initialized = true;
        } else {
            sum += std::move(el);
        }
    };

    auto sum_array = [&](binary_json::array &arr) {
        for (auto &el : arr) {
            sum_element(std::move(el));
        }
    };

    auto res = this->execute_inner(params);
    if (boost::get<binary_json::none>(&res) == nullptr) {
        auto arr_ptr = boost::get<binary_json::array>(&res);
        if (arr_ptr == nullptr)
            sum_element(std::move(res));
        else
            sum_array(*arr_ptr);
    }

    auto elems = get_argument_of_type<binary_json::array>(db::labels::elements, params);
    if (elems != boost::none)
        sum_array(*elems);

    return sum_initialized ? sum : boost::none;
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
