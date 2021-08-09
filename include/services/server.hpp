#ifndef DBSON_SERVER_HPP
#define DBSON_SERVER_HPP

#include <future>
#include <vector>
#include <boost/asio.hpp>

#include "services/server_config.hpp"
#include "services/handler_interface.hpp"

namespace service::server {

    using namespace service;
    using service::handler::IHandler;

    using endpoint = boost::asio::ip::tcp::endpoint;

    using vec_bytes = std::vector<char>;

    struct Server {

        explicit Server(IHandler &h, ServerConfig config): handler{h}, acceptor{
            context, endpoint(boost::asio::ip::tcp::v4(), config.port)
        }, config{config} {}

        std::future<void> run();
        void shutdown();

    private:
        void serve();

    private:
        ServerConfig config;
        IHandler &handler;
        boost::asio::io_context context;
        boost::asio::ip::tcp::acceptor acceptor;
        std::atomic_bool running = false;
    };

}

#endif //DBSON_SERVER_HPP
