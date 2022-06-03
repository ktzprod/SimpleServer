#include <thread>

#include "doctest.h"
#include "server.h"

TEST_SUITE_BEGIN("server");

TEST_CASE("Create server")
{
    SUBCASE("valid information")
    {
        auto server = Connectivity::Server::create("127.0.0.1", "8080");
        REQUIRE(server);
    }

    SUBCASE("invalid IP")
    {
        auto server = Connectivity::Server::create("32187.9128392183.12231.31", "8080");
        REQUIRE_FALSE(server);
    }

    SUBCASE("invalid PORT")
    {
        auto server = Connectivity::Server::create("127.0.0.1", "wrong");
        REQUIRE_FALSE(server);
    }
}

TEST_CASE("Accept clients")
{
    auto server = Connectivity::Server::create("127.0.0.1", "8080");
    auto succeeded = false;

    std::thread my_thread([&server, &succeeded] {
        bool new_client = false;
        auto start = std::chrono::high_resolution_clock::now();
        while (!new_client) {
            auto client = server->accept_new_client();
            if (client) {
                new_client = true;
                succeeded = true;
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
            if (duration.count() > 10) {
                break;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });

    std::system("telnet localhost 8080");
    my_thread.join();
}

TEST_SUITE_END();
