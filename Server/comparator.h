#pragma once

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "server.h"

class Comparator{
    private:
        Evaluator* _evaluator;
        RelinKeys* _relin_keys;
    public:
        Comparator(Evaluator* evaluator, RelinKeys* relin_keys){
            _evaluator = evaluator;
            _relin_keys = relin_keys;
        }

        Ciphertext _not(Ciphertext a, Evaluator &eval){
            Plaintext plain_one("1");
            eval.sub_plain_inplace(a, plain_one);
            eval.negate_inplace(a);
            return a;
        }

        /*
            noise budget consumption: +/-20
            size after: +1
        */
        Ciphertext _or(Ciphertext a, Ciphertext b, Evaluator &eval){
            Ciphertext c;
            eval.add(a, b, c);
            eval.multiply_inplace(a,b);
            eval.relinearize_inplace(a, *_relin_keys);
            eval.sub_inplace(c, a);
            return c;
        }

        Ciphertext _and(Ciphertext a, Ciphertext b, Evaluator &eval){
            eval.multiply_inplace(a,b);
            eval.relinearize_inplace(a, *_relin_keys);
            return a;
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compare( tuple<Ciphertext, Ciphertext, Ciphertext> last_proccess, Ciphertext a_i, Ciphertext b_i, Evaluator &eval){
            Ciphertext next_greater_than = _and(a_i,_not(b_i, eval), eval);
            Ciphertext next_equal = _not(_or(_and(_not(a_i, eval), b_i, eval), _and(a_i, _not(b_i, eval), eval), eval), eval);
            Ciphertext next_smaller_than = _and(_not(a_i, eval), b_i, eval);
            return make_tuple(next_greater_than, next_equal, next_smaller_than);
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compareNBits( tuple<Ciphertext, Ciphertext, Ciphertext> init, vector<Ciphertext> A, vector<Ciphertext> B, Evaluator &eval){
            
            int n_bits = A.size();
            
            cout << "n bits: " << n_bits << endl;

            Ciphertext n_g;
            Ciphertext n_s;
            Ciphertext n_e;

            tuple<Ciphertext, Ciphertext, Ciphertext> buff;
            tuple<Ciphertext, Ciphertext, Ciphertext> in = init;

            for(int i=n_bits-1; i>=0; i--){
                buff = compare(in, A[i], B[i], eval);

                n_g = _or(get<0>(in), _and(get<1>(in), get<0>(buff), eval), eval);
                n_s = _or(get<2>(in), _and(get<1>(in), get<2>(buff), eval), eval);
                n_e = _not(_or(n_g, n_s, eval), eval);

                in = make_tuple(n_g, n_e, n_s);
            }
            
            return in;
        }

};

#endif