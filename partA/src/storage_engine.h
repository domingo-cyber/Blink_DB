/**
 * @file storage_engine.h
 * @brief Declaration of StorageEngine class for in-memory key-value storage.
 */
#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <unordered_map>
#include <string>
#include <mutex>

/**
 * @brief A simple key-value storage engine.
 */
class StorageEngine {
private:
    std::unordered_map<std::string, std::string> db;
    std::mutex db_mutex;  ///< Mutex for thread-safety (optional for single-threaded REPL).

public:
    /**
     * @brief Sets the value for a given key.
     * @param key The key.
     * @param value The value.
     */
    void set(const std::string &key, const std::string &value);

    /**
     * @brief Gets the value associated with a key.
     * @param key The key.
     * @return The value, or "NULL" if not found.
     */
    std::string get(const std::string &key);

    /**
     * @brief Deletes a key-value pair.
     * @param key The key.
     */
    void del(const std::string &key);
};

#endif
