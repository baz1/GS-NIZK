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

#if defined(_WIN16) || defined(_WIN32) || defined(_WIN64) || \
  defined(__WINDOWS__)
#include <Winsock2.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#else
#define BIGENDIAN_CPP_IMPLEMENTATION

/**
 * @brief Converts from host to network byte order.
 * @param hostlong The host 32-bit integer.
 * @return The network 32-bit integer.
 */
uint32_t htonl(uint32_t hostlong);

/**
 * @brief Converts from host to network byte order.
 * @param hostshort The host 16-bit integer.
 * @return The network 16-bit integer.
 */
uint16_t htons(uint16_t hostshort);

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

#endif

#endif /* End of BIGENDIAN_H */
