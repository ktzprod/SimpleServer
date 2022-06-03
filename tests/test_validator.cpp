#include "doctest.h"
#include "validator.h"

TEST_SUITE_BEGIN("validator");

TEST_CASE("valid ipv4")
{
    SUBCASE("valid ip")
    {
        REQUIRE(Connectivity::Validator::is_valid_ipv4("127.0.0.1"));
    }

    SUBCASE("incomplete ip")
    {
        REQUIRE_FALSE(Connectivity::Validator::is_valid_ipv4("127.0.1"));
    }

    SUBCASE("invalid ip")
    {
        REQUIRE_FALSE(Connectivity::Validator::is_valid_ipv4("my_string"));
    }
}

TEST_CASE("valid port")
{
    SUBCASE("valid port")
    {
        REQUIRE(Connectivity::Validator::is_valid_port("8084"));
    }

    SUBCASE("invalid port (not a number)")
    {
        REQUIRE_FALSE(Connectivity::Validator::is_valid_port("my_string"));
    }
}

TEST_SUITE_END();
