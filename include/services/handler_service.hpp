#ifndef DBSON_HANDLER_SERVICE_HPP
#define DBSON_HANDLER_SERVICE_HPP

#include <iostream>
#include <queue>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <future>

#include "services/handler_interface.hpp"
#include "exceptions/dbson_error.hpp"
#include "binary_json/object.hpp"
#include "binary_json/serialize.hpp"
#include "binary_json/deserialize.hpp"
#include "db/consts.hpp"

namespace service::handler {

    using namespace service;

    struct HandlerService final : IHandlerService {

        explicit HandlerService(repository::AbstractRepositoryService &repository): repository{repository} {}

        const std::string_view internal_error_message = "internal error";

        void add_task(binary_json::object_t command,
                      callback_function callback) override;

        std::future<void> run();

        void shutdown();

    private:

        struct Task {
            binary_json::object_t command;
            callback_function callback;
        };

        void process(Task task);

    private:
        repository::AbstractRepositoryService &repository;
        std::atomic_bool running = false;
        std::atomic_bool stop = false;
        std::deque<Task> tasks;
        std::mutex tasks_mutex;
        std::condition_variable new_task;
    };

}

#endif //DBSON_HANDLER_SERVICE_HPP
