#include <regex>

#include "validator.h"

namespace Connectivity::Validator {

    bool is_valid_port(const std::string& port)
    {
        std::regex is_number_regex(R"(^^\s*[-+]?((\d+(\.\d+)?)|(\d+\.)|(\.\d+))(e[-+]?\d+)?\s*$)");
        return std::regex_match(port, is_number_regex);
    }

    bool is_valid_ipv4(const std::string& ip)
    {
        std::regex is_ip_regex("(([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9][0-9]|2[0-4][0-9]|25[0-5])");
        return std::regex_match(ip, is_ip_regex);
    }

}
