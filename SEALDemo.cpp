#include "seal/seal.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace seal;

int main() {

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

	Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

	// This x is the value that is going to be encrypted
	int x = 6;
    Plaintext x_plain(to_string(x));
    cout << "Express x = " + to_string(x) + " as a plaintext polynomial 0x" + x_plain.to_string() + "." << endl;

	Ciphertext x_encrypted;
    cout << "Encrypt x_plain to x_encrypted." << endl;
    encryptor.encrypt(x_plain, x_encrypted);
    
	// saving the encrypted values into the file
    ofstream encrypted_file;
    encrypted_file.open("encrypted.txt");
    x_encrypted.save(encrypted_file);
    encrypted_file.close();

    cout << "    + size of freshly encrypted x: " << x_encrypted.size() << endl;

    cout << "    + noise budget in freshly encrypted x: " << decryptor.invariant_noise_budget(x_encrypted) << " bits" << endl;


	Ciphertext input;
	ifstream input_encrypted_file;
	input_encrypted_file.open("encrypted.txt");
	input.unsafe_load(context, input_encrypted_file);
	input_encrypted_file.close();

	Plaintext x_decrypted;
    cout << "    + decryption of x_encrypted: ";
    decryptor.decrypt(input, x_decrypted);
    cout << "0x" << x_decrypted.to_string() << " ...... Correct." << endl;

	cout << "Decrypted value : " << x_decrypted.to_string() << endl;

	return 0;
}
