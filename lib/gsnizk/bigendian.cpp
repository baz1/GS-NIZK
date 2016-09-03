/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "bigendian.h"

#ifdef BIGENDIAN_CPP_IMPLEMENTATION

#define BYTE_VALUE(i,b) reinterpret_cast<uint8_t*>(&i)[b]

uint32_t htonl(uint32_t hostlong) {
    uint32_t result;
    BYTE_VALUE(result, 0) = static_cast<uint8_t>(hostlong >> 24);
    BYTE_VALUE(result, 1) = static_cast<uint8_t>((hostlong >> 16) & 0xFF);
    BYTE_VALUE(result, 2) = static_cast<uint8_t>((hostlong >> 8) & 0xFF);
    BYTE_VALUE(result, 3) = static_cast<uint8_t>(hostlong & 0xFF);
    return result;
}

uint16_t htons(uint16_t hostshort) {
    uint16_t result;
    BYTE_VALUE(result, 0) = static_cast<uint8_t>(hostshort >> 8);
    BYTE_VALUE(result, 1) = static_cast<uint8_t>(hostshort & 0xFF);
    return result;
}

#define BYTE_VALUE_32(i,b) (static_cast<uint32_t>( \
    reinterpret_cast<uint8_t*>(&i)[b]))

uint32_t ntohl(uint32_t netlong) {
    return (BYTE_VALUE_32(netlong, 0) << 24) |
            (BYTE_VALUE_32(netlong, 1) << 16) |
            (BYTE_VALUE_32(netlong, 2) << 8) |
            BYTE_VALUE_32(netlong, 3);
}

#define BYTE_VALUE_16(i,b) (static_cast<uint16_t>( \
    reinterpret_cast<uint8_t*>(&i)[b]))

uint16_t ntohs(uint16_t netshort) {
    return (BYTE_VALUE_16(netshort, 0) << 8) |
            BYTE_VALUE_16(netshort, 1);
}

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION */

uint64_t htonll(uint64_t hostlong) {
    uint64_t result;
    reinterpret_cast<uint32_t*>(&result)[0] = htonl(hostlong >> 32);
    reinterpret_cast<uint32_t*>(&result)[1] = htonl(hostlong & 0xFFFFFFFFULL);
    return result;
}

uint64_t ntohll(uint64_t netlong) {
    return (static_cast<uint64_t>(
            ntohl(reinterpret_cast<uint32_t*>(&netlong)[0])) << 32) |
            ntohl(reinterpret_cast<uint32_t*>(&netlong)[1]);
}
