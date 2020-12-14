#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"

int main(){
	// client side
    SealWrapperClient sealClient((size_t)32768, 881);

    // server side
    SealWrapperServer sealServer(sealClient._poly_modulus_degree, sealClient._plain_modulus, sealClient._relin_key);
    Comparator comparator(sealServer._evaluator, sealServer._relin_keys);
    
    Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);
	
	vector<Ciphertext> A = { x_1_encrypted, x_0_encrypted,x_1_encrypted, x_0_encrypted };
	vector<Ciphertext> B = { x_1_encrypted, x_0_encrypted,x_1_encrypted, x_0_encrypted };

	auto init = make_tuple(x_0_encrypted, x_1_encrypted, x_0_encrypted);

	auto res = comparator.compareNBits(tuple<Ciphertext, Ciphertext, Ciphertext>{x_0_encrypted, x_1_encrypted, x_0_encrypted},\
													A, B, *sealServer._evaluator);

	cout << "A>B:" << sealClient.decrypt_toString(get<0>(res)) <<\
			" A=B:" << sealClient.decrypt_toString(get<1>(res)) <<\
			" A<B:" << sealClient.decrypt_toString(get<2>(res)) << endl;

    return 0;

}

