#ifndef MD5_H
#define MD5_H

#include <stddef.h>  // for size_t

#define MD5_DIGEST_LENGTH 16

typedef unsigned int UINT_4;
typedef unsigned char *POINTER;

typedef struct {
    UINT_4 state[4];         // state (ABCD)
    UINT_4 count[2];         // number of bits, modulo 2^64 (lsb first)
    unsigned char buffer[64]; // input buffer
} MD5_CTX;

// API functions
int MD5One(const unsigned char *data, size_t len, unsigned char *out_digest);
void MD5Init(MD5_CTX *context);
void MD5Update(MD5_CTX *context, const unsigned char *input, unsigned int inputLen);
void MD5Final(unsigned char digest[MD5_DIGEST_LENGTH], MD5_CTX *context);
int MD5Print(const unsigned char *out_digest);

#endif
