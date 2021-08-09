#ifndef DBSON_SERVER_CONFIG_HPP
#define DBSON_SERVER_CONFIG_HPP

namespace service::server {

    struct ServerConfig {
        int port;
        size_t max_message_size;
    };
}

#endif //DBSON_SERVER_CONFIG_HPP
