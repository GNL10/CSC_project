#ifndef SEAL_SERVER_INTERFACE_H
#define SEAL_SERVER_INTERFACE_H

#include "server.h"

class SealWrapperServer {
    private:
        size_t _poly_modulus_degree;
        vector<Modulus> _coeff_modulus;
        int _plain_modulus;

        EncryptionParameters* _params;
        SEALContext* ctx;

    public:
        Evaluator* _evaluator;
        RelinKeys* _relin_keys;

        SealWrapperServer(size_t poly_modulus_degree, int plain_modulus, RelinKeys *relin_keys){
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

            _relin_keys = relin_keys;

            // set backbone functionality of Seal
            static Evaluator evaluator(*ctx);
            _evaluator = &evaluator;
        }

        Ciphertext load_ciphertext(ifstream &encrypted_file) {
            // read the encrypted file
            Ciphertext input;
        	input.load(*ctx, encrypted_file);
            return input;
        }

};

#endif
