/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef BIGENDIAN_H
#define BIGENDIAN_H

/**
 * @file bigendian.h
 * @brief Integer conversions to Big Endian encoding.
 *
 * This is only a small utility file to get the usual
 * htonl, htons, ntohl and ntohs functions to convert
 * 16-bit and 32-bit integers to and from big endian
 * encoding (versus host encoding).
 */

#include <cstdint>

#ifdef BIGENDIAN_CPP_IMPLEMENTATION
#undef BIGENDIAN_CPP_IMPLEMENTATION
#endif

#include "bigendian_cfg.h"

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || \
  defined(__WINDOWS__)
#include <Winsock2.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#else
#define BIGENDIAN_CPP_IMPLEMENTATION

#if BIGENDIAN_CFG && !defined(DOXYGEN_COMPILATION)

#if BIGENDIAN_INV16
#define _TO8B reinterpret_cast<uint8_t*>
inline uint16_t htons(const uint16_t hostshort) {
    uint16_t result;
    _TO8B(result)[0] = _TO8B(hostshort)[1];
    _TO8B(result)[1] = _TO8B(hostshort)[0];
    return result;
}
#else
inline uint16_t htons(const uint16_t hostshort) { return hostshort; }
#endif /* End of BIGENDIAN_INV16 */

#else

/**
 * @brief Converts from host to network byte order.
 * @param hostshort The host 16-bit integer.
 * @return The network 16-bit integer.
 */
uint16_t htons(uint16_t hostshort);

#endif /* End of BIGENDIAN_CFG */

#if BIGENDIAN_CFG && !defined(DOXYGEN_COMPILATION)

#define _TO16B reinterpret_cast<uint16_t*>

inline uint32_t htonl(const uint32_t hostlong) {
    uint32_t result;
    _TO16B(&result)[0] = htonl(_TO16B(hostlong[BIGENDIAN_INV32]));
    _TO16B(&result)[1] = htonl(_TO16B(hostlong[1 - BIGENDIAN_INV32]));
    return result;
}

#else

/**
 * @brief Converts from host to network byte order.
 * @param hostlong The host 32-bit integer.
 * @return The network 32-bit integer.
 */
uint32_t htonl(uint32_t hostlong);

#endif /* End of BIGENDIAN_CFG */

#define ntohl(n) htonl(n)
#define ntohs(n) htons(n)

#ifdef DOXYGEN_COMPILATION

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 32-bit integer.
 * @return The host 32-bit integer.
 */
uint32_t ntohl(uint32_t netlong);

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 16-bit integer.
 * @return The host 16-bit integer.
 */
uint16_t ntohs(uint16_t netshort);

#endif /* End of DOXYGEN_COMPILATION */

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION */

#if BIGENDIAN_CFG && !defined(DOXYGEN_COMPILATION)

#define _TO32B reinterpret_cast<uint32_t*>

inline uint64_t htonll(const uint64_t hostlong) {
    uint64_t result;
    _TO32B(&result)[0] = htonl(_TO32B(hostlong[BIGENDIAN_INV64]));
    _TO32B(&result)[1] = htonl(_TO32B(hostlong[1 - BIGENDIAN_INV64]));
    return result;
}

#else

/**
 * @brief Converts from host to network byte order.
 * @param hostlong The host 64-bit integer.
 * @return The network 64-bit integer.
 */
uint64_t htonll(uint64_t hostlong);

#endif /* End of BIGENDIAN_CFG */

#define ntohll(n) htonll(n)

#ifdef DOXYGEN_COMPILATION

/**
 * @brief Converts from network to host byte order.
 * @param netlong The network 64-bit integer.
 * @return The host 64-bit integer.
 */
uint64_t ntohll(uint64_t netlong);

#endif /* End of DOXYGEN_COMPILATION */

#endif /* End of BIGENDIAN_H */
