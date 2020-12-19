#pragma once

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
        RelinKeys* _relin_key;

        SealWrapperServer(size_t poly_modulus_degree, int plain_modulus) {
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
            static Evaluator evaluator(*ctx);
            _evaluator = &evaluator;
        }

        Ciphertext load_ciphertext(ifstream &encrypted_file) {
            // read the encrypted file
            Ciphertext input;
        	input.load(*ctx, encrypted_file);
            return input;
        }

        void load_RK_from_file(const char *relin_key_fname) {
            try{
                ifstream relin_key_f;
                _relin_key = new RelinKeys();

                relin_key_f.open(relin_key_fname, ios::binary);
                _relin_key->load(*ctx, relin_key_f);
                relin_key_f.close();

                if (DEBUG)
                    cout << "[DEBUG] Loaded Relin Key from file: " << relin_key_fname << endl;
            }catch(std::exception const& e){
                cout << "There was an error loading de RK from Server folder: " << e.what() << endl;
            }
        }

};

#endif
