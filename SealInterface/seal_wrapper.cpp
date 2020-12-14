#include "seal_wrapper.h"

/* -> Como escolher poly_modulus_degree e plain_modulus

	    +----------------------------------------------------+
        | poly_modulus_degree | max coeff_modulus bit-length |
        +---------------------+------------------------------+
        | 1024                | 27                           |
        | 2048                | 54                           |
        | 4096                | 109                          |
        | 8192                | 218                          |
        | 16384               | 438                          |
        | 32768               | 881                          |
        +---------------------+------------------------------+

	sealClient((size_t)poly_modulus_degree, plain_modulus)
*/

int main(){
    SealWrapperClient sealClient((size_t)16384, 438);
    SealWrapperServer sealServer(sealClient.getParams(), sealClient.getPublicKey());
    Comparator comparator(sealServer._evaluator, sealClient._relin_key);

	Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);
	
	vector<Ciphertext> A = { x_1_encrypted, x_0_encrypted,x_1_encrypted, x_0_encrypted };
	vector<Ciphertext> B = { x_1_encrypted, x_0_encrypted,x_0_encrypted, x_1_encrypted };

	auto init = make_tuple(x_0_encrypted, x_1_encrypted, x_0_encrypted);
	
	auto res = comparator.compareNBits(tuple<Ciphertext, Ciphertext, Ciphertext>{x_0_encrypted, x_1_encrypted, x_0_encrypted},\
							A, B, *sealServer._evaluator);

	cout << "A>B:" << sealClient.decrypt_toString(get<0>(res)) <<\
            " A=B:" << sealClient.decrypt_toString(get<1>(res)) <<\
            " A<B:" << sealClient.decrypt_toString(get<2>(res)) << endl;

    return 0;
}