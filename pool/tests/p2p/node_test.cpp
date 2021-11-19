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
#include <gtest/gtest.h>

using namespace bp;
using namespace bp::p2p;

TEST(NodeTest, ConstructorReturnsNode) { node("localhost", "9999"); }

TEST(NodeTest, NormalStartStopShouldStopClealy)
{
    node node_ { "localhost", "9999" };
    node_.start("localhost", "9998");
    //    EXPECT_EXIT(node_.stop(), testing::ExitedWithCode(0), "Stopped.");
}

// TEST(NodeTest, StartShouldCallAsynchConnectOnSocket) { }

// TEST(NodeTest, StartShouldCallStartConnection) { }

// TEST(NodeTest, StartConnectionShouldCreateConnection) { }

// TEST(NodeTest, StartConnectionShouldCallReceiveFromPeer) { }

// TEST(NodeTest, StartConnectionShouldCallSendToPeer) { }
