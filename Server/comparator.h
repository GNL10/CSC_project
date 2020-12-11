#ifndef COMPARATOR_H
#define COMPARATOR_H

#include "server.h"

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

        Ciphertext _and_3_inputs(Ciphertext a, Ciphertext b, Ciphertext c, Evaluator &eval){
            eval.multiply_inplace(a,b);
            eval.multiply_inplace(a,c);
            return a;
        }

        tuple<Ciphertext, Ciphertext, Ciphertext> \
        compare1bit( tuple<Ciphertext, Ciphertext, Ciphertext> last_proccess, Ciphertext a_i, Ciphertext b_i, Evaluator &eval){
            Ciphertext next_greater_than = _and(a_i,_not(b_i, eval), eval);
            Ciphertext next_equal = _not(_or(_and(_not(a_i, eval), b_i, eval), _and(a_i, _not(b_i, eval), eval), eval), eval);
            Ciphertext next_smaller_than = _and(_not(a_i, eval), b_i, eval);
            // falta manipular last_proccess
            return make_tuple(next_greater_than, next_equal, next_smaller_than);
        }

};

#endif