/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef SHA512_H
#define SHA512_H

/**
 * @brief Hashes some data.
 *
 * The hash memory space is to be allocated before calling
 * this function, and thus the parameter @p hash should point
 * to some accessible space containing at least 64
 * available bytes.
 *
 * The hash will always contain exactly 512 bits, i.e. 64 bytes.
 *
 * @param data Pointer to the data to hash.
 * @param len Length of the data to hash.
 * @param hash Pointer to where the hash is to be stored.
 */
void hash_sha512(const char *data, int len, char *hash);

#endif /* End of SHA512_H */
