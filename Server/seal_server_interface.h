#ifndef SEAL_SERVER_INTERFACE_H
#define SEAL_SERVER_INTERFACE_H

#include "server.h"

class SealWrapperServer {
    private:
        EncryptionParameters* _params;
        SEALContext* ctx;

    public:
        Evaluator* _evaluator;
        SealWrapperServer(){
            static EncryptionParameters params(scheme_type::bfv);
            params.set_poly_modulus_degree(4096);
            params.set_coeff_modulus(CoeffModulus::BFVDefault(4096));
            params.set_plain_modulus(1024);
            _params = &params;

            static SEALContext context(*_params);
            ctx = &context;

            // set backbone functionality of Seal
            static Evaluator evaluator(*ctx);
            _evaluator = &evaluator;
        }
};

#endif
