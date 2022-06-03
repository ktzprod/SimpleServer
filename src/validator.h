#pragma once

#include <string>

namespace Connectivity::Validator {

    bool is_valid_port(const std::string& port);
    bool is_valid_ipv4(const std::string& ip);

}
