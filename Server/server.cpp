#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"

int main(){
    // receive params
    SealWrapperClient sealClient((size_t)4096, 1024);
    SealWrapperServer sealServer;
    Comparator comparator;

    /////////////////////////////////////////////////////////////////////////////////////
    // 1bit
    /*
    int A = 1;
    int B = 0;

    Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);

    Ciphertext value_x_encrypted = sealClient.encrypt(A);
    Ciphertext value_y_encrypted = sealClient.encrypt(B);

    auto [ logic_res1, logic_res2, logic_res3 ] = comparator.compare1bit(\
            tuple<Ciphertext, Ciphertext, Ciphertext>{x_0_encrypted, x_0_encrypted, x_1_encrypted},\
            value_x_encrypted, value_y_encrypted, *sealServer._evaluator);
    
    cout << "A>B:" << sealClient.decrypt_toString(logic_res1) <<\
            " A=B:" << sealClient.decrypt_toString(logic_res2) <<\
            " A<B:" << sealClient.decrypt_toString(logic_res3) << endl;
        */
	// Multiple bit
	int num_bits = 4;

	Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);

	vector<Ciphertext> A = {x_0_encrypted, x_0_encrypted, x_0_encrypted, x_1_encrypted};
	vector<Ciphertext> B = {x_0_encrypted, x_0_encrypted, x_1_encrypted, x_1_encrypted};
	
	auto init_carry = std::make_tuple(x_0_encrypted, x_1_encrypted, x_0_encrypted);

	auto logic_res = comparator.compare1bit(init_carry,\
            A[num_bits-1], B[num_bits-1], *sealServer._evaluator);

	for(int i=num_bits-2; i>=0; i--){
		cout << i << endl;
		auto _logic_res = comparator.compare1bit(logic_res, A[i], B[i], *sealServer._evaluator);
		logic_res = _logic_res;
	}

	cout << "A>B:" << sealClient.decrypt_toString(get<0>(logic_res)) <<\
	" A=B:" << sealClient.decrypt_toString(get<1>(logic_res)) <<\
	" A<B:" << sealClient.decrypt_toString(get<2>(logic_res)) << endl;


}

