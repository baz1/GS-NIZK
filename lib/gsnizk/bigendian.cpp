/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#include "bigendian.h"

#ifdef BIGENDIAN_CPP_IMPLEMENTATION

uint16_t htons(uint16_t hostshort) {
    U16 input;
    input.u16 = hostshort;
    return (static_cast<uint16_t>(input.u8[0]) << 8) |
            static_cast<uint16_t>(input.u8[1]);
}

uint32_t htonl(uint32_t hostlong) {
    U32 input;
    input.u32 = hostlong;
    return (static_cast<uint32_t>(input.u8[0]) << 24) |
            (static_cast<uint32_t>(input.u8[1]) << 16) |
            (static_cast<uint32_t>(input.u8[2]) << 8) |
            static_cast<uint32_t>(input.u8[3]);
}

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION */

#ifdef BIGENDIAN_CPP_IMPLEMENTATION_64

uint64_t htonll(uint64_t hostlong) {
    U64 input;
    input.u64 = hostlong;
    return (static_cast<uint64_t>(ntohl(input.u32[0])) << 32) |
            ntohl(input.u32[1]);
}

#endif /* End of BIGENDIAN_CPP_IMPLEMENTATION_64 */
