#include <future>
#include <boost/asio.hpp>
#include <iostream>

#include "services/server.hpp"
#include "exceptions/deserializer_error.hpp"
#include "binary_json/deserialize.hpp"
#include "binary_json/serialize.hpp"

using namespace service::server;

using tcp_socket = boost::asio::ip::tcp::socket;

void Server::serve() {
    this->acceptor.async_accept([this](boost::system::error_code e, tcp_socket socket) {
        if (this->running)
            this->serve();
        if (e.failed()) {
            if (this->config.log_io_errors)
                std::cerr << e.message() << std::endl;
            return;
        }
        vec_bytes request;
        boost::asio::read(socket, boost::asio::dynamic_buffer(request), e);
        if (e.failed() && e.value() != boost::asio::error::eof) {
            if (this->config.log_io_errors)
                std::cerr << e.message() << std::endl;
            return;
        }
        binary_json::object_t command;
        try {
            command = binary_json::deserialize(request.begin(), request.end());
        } catch (deserializer_error &e) {
            if (this->config.log_io_errors)
                std::cerr << e.what() << std::endl;
            return;
        }
        this->handler.add_task(std::move(command), [&](binary_json::object_t response){
            auto _ = std::async(std::launch::async, [&](binary_json::object_t response){
                vec_bytes response_buffer;
                binary_json::serialize(std::back_inserter(response_buffer), response);
                boost::system::error_code ec;
                boost::asio::write(socket, boost::asio::buffer(response_buffer), ec);
                if (ec.failed() && this->config.log_io_errors)
                    std::cerr << e.message() << std::endl;
            }, std::move(response));
        });
    });
}

void Server::run() {
    bool expected = false;
    if (!this->running.compare_exchange_strong(expected, true))
        throw std::runtime_error("attempt to run server while it is already running");
    serve();
}

void Server::shutdown() {
    this->running = false;
}
