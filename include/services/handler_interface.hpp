#ifndef DBSON_HANDLER_INTERFACE_HPP
#define DBSON_HANDLER_INTERFACE_HPP

#include <vector>
#include <functional>

#include "services/repository_abstract.hpp"
#include "binary_json/object.hpp"

namespace service::handler {

    using namespace service;

    using bytes = std::vector<char>;
    using callback_function = std::function<void(binary_json::object_t response)>;

    struct IHandler {

        virtual ~IHandler() = default;

        virtual void add_task(binary_json::object_t command,
                              callback_function callback) = 0;

    };

}

#endif //DBSON_HANDLER_INTERFACE_HPP
