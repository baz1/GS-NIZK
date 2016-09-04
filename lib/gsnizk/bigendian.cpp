/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "bigendian.h"

#ifdef BIGENDIAN_CPP_IMPLEMENTATION

#define BYTE_VALUE_16(i,b) (static_cast<uint16_t>( \
    reinterpret_cast<uint8_t*>(&i)[b]))

uint16_t htons(uint16_t hostshort) {
    return (BYTE_VALUE_16(hostshort, 0) << 8) |
            BYTE_VALUE_16(hostshort, 1);
}

#define BYTE_VALUE_32(i,b) (static_cast<uint32_t>( \
    reinterpret_cast<uint8_t*>(&i)[b]))

uint32_t htonl(uint32_t hostlong) {
    return (BYTE_VALUE_32(hostlong, 0) << 24) |
            (BYTE_VALUE_32(hostlong, 1) << 16) |
            (BYTE_VALUE_32(hostlong, 2) << 8) |
            BYTE_VALUE_32(hostlong, 3);
}

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION */

uint64_t htonll(uint64_t hostlong) {
    return (static_cast<uint64_t>(
            ntohl(reinterpret_cast<uint32_t*>(&hostlong)[0])) << 32) |
            ntohl(reinterpret_cast<uint32_t*>(&hostlong)[1]);
}
