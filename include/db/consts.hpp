//
// Created by waldemar on 8/8/21.
//

#ifndef DBSON_CONSTS_HPP
#define DBSON_CONSTS_HPP

namespace db::labels {
    const std::string_view command = "command";
    const std::string_view collection = "collection";
    const std::string_view object_name = "object";
    const std::string_view object_selector = "selector";
    const std::string_view success_status = "success";
    const std::string_view message = "message";
    const std::string_view data = "data";
    const std::string_view inner = "result_of";
}

namespace db::commands {
    const std::string_view ping = "ping";
    const std::string_view get = "get";
    const std::string_view set = "set";
}

#endif //DBSON_CONSTS_HPP
