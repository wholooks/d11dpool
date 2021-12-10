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

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <p2p/connection.hpp>
#include <p2p/define.hpp>
#include <p2p/node.hpp>

using namespace bp;
using namespace bp::p2p;
using boost::asio::awaitable;

using ::testing::AtLeast;
using ::testing::Return;

// class mock_socket {
//     io_context::executor_type ex_;
//     std::byte* ptr_;
//     std::size_t size_;
//     io_context::executor_type get_executor() noexcept { return ex_; }
//     template <class ConstBufferSequence, class Handler>
//     void async_write_some(ConstBufferSequence cb, Handler&& h);
// };

class mock_connection : public connection {
public:
    mock_connection(tcp::socket socket)
        : connection(std::move(socket))
    {
    }
    MOCK_METHOD((awaitable<bool>), start, (), ());
};

using test_node = bp::p2p::node<tcp::socket, mock_connection>;

TEST(NodeTest, ConstructorReturnsNode) { test_node("localhost", "9999"); }

TEST(NodeTest, NormalStartStopShouldStopClealy)
{
    test_node node_ { "localhost", "9999" };
    node_.setup_initial_peers("localhost", "9998");
    EXPECT_EXIT(node_.stop(), testing::ExitedWithCode(0), "Stopped.");
}

TEST(NodeTest, StartConnectionShouldCreateConnection)
{
    io_context io_context_;
    tcp::socket socket_(io_context_);
    auto connection_ = std::make_shared<connection>(std::move(socket_));
    test_node node_ { "localhost", "9999" };
    // EXPECT_CALL(*connection_, start()).Times(AtLeast(1));

    bool started = false;
    co_spawn(io_context_, node_.start_connection(connection_),
        [&](std::exception_ptr e) {
            LOG_DEBUG << "X";
            started = true;
            return;
        });
    LOG_DEBUG << "A";
    io_context_.run();
    LOG_DEBUG << "B";
    EXPECT_TRUE(started) << "Failed to start connection";
}

// TEST(NodeTest, StartConnectionShouldCallReceiveFromPeer) { }

// TEST(NodeTest, StartConnectionShouldCallSendToPeer) { }
