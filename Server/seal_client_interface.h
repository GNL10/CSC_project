#ifndef SEAL_CLIENT_INTERFACE_H
#define SEAL_CLIENT_INTERFACE_H

#include "server.h"

class SealWrapperClient {
    private:
        vector<Modulus> _coeff_modulus;

        EncryptionParameters* _params;
        SEALContext* ctx;
        SecretKey* _secret_key;
        PublicKey* _public_key;

    protected:
        KeyGenerator* _keygen;
        Decryptor* _decryptor;

    public:
        size_t _poly_modulus_degree;
        int _plain_modulus;

        Evaluator* _evaluator;
        Encryptor* _encryptor;
        RelinKeys* _relin_key;

        SealWrapperClient(size_t poly_modulus_degree, int plain_modulus = 1024){
            static EncryptionParameters params(scheme_type::bfv);

            _poly_modulus_degree = poly_modulus_degree;

            // set poly modulus degree
            params.set_poly_modulus_degree(_poly_modulus_degree);

            // set coeff modulus
            _coeff_modulus = CoeffModulus::BFVDefault(_poly_modulus_degree);

            params.set_coeff_modulus(_coeff_modulus);

            // set plain modulus
            _plain_modulus = plain_modulus;
            params.set_plain_modulus(_plain_modulus);
            _params = &params;

            static SEALContext context(*_params);
            ctx = &context;



            // set backbone functionality of Seal
            static Encryptor encryptor(*ctx, *_public_key);
            static Decryptor decryptor(*ctx, *_secret_key);
            static Evaluator evaluator(*ctx);

            static RelinKeys relin_keys;
            _keygen->create_relin_keys(relin_keys);
            _relin_key = &relin_keys;

            _encryptor = &encryptor;
            _decryptor = &decryptor;
            _evaluator = &evaluator;
        }

        Ciphertext encrypt(int input){
            Plaintext input_plain(to_string(input));
            Ciphertext input_encrypted;
            _encryptor->encrypt(input_plain, input_encrypted);
            return input_encrypted;
        }

        Ciphertext encrypt_fromPlaintext(Plaintext input){
            Ciphertext input_encrypted;
            _encryptor->encrypt(input, input_encrypted);
            return input_encrypted;
        }

        Plaintext decrypt(Ciphertext input){
            Plaintext input_decrypted;
            _decryptor->decrypt(input, input_decrypted);
            return input_decrypted;
        }

        string decrypt_toString(Ciphertext input){
            Plaintext input_decrypted;
            _decryptor->decrypt(input, input_decrypted);
            return input_decrypted.to_string();
        }

        EncryptionParameters getParams(){ return *_params; }
        PublicKey getPublicKey(){ return *_public_key; }

        /**
         * Reads the file the the secret key and returns it in SecretKey type
         * @param context Context of SEAL
         * @param secret_key_fname Name of the file containing the secret key
         * @return Secret Key
         */
        SecretKey load_SK_from_file(SEALContext context, const char *secret_key_fname) {
            // read secret key
            ifstream secret_key_f;
            secret_key_f.open(secret_key_fname, ios::binary);
        	_secret_key->load(context, secret_key_f);
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
        void load_PK_from_file(const char *public_key_fname) {
            //read public key
            ifstream public_key_f;
        	public_key_f.open(public_key_fname);
        	_public_key->unsafe_load(context, public_key_f);
        	public_key_f.close();

            if (DEBUG)
                cout << "[DEBUG] Loaded Public Key from file: " << public_key_fname << endl;
            return public_key;
        }

        /**
         * Encrypts the value into a file
         * @param value Value to be encrypted
         * @param encrypted_file File to save the encryption onto
         */
        void encrypt_value(int value, ofstream &encrypted_file) {
            stringstream val_hex;
            val_hex << std::hex << value;
            Plaintext x_plain(val_hex.str());

            // Encrypt the information
            Ciphertext x_encrypted;
            _encryptor->encrypt(x_plain, x_encrypted);

            // write encrypted values to output files
            x_encrypted.save(encrypted_file);
            if (DEBUG)
                cout << "[DEBUG] Encrypted value   -> \tdecimal: " << to_string(value) << " \t hex: " << val_hex.str() << endl;
        }

        /**
         * Decrypts next value from the file
         * @param encrypted_file File to read the encrypted data from
         * @return Integer with decrypted value
         */
        int decrypt_value(ifstream &encrypted_file) {
            // read the encrypted file
            Ciphertext input;

        	input.load(context, encrypted_file);

            Plaintext x_decrypted;
            _decryptor->decrypt(input, x_decrypted);

            if (DEBUG)
                cout << "[DEBUG] Decrypted value   -> \tdecimal: " << (int)strtol(x_decrypted.to_string().c_str(), NULL, 16) << " \t hex: " << x_decrypted.to_string() << endl;
            return (int)strtol(x_decrypted.to_string().c_str(), NULL, 16);
        }

};

#endif
