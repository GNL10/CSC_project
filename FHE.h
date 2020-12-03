#ifndef _FHE_H_
#define _FHE_H_

using namespace std;
using namespace seal;

void gen_key (const char *secret_key_fname, const char *public_key_fname);
void encrypt_value(int value, const char *public_key_fname, const char *encrypted_fname);
void decrypt_value(const char *secret_key_fname, const char *encrypted_fname);
#endif