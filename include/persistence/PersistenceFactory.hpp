/**
 * @file PersistenceFactory.hpp
 * @brief Factory for creating IDataStore implementations based on configuration.
 */

#pragma once

#include "persistence/IDataStore.hpp"
#include <memory>
#include <string>

namespace qga::persistence {

    enum class StoreBackend {
        SQLite,
        // Future backends can be added here (e.g. PostgreSQL, MySQL, etc.)
    };

    class PersistenceFactory {
    public:
        /// @brief Creates an IDataStore instance based on the specified backend and configuration.
        /// @param backend The storage backend to use (e.g. SQLite).
        /// @param config Configuration string (e.g. connection string or file path).
        static std::unique_ptr<IDataStore> create(StoreBackend backend, const std::string& config);
    };

} // namespace qga::persistence
