/**
 * Copyright (c) 2021 braidpool developers (see AUTHORS)
 *
 * This file is part of braidpool.
 *
 * braidpool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * braidpool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with braidpool.  If not, see <https://www.gnu.org/licenses/>.
 */

#define pool_VERSION_MAJOR @pool_VERSION_MAJOR @
#define pool_VERSION_MINOR @pool_VERSION_MINOR @

#include "system.hpp"
#include <boost/log/trivial.hpp>
#include <iostream>
#include <p2p/define.hpp>
#include <p2p/node.hpp>

using namespace bp::p2p;

int main(int argc, char* argv[])
{
    LOG_INFO << "Starting braid pool...";
    try {
        if (argc != 5) {
            std::cout << "Usage: bp";
            std::cout << " <listen_address> <listen_port>";
            std::cout << " <peer_address> <peer_port>\n";
            return 1;
        }
    } catch (std::exception& e) {
        LOG_ERROR << e.what();
    }

    // TODO(kp): Improve arg parsing and remove passing char ptrs
    tcp_socket_node node_ { std::string(argv[1]), std::string(argv[2]) };
    node_.setup_initial_peers(std::string(argv[3]), std::string(argv[4]));

    node_.run();
}
