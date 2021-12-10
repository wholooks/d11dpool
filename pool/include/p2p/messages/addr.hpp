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

#ifndef BP_MESSAGE_HPP
#define BP_MESSAGE_HPP

#include "message.hpp"
#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <memory>
#include <p2p/define.hpp>

namespace bp {
namespace p2p {
    namespace messages {

        using boost::asio::const_buffer;

        /// Abstract class for all network messages.
        ///
        /// Provides `to_buffer` and `from_buffer` messages for serialization
        /// and deserialization.
        class address : private boost::noncopyable {
            address(const_buffer& from_buffer);
            virtual const_buffer& to_buffer();
        };
    }
}
}
#endif
