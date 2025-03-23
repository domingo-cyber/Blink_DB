/**
 * @file main.cpp
 * @brief REPL for interacting with the StorageEngine.
 */
#include <iostream>
#include <sstream>
#include "storage_engine.h"

int main() {
    StorageEngine engine;
    std::string input;

    std::cout << "BLINK DB Storage Engine REPL. Type 'EXIT' to quit." << std::endl;
    while (true) {
        std::cout << "User> ";
        std::getline(std::cin, input);
        if (input == "EXIT")
            break;

        std::istringstream iss(input);
        std::string command, key, value;
        iss >> command >> key;
        if (command == "SET") {
            // Get the remaining value (may include spaces)
            std::getline(iss, value);
            // Trim possible quotes and whitespace.
            if (!value.empty() && value.front() == ' ')
                value.erase(0, 1);
            if (!value.empty() && value.front() == '"')
                value.erase(0, 1);
            if (!value.empty() && value.back() == '"')
                value.pop_back();
            engine.set(key, value);
        } else if (command == "GET") {
            std::cout << engine.get(key) << std::endl;
        } else if (command == "DEL") {
            if (engine.get(key) == "NULL")
                std::cout << "Does not exist." << std::endl;
            else {
                engine.del(key);
            }
        } else {
            std::cout << "Invalid command." << std::endl;
        }
    }
    return 0;
}
