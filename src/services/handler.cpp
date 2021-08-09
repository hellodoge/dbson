#include <mutex>

#include "services/handler.hpp"

using namespace service::handler;

void Handler::add_task(binary_json::object_t command, callback_function callback) {
    std::lock_guard lock { this->tasks_mutex };
    this->tasks.emplace_back(Task{std::move(command), std::move(callback)});
    this->new_task.notify_one();
}

void Handler::process(Task task) {
    binary_json::assoc response;
    try {
        binary_json::object_t command = std::move(task.command);
        // TODO middleware etc
        binary_json::object_t data = repository.execute(std::move(command));
        response.insert(std::make_pair(db::labels::success_status, true));
        response.insert(std::make_pair(db::labels::data, std::move(data)));
    } catch (dbson_error &e) {
        response.insert(std::make_pair(db::labels::success_status, false));
        response.insert(std::make_pair(db::labels::message, binary_json::string { e.what() }));
    } catch (std::exception &e) {
        response.insert(std::make_pair(db::labels::success_status, false));
        response.insert(std::make_pair(db::labels::message, binary_json::string(internal_error_message)));
        std::cerr << e.what() << std::endl;
    }
    task.callback(std::move(response));
}

std::future<void> Handler::run() {
    bool expected = false;
    if (!this->running.compare_exchange_strong(expected, true))
        throw std::runtime_error("attempt to run handler while it is already running");
    this->stop = false;
    return std::async(std::launch::async, [this](){
        while (true) {
            std::unique_lock lock { this->tasks_mutex };
            if (this->tasks.empty()) {
                this->new_task.wait_for(lock, std::chrono::seconds{5},[this]() {
                    return !this->tasks.empty() || this->stop;
                });
                if (this->tasks.empty()) {
                    if (!this->stop)
                        continue;
                    this->stop = false;
                    this->running = false;
                    return;
                }
            }
            Task task = this->tasks.front();
            this->tasks.pop_front();
            this->process(std::move(task));
        }
    });
}

void Handler::shutdown() {
    this->stop = true;
}
