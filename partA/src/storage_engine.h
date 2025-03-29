#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <unordered_map>
#include <list>
#include <string>

class StorageEngine {
private:
    std::unordered_map<std::string, std::string> kv_store;
    std::list<std::string> lru_list;
    std::unordered_map<std::string, std::list<std::string>::iterator> lru_map;
    size_t capacity;

    void evictIfNeeded();

public:
    StorageEngine(size_t cap = 1000);
    void set(const std::string& key, const std::string& value);
    std::string get(const std::string& key);
    void del(const std::string& key);
};

#endif