// Copyright (c) 2017 Pieter Wuille
// Copyright (c) 2017 The Bitcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

// Cashaddr is an address format inspired by bech32.

#ifndef _BCHUTILS_CASHADDR_H_
#define _BCHUTILS_CASHADDR_H_

#include <cstdint>
#include <string>
#include <vector>
#include <tuple>

namespace bchutils
{

/**
 * Encode a cashaddr string. Returns the empty string in case of failure.
 */
std::string Encode(uint8_t type,
                   const std::string &prefix,
                   const std::vector<uint8_t> &values);

/**
 * Decode a cashaddr string. Returns (type, prefix, data). Empty prefix means failure.
 */
std::tuple<uint8_t, std::string, std::vector<uint8_t>>
Decode(const std::string &str, const std::string &default_prefix);

}

#endif
