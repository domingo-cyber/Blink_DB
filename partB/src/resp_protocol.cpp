#include "resp_protocol.h"
#include <sstream>

std::string RESPProtocol::encodeSimpleString(const std::string& str) {
    return "+" + str + "\r\n";
}

std::string RESPProtocol::encodeBulkString(const std::string& str) {
    return "$" + std::to_string(str.size()) + "\r\n" + str + "\r\n";
}

std::string RESPProtocol::encodeError(const std::string& msg) {
    return "-" + msg + "\r\n";
}

std::string RESPProtocol::encodeInteger(int num) {
    return ":" + std::to_string(num) + "\r\n";
}

std::vector<std::string> RESPProtocol::decodeArray(const std::string& resp) {
    std::vector<std::string> result;
    std::istringstream stream(resp);
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.empty() || line[0] == '*') continue; // Ignore array length line
        result.push_back(line);
    }
    return result;
}
