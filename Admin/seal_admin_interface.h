#ifndef SEAL_ADMIN_INTERFACE_H
#define SEAL_ADMIN_INTERFACE_H

#include "admin.h"

class SealWrapperAdmin {
    private:
        vector<Modulus> _coeff_modulus;
        EncryptionParameters* _params;
        KeyGenerator* _keygen;

        void gen_keys(){
            static SecretKey secret_key = _keygen->secret_key();
            static PublicKey public_key;
            static Serializable<RelinKeys> relin_keys = _keygen->create_relin_keys();;

            _keygen->create_public_key(public_key);

            _secret_key = &secret_key;
            _public_key = &public_key;
            _relin_key = &relin_keys;
        }

    public:
        size_t _poly_modulus_degree;
        int _plain_modulus;
        SEALContext* ctx;

        Serializable<RelinKeys>* _relin_key;
        SecretKey* _secret_key;
        PublicKey* _public_key;

        SealWrapperAdmin(size_t poly_modulus_degree, int plain_modulus){

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

            KeyGenerator keygen(*ctx);
            _keygen = &keygen;
            gen_keys();
        }

        void gen_and_upload_keys(const char *secret_key_fname, const char *public_key_fname, const char *relin_key_fname) {
            upload_SK_to_file(secret_key_fname);
            upload_PK_to_file(public_key_fname);
            upload_RK_to_file(relin_key_fname);
        }

        void upload_SK_to_file(const char *secret_key_fname) {
            ofstream secret_key_f;
            secret_key_f.open(secret_key_fname, ios::binary);
            _secret_key->save(secret_key_f);
            secret_key_f.close();
        }

        void upload_PK_to_file(const char *public_key_fname) {
            ofstream public_key_f;
            public_key_f.open(public_key_fname, ios::binary);
            _public_key->save(public_key_f);
            public_key_f.close();
        }

        void upload_RK_to_file(const char *relin_key_fname) {
            ofstream relin_key_f;
            relin_key_f.open(relin_key_fname, ios::binary);
            _relin_key->save(relin_key_f, compr_mode_type::none);
            relin_key_f.close();
        }
};

#endif
