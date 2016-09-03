/*
 * Copyright (c) 2016, Remi Bazin <bazin.remi@gmail.com>
 * See LICENSE for licensing details.
 */

#ifndef SHA256_H
#define SHA256_H

/**
 * @brief Hashes some data.
 *
 * The hash memory space is to be allocated before calling
 * this function, and thus the parameter @p hash should point
 * to some accessible space containing at least 32
 * available bytes.
 *
 * The hash will always contain exactly 256 bits, i.e. 32 bytes.
 *
 * @param data Pointer to the data to hash.
 * @param len Length of the data to hash.
 * @param hash Pointer to where the hash is to be stored.
 */
void hash_sha256(const char *data, int len, char *hash);

#endif /* End of SHA256_H */
