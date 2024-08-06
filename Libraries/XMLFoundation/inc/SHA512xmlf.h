/* sha.h sha512 and sha384 hash functions */
#ifndef SHA512_H
#define SHA512_H

#include <stdint.h>
#include <stdlib.h> // 2024 - added for definition of size_t on Ubuntu

void SHA512Hash(unsigned char* pzData, long long nDataLength, unsigned char* pDigestDest);


#define sha512_block_size 128
#define sha512_hash_size  64
#define sha384_hash_size  48

/* algorithm context */
typedef struct sha512_ctx_xmlf
{
	uint64_t message[16];   /* 1024-bit buffer for leftovers */
	uint64_t length;        /* number of processed bytes */
	uint64_t hash[8];       /* 512-bit algorithm internal hashing state */
	unsigned digest_length; /* length of the algorithm digest in bytes */
} sha512_ctx_xmlf;

void sha384_init_xmlf(sha512_ctx_xmlf* ctx);
void sha512_init_xmlf(sha512_ctx_xmlf* ctx);
void sha512_update_xmlf(sha512_ctx_xmlf* ctx, const unsigned char* data, size_t length);
void sha512_final_xmlf(sha512_ctx_xmlf* ctx, unsigned char* result);


#endif /* SHA512_H */
