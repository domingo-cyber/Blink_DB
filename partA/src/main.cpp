#include "storage_engine.h"
#include <iostream>
#include <sstream>

int main() {
    StorageEngine db(1000);
    std::string command, key, value;
    
    while (true) {
        std::cout << "User> ";
        std::getline(std::cin, command);
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "SET") {
            iss >> key;
            iss.ignore();
            std::getline(iss, value);
            db.set(key, value);
        } else if (cmd == "GET") {
            iss >> key;
            std::cout << db.get(key) << std::endl;
        } else if (cmd == "DEL") {
            iss >> key;
            db.del(key);
        } else if (cmd == "EXIT") {
            break;
        } else {
            std::cout << "Invalid command.\n";
        }
    }
    
    return 0;
}