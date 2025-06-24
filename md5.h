#ifndef MD5_H
#define MD5_H

#define MD5_DIGEST_LENGTH 16

typedef unsigned int UINT_4;
typedef unsigned char *POINTER;

typedef struct {
    UINT_4 state[4];    // state (ABCD)
    UINT_4 count[2];    // number of bits, modulo 2^64 (lsb first)
    unsigned char buffer[64]; // input buffer
} MD5_CTX;

int MD5One(unsigned char *data, unsigned int dataLen, unsigned char *digest);
void MD5Init(MD5_CTX *);
void MD5Update(MD5_CTX *, unsigned char *, unsigned int);
void MD5Final(unsigned char [16], MD5_CTX *);
int MD5Print(unsigned char *digest);

#endif
