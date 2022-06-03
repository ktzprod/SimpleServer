#pragma once

#include <string>

namespace Connectivity::Validator {

    /**
     * @brief validate a given port if its a number
     *
     * @param port
     *
     * @return true if the given port is a number, false otherwise
     */
    bool is_valid_port(const std::string& port);

    /**
     * @brief validate a given IP if it match IPv4 requirements
     *
     * @param ip
     *
     * @return true if the given IP is a valid IPv4 address, false otherwise
     */
    bool is_valid_ipv4(const std::string& ip);

}
