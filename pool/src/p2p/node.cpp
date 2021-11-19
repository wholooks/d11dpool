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

#include "p2p/node.hpp"
#include "system.hpp"
#include <boost/thread.hpp>
#include <iostream>
#include <p2p/define.hpp>

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::io_context;
using boost::asio::use_awaitable;

namespace bp {
namespace p2p {

    node::node(
        const std::string& listen_address, const std::string& listen_port)
    {
        auto listen_endpoint = *tcp::resolver(io_context_)
                                    .resolve(listen_address, listen_port,
                                        tcp::resolver::passive);
        acceptor_
            = std::make_unique<tcp::acceptor>(io_context_, listen_endpoint);
    }

    // TODO: guard against duplicate connection creation to the same
    // peer. One in response to incoming connection other via this call.
    awaitable<void> node::connect_to_peer(
        const std::string& host, const std::string& port)
    {
        auto peer_endpoint = *tcp::resolver(io_context_).resolve(host, port);
        auto client_socket = tcp::socket(io_context_);
        LOG_DEBUG << "Calling async connect...";
        co_await client_socket.async_connect(peer_endpoint, use_awaitable);
        LOG_DEBUG << "Connect returned...";
        co_spawn(
            io_context_, start_connection(std::move(client_socket)), detached);
    }

    awaitable<void> node::listen(tcp::acceptor& acceptor)
    {
        for (;;) {
            auto client = co_await acceptor.async_accept(use_awaitable);
            auto client_executor = client.get_executor();
            co_spawn(
                client_executor, start_connection(std::move(client)), detached);
        }
    }

    awaitable<void> node::start_connection(tcp::socket client)
    {
        auto client_connection
            = std::make_shared<connection>(std::move(client));
        add_connection(client_connection);
        auto ex = client.get_executor();
        co_spawn(ex, client_connection->receive_from_peer(), detached);
        co_await client_connection->send_to_peer("ping\r\n");
    }

    void node::start(const std::string& peer_host, const std::string& peer_port)
    {
        co_spawn(io_context_, listen(*acceptor_), detached);

        // brute force stop context for now.
        boost::asio::signal_set signals(io_context_, SIGINT, SIGTERM);
        signals.async_wait([&](auto, auto) { io_context_.stop(); });

        co_spawn(io_context_, connect_to_peer(peer_host, peer_port), detached);

        for (unsigned i = 0; i < boost::thread::hardware_concurrency(); ++i)
            threads_.create_thread(boost::bind(&io_context::run, &io_context_));
        threads_.join_all();
    }

    void node::stop()
    {
        // TODO: Close all connections
        // TODO: Stop all threads
        std::cerr << "Stopped." << std::endl;
        exit(0);
    }

    void node::add_connection(connection::ptr con)
    {
        connections_mutex_.lock();
        connections_.emplace_back(con);
        connections_mutex_.unlock();
    }

    void node::remove_connection(connection::ptr con)
    {
        // connections_mutex_.lock();
        // connections_.erase(con);
        // connections_mutex_.unlock();
    }

}
}
