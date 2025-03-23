#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <unordered_map>
#include <string>
#include <mutex>

class StorageEngine {
private:
    std::unordered_map<std::string, std::string> db;
    std::mutex db_mutex;

public:
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void del(const std::string& key);
};

#endif
