#include "FHE.h"

/*
int main () {   

	SEALContext context = create_context();
    gen_keys(context, SK_fname, PK_fname);

    PublicKey public_key = load_PK_from_file(context, PK_fname);
    SecretKey secret_key = load_SK_from_file(context, SK_fname);

    // Writing encrypted values to file
    ofstream encrypted_file_out;
    encrypted_file_out.open(input_fname, ios::binary);
    
    for (int value = 0; value < 20; value ++)
        encrypt_value(context, value, public_key, encrypted_file_out);

    encrypted_file_out.close();

    // Reading encrypted values from file

    ifstream encrypted_file_in;
	encrypted_file_in.open(input_fname);
    for (int value = 0; value < 20; value ++)
        decrypt_value(context, secret_key, encrypted_file_in);
    encrypted_file_in.close();
}*/

/**
 * Creates a context with: 
 *  bfv scheme type
 *  poly_modulus_degree = 4096
 *  plain_modulus = 1024
 *  @return context, configured with the values above
 */
SEALContext create_context() {
    EncryptionParameters parms(scheme_type::bfv);
	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
	parms.set_plain_modulus(1024);
    SEALContext context(parms);
    if (DEBUG)
        cout << "[DEBUG] Created context" << endl;
    return context;
}

/**
 * Reads the file the the secret key and returns it in SecretKey type
 * @param context Context of SEAL
 * @param secret_key_fname Name of the file containing the secret key
 * @return Secret Key
 */ 
SecretKey load_SK_from_file(SEALContext context, const char *secret_key_fname) {
    // read secret key
    SecretKey secret_key;
    ifstream secret_key_f;
    secret_key_f.open(secret_key_fname, ios::binary);
	secret_key.load(context, secret_key_f);
	secret_key_f.close();
    if (DEBUG)
        cout << "[DEBUG] Loaded Secret Key from file: " << secret_key_fname << endl;
    return secret_key;
}

/**
 * Reads the file the the public key and returns it in PublicKey type
 * @param context Context of SEAL
 * @param public_key_fname Name of the file containing the public key
 * @return Public Key
 */ 
PublicKey load_PK_from_file(SEALContext context, const char *public_key_fname) {
    //read public key
    PublicKey public_key;
    ifstream public_key_f;
	public_key_f.open(public_key_fname);
	public_key.unsafe_load(context, public_key_f);
	public_key_f.close();
    if (DEBUG)
        cout << "[DEBUG] Loaded Public Key from file: " << public_key_fname << endl;
    return public_key;
}

/**
 * Generates the secret and public keys
 * @param context Context of SEAL
 * @param secret_key_fname Name of secret key file
 * @param public_key_fname Name of public key file
 */
void gen_keys (SEALContext context, const char *secret_key_fname, const char *public_key_fname) {
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

    if (DEBUG)
        cout << "[DEBUG] Generated Public and Secret Keys" << endl;
    
}

/**
 * Encrypts the value into a file
 * @param context Context of SEAL
 * @param value Value to be encrypted
 * @param public_key Public key to encrypt value
 * @param encrypted_file File to save the encryption onto
 */
void encrypt_value(SEALContext context, int value, PublicKey public_key, ofstream &encrypted_file) {
    stringstream val_hex;
    val_hex << std::hex << value;
    Plaintext x_plain(val_hex.str());

    // Encrypt the information
    Encryptor encryptor(context, public_key);
    Ciphertext x_encrypted;
    encryptor.encrypt(x_plain, x_encrypted);
    
    // write encrypted values to output files
    x_encrypted.save(encrypted_file);
    if (DEBUG)
        cout << "[DEBUG] Encrypted value   -> \tdecimal: " << to_string(value) << " \t hex: " << val_hex.str() << endl;
}

/**
 * Decrypts next value from the file
 * @param context Context of SEAL
 * @param secret_key Secret key to decrypt value
 * @param encrypted_file File to read the encrypted data from
 * @return Integer with decrypted value
 */
int decrypt_value(SEALContext context, SecretKey secret_key, ifstream &encrypted_file) {

    Decryptor decryptor(context, secret_key);

    // read the encrypted file
    Ciphertext input;

	input.load(context, encrypted_file);

    Plaintext x_decrypted;
    decryptor.decrypt(input, x_decrypted);

    if (DEBUG)
        cout << "[DEBUG] Decrypted value   -> \tdecimal: " << (int)strtol(x_decrypted.to_string().c_str(), NULL, 16) << " \t hex: " << x_decrypted.to_string() << endl;
    return (int)strtol(x_decrypted.to_string().c_str(), NULL, 16);
}
