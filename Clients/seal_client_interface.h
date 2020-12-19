#ifndef SEAL_CLIENT_INTERFACE_H
#define SEAL_CLIENT_INTERFACE_H

#include "client.h"
#include "../config.h"

#include <bitset>

class SealWrapperClient {
    private:
        vector<Modulus> _coeff_modulus;
        EncryptionParameters* _params;

    protected:
        KeyGenerator* _keygen;
        Decryptor* _decryptor;

    public:
        size_t _poly_modulus_degree;
        int _plain_modulus;

        SEALContext* ctx;

        Encryptor* _encryptor;

        PublicKey* _public_key;
        SecretKey* _secret_key;

        SealWrapperClient(size_t poly_modulus_degree, int plain_modulus){
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

            load_SK_from_file(SK_fname);
            load_PK_from_file(PK_fname);

            // set backbone functionality of Seal
            static Encryptor encryptor(*ctx, *_public_key);
            static Decryptor decryptor(*ctx, *_secret_key);

            _encryptor = &encryptor;
            _decryptor = &decryptor;
        }

        Ciphertext encrypt(int input){
            Plaintext input_plain(to_string(input));
            Ciphertext input_encrypted;
            _encryptor->encrypt(input_plain, input_encrypted);
            return input_encrypted;
        }

        Ciphertext encrypt_from_plain(Plaintext input){
            Ciphertext input_encrypted;
            _encryptor->encrypt(input, input_encrypted);
            return input_encrypted;
        }

        Plaintext decrypt(Ciphertext input){
            Plaintext input_decrypted;
            _decryptor->decrypt(input, input_decrypted);
            return input_decrypted;
        }

        string decrypt_to_string(Ciphertext input){
            Plaintext input_decrypted;
            _decryptor->decrypt(input, input_decrypted);
            return input_decrypted.to_string();
        }

        void load_SK_from_file(const char *secret_key_fname) {
            try{
                ifstream secret_key_f;
                _secret_key = new SecretKey();
                secret_key_f.open(secret_key_fname, ios::binary);
                _secret_key->load(*ctx, secret_key_f);
                secret_key_f.close();

                if (DEBUG)
                    cout << "[DEBUG] Loaded Secret Key from file: " << secret_key_fname << endl;
            }catch(std::exception const& e){
                cout << "There was an error loading de SK from file: " << e.what() << endl;
            }
        }

        void load_PK_from_file(const char *public_key_fname) {
            try{
                //read public key
                ifstream public_key_f;
                _public_key = new PublicKey();
                public_key_f.open(public_key_fname);
                _public_key->unsafe_load(*ctx, public_key_f);
                public_key_f.close();

                if (DEBUG)
                    cout << "[DEBUG] Loaded Public Key from file: " << public_key_fname << endl;
            }catch(std::exception const& e){
                cout << "There was an error loading de PK from file: " << e.what() << endl;
            }

        }

        void encrypt_and_save(int value, ofstream &encrypted_file) {
            stringstream val_hex;
            val_hex << std::hex << value;
            Plaintext x_plain(val_hex.str());

            // Encrypt the information
            Ciphertext x_encrypted;
            _encryptor->encrypt(x_plain, x_encrypted);

            // write encrypted values to output files
            x_encrypted.save(encrypted_file);

            std::string binary = std::bitset<NUM_MAX_BITS>(value).to_string(); //to binary
            encrypt_and_save_binary(binary, encrypted_file);
            if (DEBUG)
                cout << "[DEBUG] Encrypted value   -> \tdecimal: " << to_string(value) << " \t hex: " << val_hex.str() << " \t bin: " << binary << endl;
        }

        void encrypt_and_save_binary (string binary, ofstream &encrypted_file) {
            for (char const &c: binary) {
                Plaintext x_plain(c);

                // Encrypt the information
                Ciphertext x_encrypted;
                _encryptor->encrypt(x_plain, x_encrypted);

                // write encrypted values to output files
                x_encrypted.save(encrypted_file);
            }
        }


        int decrypt_from_file(ifstream &encrypted_file) {
            // read the encrypted file
            Ciphertext input;

        	input.load(*ctx, encrypted_file);

            Plaintext x_decrypted;
            _decryptor->decrypt(input, x_decrypted);

            if (DEBUG)
                cout << "[DEBUG] Decrypted value   -> \tdecimal: " << (int)strtol(x_decrypted.to_string().c_str(), NULL, 16) << " \t hex: " << x_decrypted.to_string() << endl;
            return (int)strtol(x_decrypted.to_string().c_str(), NULL, 16);
        }

        EncryptionParameters getParams(){ return *_params; }

        PublicKey getPublicKey(){ return *_public_key; }
};

#endif
