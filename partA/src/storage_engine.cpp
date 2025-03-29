#include "storage_engine.h"
#include <iostream>

StorageEngine::StorageEngine(size_t cap) : capacity(cap) {}

void StorageEngine::evictIfNeeded() {
    if (kv_store.size() >= capacity) {
        std::string oldest_key = lru_list.back();
        lru_list.pop_back();
        kv_store.erase(oldest_key);
        lru_map.erase(oldest_key);
    }
}

void StorageEngine::set(const std::string& key, const std::string& value) {
    if (kv_store.find(key) == kv_store.end()) {
        evictIfNeeded();
    } else {
        lru_list.erase(lru_map[key]);
    }
    
    kv_store[key] = value;
    lru_list.push_front(key);
    lru_map[key] = lru_list.begin();
}

std::string StorageEngine::get(const std::string& key) {
    if (kv_store.find(key) == kv_store.end()) {
        return "NULL";
    }
    lru_list.erase(lru_map[key]);
    lru_list.push_front(key);
    lru_map[key] = lru_list.begin();
    return kv_store[key];
}

void StorageEngine::del(const std::string& key) {
    if (kv_store.find(key) == kv_store.end()) {
        std::cout << "Does not exist.\n";
        return;
    }
    lru_list.erase(lru_map[key]);
    lru_map.erase(key);
    kv_store.erase(key);
}