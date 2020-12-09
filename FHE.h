#ifndef _FHE_H_
#define _FHE_H_

using namespace std;
using namespace seal;

SEALContext create_context();
SecretKey load_SK_from_file(SEALContext context, const char *secret_key_fname);
PublicKey load_PK_from_file(SEALContext context, const char *public_key_fname);

void gen_keys (SEALContext context, const char *secret_key_fname, const char *public_key_fname);
void encrypt_value(SEALContext context, int value, PublicKey public_key, ofstream &encrypted_file);
Plaintext decrypt_value(SEALContext context, SecretKey secret_key, ifstream &encrypted_file);
#endif