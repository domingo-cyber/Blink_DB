#ifndef RESP_PROTOCOL_H
#define RESP_PROTOCOL_H

#include <string>
#include <vector>

class RESPProtocol {
public:
    static std::string encodeSimpleString(const std::string& str);
    static std::string encodeBulkString(const std::string& str);
    static std::string encodeError(const std::string& msg);
    static std::string encodeInteger(int num);
    static std::vector<std::string> decodeArray(const std::string& resp);
};

#endif
