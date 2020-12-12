#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"

int main(){
    // receive params
    SealWrapperClient sealClient((size_t)4096, 1024);
    SealWrapperServer sealServer;
    Comparator comparator(sealClient._evaluator, sealClient._keygen);

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

	vector<Ciphertext> A = {x_1_encrypted, x_0_encrypted, x_0_encrypted, x_1_encrypted};
	vector<Ciphertext> B = {x_0_encrypted, x_0_encrypted, x_1_encrypted, x_0_encrypted};

    ComparatorCarry init_carry = {x_0_encrypted, x_1_encrypted, x_0_encrypted};
	
    //auto logic_res =comparator.compare(init_carry, A[num_bits-1], B[num_bits-1], *sealServer._evaluator);
    /*
    auto logic_res = init_carry;

	auto [value1, value2, value3] = comparator.compare(logic_res, A[3], B[3], *sealServer._evaluator);
    Ciphertext a_1 = comparator._and(logic_res.equal,value1,*sealServer._evaluator);

    cout << "noise budget is: " << sealClient._decryptor->invariant_noise_budget(a_1) << " bits" << endl;
    a_2 = comparator._or(a_1,x_0_encrypted,*sealServer._evaluator);
    
    RelinKeys relin_keys;
    sealClient._keygen->create_relin_keys(relin_keys);
    sealClient._evaluator->relinearize_inplace(a_2, relin_keys);
    

    cout << "noise budget is: " << sealClient._decryptor->invariant_noise_budget(a_2) << " bits" << endl;
    */
   
    auto n_greater = comparator._or(value1,a_1,*sealServer._evaluator);
    auto n_smaller = comparator._or(logic_res.smaller,comparator._and(logic_res.equal,logic_res.smaller,*sealServer._evaluator),*sealServer._evaluator);
    ComparatorCarry _logic_res = {value1, value2, value3};
    
    cout << "A>B:" << sealClient.decrypt_toString(_logic_res.greater) <<\
	" A=B:" << sealClient.decrypt_toString(_logic_res.equal) <<\
	" A<B:" << sealClient.decrypt_toString(_logic_res.smaller) << endl;

    cout << "n_greater: " << sealClient.decrypt_toString(n_greater) <<\
	" n_smaller: " << sealClient.decrypt_toString(n_smaller) << "\n" << endl;

    cout << sealClient.decrypt_toString(comparator._or(x_0_encrypted, x_1_encrypted, *sealServer._evaluator))<< endl;
*/
    /*
    RelinKeys relin_keys;
    sealClient._keygen->create_relin_keys(relin_keys);
    Ciphertext x_1;
    sealClient._evaluator->relinearize(_logic_res.smaller, relin_keys, x_1);

    auto [_value1, _value2, _value3] = comparator.compare(_logic_res, A[2], B[2], *sealServer._evaluator);
    auto _n_greater = comparator._or(logic_res.greater,comparator._and(logic_res.equal,logic_res.greater,*sealServer._evaluator),*sealServer._evaluator);
    auto _n_smaller = comparator._or(x_1,comparator._and(logic_res.equal,x_1,*sealServer._evaluator),*sealServer._evaluator);
    ComparatorCarry __logic_res = {_value1, _value2, _value3};

    cout << "A>B:" << sealClient.decrypt_toString(__logic_res.greater) <<\
	" A=B:" << sealClient.decrypt_toString(__logic_res.equal) <<\
	" A<B:" << sealClient.decrypt_toString(__logic_res.smaller) << endl;

    cout << "n_greater: " << sealClient.decrypt_toString(_n_greater) <<\
	" n_smaller: " << sealClient.decrypt_toString(_n_smaller) << "\n" << endl;

    auto [__value1, __value2, __value3] = comparator.compare(__logic_res, A[1], B[1], *sealServer._evaluator);
    auto __n_greater = comparator._or(logic_res.greater,comparator._and(logic_res.equal,logic_res.greater,*sealServer._evaluator),*sealServer._evaluator);
    auto __n_smaller = comparator._or(logic_res.smaller,comparator._and(logic_res.equal,logic_res.smaller,*sealServer._evaluator),*sealServer._evaluator);
    ComparatorCarry ___logic_res = {__value1, __value2, __value3};
    
	cout << "A>B:" << sealClient.decrypt_toString(___logic_res.greater) <<\
	" A=B:" << sealClient.decrypt_toString(___logic_res.equal) <<\
	" A<B:" << sealClient.decrypt_toString(___logic_res.smaller) << endl;

    cout << "n_greater: " << sealClient.decrypt_toString(__n_greater) <<\
	" n_smaller: " << sealClient.decrypt_toString(__n_smaller) << "\n" << endl;

    auto [___value1, ___value2, ___value3] = comparator.compare(___logic_res, A[0], B[0], *sealServer._evaluator);
    auto ___n_greater = comparator._or(logic_res.greater,comparator._and(logic_res.equal,logic_res.greater,*sealServer._evaluator),*sealServer._evaluator);
    auto ___n_smaller = comparator._or(logic_res.smaller,comparator._and(logic_res.equal,logic_res.smaller,*sealServer._evaluator),*sealServer._evaluator);
    ComparatorCarry ____logic_res = {___value1, ___value2, ___value3};
    
	cout << "A>B:" << sealClient.decrypt_toString(____logic_res.greater) <<\
	" A=B:" << sealClient.decrypt_toString(____logic_res.equal) <<\
	" A<B:" << sealClient.decrypt_toString(____logic_res.smaller) << endl;

    cout << "n_greater: " << sealClient.decrypt_toString(___n_greater) <<\
	" n_smaller: " << sealClient.decrypt_toString(___n_smaller) << "\n" << endl;
    */
    // cout << "    + noise budget is: " << sealClient._decryptor->invariant_noise_budget(_logic_res.smaller) << " bits" << sealClient.decrypt_toString(_logic_res.smaller)<< endl;
    

}

