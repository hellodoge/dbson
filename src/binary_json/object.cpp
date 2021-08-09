
#include "binary_json/object.hpp"
#include "exceptions/unexpected_type_error.hpp"

using namespace binary_json;

template <typename T>
T& get_type(binary_json::object_t &obj) {
    auto type_ptr = boost::get<T>(obj);
}

void operator+=(binary_json::object_t &sum, binary_json::object_t &&add_obj) {
    if (sum.which() != add_obj.which())
        throw unexpected_type_error("cannot sum different types");
    auto visitor = [&](auto &sum) {
        using T = std::decay_t<decltype(sum)>;
        if constexpr (std::is_same<T, integer>::value) {
            sum += boost::get<integer>(add_obj);
        } else if constexpr (std::is_same<T, real>::value) {
            sum += boost::get<real>(add_obj);
        } else if constexpr (std::is_same<T, string>::value) {
            sum += boost::get<string>(add_obj);
        } else if constexpr (std::is_same<T, array>::value) {
            auto &add = boost::get<array>(add_obj);
            for (auto &value : add)
                sum.push_back(std::move(value));
        } else if constexpr (std::is_same<T, assoc>::value) {
            auto &add = boost::get<assoc>(add_obj);
            for (auto &value : add)
                sum.emplace(std::move(value));
        } else if constexpr (std::is_same<T, bool>::value) {
            sum = sum || boost::get<bool>(add_obj);
        } else {
            throw unexpected_type_error("not supported type for sum operation");
        }
    };
    return boost::apply_visitor(visitor, sum);
}
