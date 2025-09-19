/**
 * @file DatabaseWorker.hpp
 * @brief Background worker thread that serializes persistence operations.
 */

#pragma once

#include "persistence/IDataStore.hpp"
#include "utils/ILogger.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <atomic>

namespace qga::persistence {

    class DatabaseWorker {
    public:
        using Task = std::function<void(IDataStore&)>;

        DatabaseWorker(std::unique_ptr<IDataStore> store,
                       std::shared_ptr<utils::ILogger> logger);
        ~DatabaseWorker();

        /// @brief Enqueue a database task (executed in background).
        void enqueue(Task task);

        /// @brief Gracefully stop worker thread.
        void stop();

    private:
        void run();

        std::unique_ptr<IDataStore> store_;
        std::shared_ptr<utils::ILogger> logger_;

        std::queue<Task> tasks_;
        std::mutex mutex_;
        std::condition_variable cv_;
        std::thread worker_;
        std::atomic<bool> running_{true};
    };

} // namespace qga::persistence
