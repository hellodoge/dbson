#include <future>
#include <boost/asio.hpp>
#include <iostream>
#include <atomic>

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
        if (e.failed())
            return;
        vec_bytes request;
        binary_json::object_t command;
        bool reading_failed = false;
        bool deserialized = false;
        const size_t max_buffer_size = this->config.max_message_size + 1;
        boost::asio::read(socket, boost::asio::dynamic_buffer(request, max_buffer_size),
                          [&](boost::system::error_code e, size_t transferred) -> size_t {
            if (e.failed() && e.value() != boost::asio::error::eof) {
                reading_failed = true;
                return 0;
            }
            try {
                command = binary_json::deserialize(request.begin(), request.end());
                deserialized = true;
                return 0;
            } catch (eof_error &_) {
                return this->config.max_message_size - transferred;
            } catch (deserializer_error &_) {
                reading_failed = true;
                return 0;
            }
        });
        // max buffer size bigger by 1 than max message size, so if it filled up we should not handle command
        if (reading_failed || !deserialized)
            return;
        auto socket_copy = new tcp_socket{std::move(socket)};
        this->handler.add_task(std::move(command), [&, socket_copy](binary_json::object_t response){
            auto _ = std::async(std::launch::async, [&, socket_copy](binary_json::object_t response){
                vec_bytes response_buffer;
                binary_json::serialize(std::back_inserter(response_buffer), response);
                boost::system::error_code ec;
                boost::asio::write(*socket_copy, boost::asio::buffer(response_buffer), ec);
                delete socket_copy;
            }, std::move(response));
        });
    });
}

std::future<void> Server::run() {
    bool expected = false;
    if (!this->running.compare_exchange_strong(expected, true))
        throw std::runtime_error("attempt to run server while it is already running");
    return std::async(std::launch::async, [this]() {
        serve();
        while (this->running) {
            this->context.run();
        }
    });
}

void Server::shutdown() {
    this->running = false;
}
