#include <iostream>
#include <thread>
#include <signal.h>
#include <string>
#include <vector>

#include "server.h"

#define INVALID_ARGUMENT -1
#define MISSING_ARGUMENT -2
#define SERVER_FAILURE -3

/**
 * @brief print how to use this program
 */
void print_help()
{
    std::cout << "This program must be used as follow:" << std::endl;
    std::cout << "\t" << "test-timeserver ${LISTENING_IP} ${LISTENING_PORT}" << std::endl << std::endl;
    std::cout << "Known errors:" << std::endl;
    std::cout << "\t" << "INVALID_ARGUMENT(" << INVALID_ARGUMENT << ")" << std::endl;
    std::cout << "\t" << "MISSING_ARGUMENT(" << MISSING_ARGUMENT << ")" << std::endl;
    std::cout << std::endl;
}

/**
 * @brief print the arguments of this program
 */
void print_args(const std::string& listening_ip, const std::string& listening_port)
{
    std::cout << "arguments: " << std::endl;
    std::cout << "\t" << "listening_ip: " << listening_ip << std::endl;
    std::cout << "\t" << "listening_port: " << listening_port << std::endl;
}

static std::atomic_bool on_sig_int_flag = false;
static void on_signal_handler(int sig)
{
    if (sig == SIGINT) {
        on_sig_int_flag = true;
    }
}

using Server = std::unique_ptr<Connectivity::Server>;
using Client = std::unique_ptr<Connectivity::Client>;

int main(int argc, char** argv)
{
    if (argc == 2  && std::strcmp(argv[1], "-h") == 0) {
        print_help();
        return 0;
    }

    if (argc < 3) {
        print_help();
        return MISSING_ARGUMENT;
    }

    const std::string listening_ip = argv[1];
    const std::string listening_port = argv[2];

    if (listening_ip.empty() || listening_port.empty()) {
        std::cerr << "arguments must not be empty" << std::endl;
        return INVALID_ARGUMENT;
    }

    print_args(listening_ip, listening_port);

    struct sigaction signal_handler;
    signal_handler.sa_handler = &on_signal_handler;
    sigemptyset(&signal_handler.sa_mask);
    signal_handler.sa_flags = 0;
    sigaction(SIGINT, &signal_handler, NULL);

    Server server = Connectivity::Server::create(listening_ip, listening_port);
    if (!server) {
        std::cerr << "failed to create server with the given arguments" << std::endl;
        return SERVER_FAILURE;
    }

    std::cout << "server created successfully" << std::endl;

    std::vector<Client> clients;

    auto on_send_failure = [&clients](const std::vector<Client>::iterator& it) {
        std::cerr << "client seems disconnected" << std::endl;
        return clients.erase(it);
    };

    auto send_to_all_clients = [&on_send_failure, &clients](const std::string& msg)
    {
        for (auto it = clients.begin(); it != clients.end(); ) {
            const auto& client = *it;
            if (!client->send(msg.c_str(), sizeof(char) * msg.size())) {
                it = on_send_failure(it);
            } else {
                it++;
            }
        }
    };

    bool should_keep_going = true;
    while (should_keep_going)
    {
        // make sure moved client is not gonna be used outside
        // of this scope
        {
            Client client = server->accept_new_client();
            if (client) {
                std::cout << "new connection detected" << std::endl;
                client->send("hey\n", sizeof(char) * 4);
                clients.push_back(std::move(client));
            }
        }

        const auto epoch = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        );

        const std::string epoch_str = std::to_string(epoch.count()) + "\n";
        send_to_all_clients(epoch_str);

        if (on_sig_int_flag) {
            std::cerr << "catch termination signal" << std::endl;
            send_to_all_clients("bye\n");
            should_keep_going = false;
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
