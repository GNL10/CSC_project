#pragma once

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "test.h"

class Comparator{
    private:
    
        RelinKeys* _relin_keys;
    public:
        Evaluator* _eval;
        Comparator(Evaluator* evaluator, RelinKeys* relin_keys){
            _eval = evaluator;
            _relin_keys = relin_keys;
        }

        Ciphertext _not(Ciphertext a){
            Plaintext plain_one("1");
            _eval->sub_plain_inplace(a, plain_one);
            _eval->negate_inplace(a);
            return a;
        }

        Ciphertext _or(Ciphertext a, Ciphertext b){
            Ciphertext c;
            _eval->add(a, b, c);
            _eval->multiply_inplace(a,b);
            _eval->relinearize_inplace(a, *_relin_keys);
            _eval->sub_inplace(c, a);
            return c;
        }

        Ciphertext _and(Ciphertext a, Ciphertext b){
            _eval->multiply_inplace(a,b);
            _eval->relinearize_inplace(a, *_relin_keys);
            return a;
        }

        Ciphertext _f_and(Ciphertext a, Plaintext b){
            _eval->multiply_plain_inplace(a,b);
            _eval->relinearize_inplace(a, *_relin_keys);
            return a;
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compare(Ciphertext a_i, Ciphertext b_i){
            Ciphertext next_greater_than = _and(a_i,_not(b_i));
            Ciphertext next_equal = _not(_or(_and(_not(a_i), b_i), _and(a_i, _not(b_i))));
            Ciphertext next_smaller_than = _and(_not(a_i), b_i);
            return make_tuple(next_greater_than, next_equal, next_smaller_than);
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compareNBits(vector<Ciphertext> A, vector<Ciphertext> B){
            Ciphertext n_g, n_s, n_e;
            tuple<Ciphertext, Ciphertext, Ciphertext> buff, in;
            Plaintext plain_one("1");
            
            int n_bits = A.size();
            
            cout << "[DEBUG] Comparator :: n bits -> " << n_bits << endl;

            buff = compare(A[n_bits-1], B[n_bits-1]);
            
            n_g = _f_and(get<0>(buff), plain_one);
            n_s = _f_and(get<2>(buff), plain_one);
            n_e = _not(_or(n_g, n_s));
            
            in = make_tuple(n_g, n_e, n_s);
            
            for(int i=n_bits-2; i>=0; i--){
                buff = compare(A[i], B[i]);

                n_g = _or(get<0>(in), _and(get<1>(in), get<0>(buff)));
                n_s = _or(get<2>(in), _and(get<1>(in), get<2>(buff)));
                n_e = _not(_or(n_g, n_s));

                in = make_tuple(n_g, n_e, n_s);
            }

            return in;
        }

        Ciphertext sum(Ciphertext a, Ciphertext b){
            Ciphertext c;
            _eval->add(a, b, c);
            return c;
        }

        Ciphertext mult(Ciphertext a, Ciphertext b){
            Ciphertext c;
            _eval->multiply(a, b, c);
            _eval->relinearize_inplace(c, *_relin_keys);
            return c;
        }

        Ciphertext compose_mult(Ciphertext a, vector<Ciphertext> B){
            Ciphertext res;
            Ciphertext c;

            for (vector<Ciphertext>::iterator bit = B.begin(); bit != B.end(); bit++){
                _eval->multiply(a, *bit, c);
                _eval->relinearize_inplace(c, *_relin_keys);
                _eval->add_inplace(res, c);
            }
            
            
            return c;
        }

        static void insert_bits(vector<Ciphertext> From, vector<Ciphertext>& To){
            for(auto elem : From) To.push_back(elem);
        }
};

#endif