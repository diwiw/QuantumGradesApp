#include "persistence/DatabaseWorker.hpp"

namespace qga::persistence {

DatabaseWorker::DatabaseWorker(std::unique_ptr<IDataStore> store,
                               std::shared_ptr<utils::ILogger> logger)
    : store_(std::move(store)), logger_(std::move(logger)) {
    worker_ = std::thread(&DatabaseWorker::run, this);
    if (logger_) logger_->info("DatabaseWorker started");
}

DatabaseWorker::~DatabaseWorker() {
    stop();
    if (worker_.joinable()) {
        worker_.join();
    }
    if (logger_) logger_->info("DatabaseWorker stopped");
}

void DatabaseWorker::enqueue(Task task) {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        tasks_.push(std::move(task));
    }
    cv_.notify_one();
}

void DatabaseWorker::stop() {
    running_ = false;
    cv_.notify_all();
}

void DatabaseWorker::run() {
    while (running_) {
        Task task;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            cv_.wait(lock, [&] { return !tasks_.empty() || !running_; });
            if (!running_ && tasks_.empty()) break;
            task = std::move(tasks_.front());
            tasks_.pop();
        }
        try {
            task(*store_);
        } catch (const std::exception& ex) {
            if (logger_) logger_->error("Database task failed: " + std::string(ex.what()));
        }
    }
}

} // namespace qga::persistence
