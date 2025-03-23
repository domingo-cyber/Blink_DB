/**
 * @file storage_engine.cpp
 * @brief Implementation of the StorageEngine class.
 */
#include "storage_engine.h"

void StorageEngine::set(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(db_mutex);
    db[key] = value;
}

std::string StorageEngine::get(const std::string &key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    return db.count(key) ? db[key] : "NULL";
}

void StorageEngine::del(const std::string &key) {
    std::lock_guard<std::mutex> lock(db_mutex);
    db.erase(key);
}
