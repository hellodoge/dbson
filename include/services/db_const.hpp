#ifndef DBSON_DB_CONST_HPP
#define DBSON_DB_CONST_HPP

namespace service::db_service::labels {
    const std::string_view command = "command";
    const std::string_view collection = "collection";
    const std::string_view object_name = "object";
    const std::string_view object_selector = "selector";
    const std::string_view success_status = "success";
    const std::string_view message = "message";
    const std::string_view data = "data";
}

namespace service::db_service::commands {
    const std::string_view ping = "ping";
    const std::string_view get = "get";
    const std::string_view set = "set";
}

#endif //DBSON_DB_CONST_HPP
