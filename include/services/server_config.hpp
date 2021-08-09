#ifndef DBSON_SERVER_CONFIG_HPP
#define DBSON_SERVER_CONFIG_HPP

namespace service::server {

    struct ServerConfig {
        int port;
        bool log_io_errors;
    };
}

#endif //DBSON_SERVER_CONFIG_HPP
