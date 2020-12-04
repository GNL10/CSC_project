#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include "FHE.h"

using namespace std;
using namespace seal;

// guardo a secret key no ficheiro ou a public key?

char const *PK_fname = "public_key.txt";
char const *SK_fname = "secret_key.txt";
char const *input_fname  = "encrypted.txt";


int main () {
    gen_key(SK_fname, PK_fname);
    int val = 234;
    encrypt_value(val, PK_fname, input_fname);
    decrypt_value(SK_fname, input_fname);
}

// Gen key
void gen_key (const char *secret_key_fname, const char *public_key_fname) {
    
    EncryptionParameters parms(scheme_type::bfv);

	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

	parms.set_plain_modulus(1024);

	SEALContext context(parms);

	KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    ofstream secret_key_f;
    secret_key_f.open(secret_key_fname, ios::binary);
    secret_key.save(secret_key_f);
    secret_key_f.close();

    ofstream public_key_f;
    public_key_f.open(public_key_fname, ios::binary);
    public_key.save(public_key_f);
    public_key_f.close();
}

// Encrypt_value
void encrypt_value(int value, const char *public_key_fname, const char *encrypted_fname) {
    EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
	SEALContext context(parms);

    Plaintext x_plain(to_string(value));

    //read public key
    PublicKey public_key;
    ifstream public_key_f;
	public_key_f.open(public_key_fname);
	public_key.unsafe_load(context, public_key_f);
	public_key_f.close();


    // Encrypt the information
    Encryptor encryptor(context, public_key);
    Ciphertext x_encrypted;
    encryptor.encrypt(x_plain, x_encrypted);
    
    // write encrypted values to output files
    ofstream encrypted_file;
    encrypted_file.open(encrypted_fname, ios::binary);
    x_encrypted.save(encrypted_file);
    encrypted_file.close();

}


// Decrypt_value
void decrypt_value(const char *secret_key_fname, const char *encrypted_fname) {
    EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
	SEALContext context(parms);

    // read secret key
    SecretKey secret_key;
    ifstream secret_key_f;
    secret_key_f.open(secret_key_fname, ios::binary);
	secret_key.load(context, secret_key_f);
	secret_key_f.close();

    Decryptor decryptor(context, secret_key);

    // read the encrypted file
    Ciphertext input;
	ifstream input_encrypted_file;
	input_encrypted_file.open(encrypted_fname);
	input.unsafe_load(context, input_encrypted_file);
	input_encrypted_file.close();

    Plaintext x_decrypted;
    decryptor.decrypt(input, x_decrypted);

    cout << "Decrypted value : " << x_decrypted.to_string() << endl;
}