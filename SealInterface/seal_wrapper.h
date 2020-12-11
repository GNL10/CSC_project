#pragma once

#include "seal/seal.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>

using namespace std;
using namespace seal;

class SealWrapperClient {
    private:
        size_t _poly_modulus_degree;
        vector<Modulus> _coeff_modulus;
        int _plain_modulus;

        EncryptionParameters* _params;
        SEALContext* ctx;
        KeyGenerator* _keygen;

        SecretKey* _secret_key;
        PublicKey* _public_key;

        Encryptor* _encryptor;
        Decryptor* _decryptor;

    public:
        Evaluator* _evaluator;

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

            // generate keys
            static KeyGenerator keygen(*ctx);
            _keygen = &keygen;

            static SecretKey secret_key = _keygen->secret_key();
            _secret_key = &secret_key;

            static PublicKey public_key;
            _keygen->create_public_key(public_key);
            _public_key = &public_key;

            // set backbone functionality of Seal
            static Encryptor encryptor(*ctx, *_public_key);
            static Decryptor decryptor(*ctx, *_secret_key);
            static Evaluator evaluator(*ctx);

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

};

class SealWrapperServer {
    private:
        EncryptionParameters* _params;
        PublicKey* _public_key;
        SEALContext* ctx;

        Encryptor* _encryptor;

    public:
        Evaluator* _evaluator;

        SealWrapperServer(EncryptionParameters params, PublicKey public_key){
            _public_key = &public_key;
            _params = &params;

            static SEALContext context(*_params);
            ctx = &context;

            // set backbone functionality of Seal
            static Evaluator evaluator(*ctx);
            static Encryptor encryptor(*ctx, *_public_key);

            _encryptor = &encryptor;
            _evaluator = &evaluator;
        }

        Ciphertext encrypt(int input){
            Plaintext input_plain(to_string(input));
            Ciphertext input_encrypted;
            _encryptor->encrypt(input_plain, input_encrypted);
            return input_encrypted;
        }
};

class SealWrapperServer {
    private:
        EncryptionParameters* _params;
        PublicKey* _public_key;
        SEALContext* ctx;

        Encryptor* _encryptor;

    public:
        Evaluator* _evaluator;

        SealWrapperServer(EncryptionParameters params, PublicKey public_key){
            _public_key = &public_key;
            _params = &params;

            static SEALContext context(*_params);
            ctx = &context;

            // set backbone functionality of Seal
            static Evaluator evaluator(*ctx);
            static Encryptor encryptor(*ctx, *_public_key);

            _encryptor = &encryptor;
            _evaluator = &evaluator;
        }

        Ciphertext encrypt(int input){
            Plaintext input_plain(to_string(input));
            Ciphertext input_encrypted;
            _encryptor->encrypt(input_plain, input_encrypted);
            return input_encrypted;
        }
};

class Comparator{
    public:
        Ciphertext _not(Ciphertext a, Evaluator &eval){
            Plaintext plain_one("1");
            eval.sub_plain_inplace(a, plain_one);
            eval.negate_inplace(a);
            return a;
        }

        Ciphertext _or(Ciphertext a, Ciphertext b, Evaluator &eval){
            Ciphertext c;
            eval.add(a, b, c);
            eval.multiply_inplace(a,b);
            eval.sub_inplace(c, a);
            return c;
        }

        Ciphertext _and(Ciphertext a, Ciphertext b, Evaluator &eval){
            eval.multiply_inplace(a,b);
            return a;
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compare( tuple<Ciphertext, Ciphertext, Ciphertext> last_proccess, Ciphertext a_i, Ciphertext b_i, Evaluator &eval){
            Ciphertext next_greater_than = _and(a_i,_not(b_i, eval), eval);
            Ciphertext next_equal = _not(_or(_and(_not(a_i, eval), b_i, eval), _and(a_i, _not(b_i, eval), eval), eval), eval);
            Ciphertext next_smaller_than = _and(_not(a_i, eval), b_i, eval);
            return make_tuple(next_greater_than, next_equal, next_smaller_than);
        }
};