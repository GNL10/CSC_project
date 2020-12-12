#include "seal_wrapper.h"

/*
void saveCiphertext(Ciphertext encrypted, string filename){
  ofstream ct;
  ct.open(filename, ios::binary);
  encrypted.save(ct);
};

Ciphertext loadCiphertext(string filename, SEALContext ctx){

  ifstream ct;
  ct.open(filename, ios::binary);
  Ciphertext result;
  result.load(ctx, ct);

  return result;
};
*/

int main(){
    SealWrapperClient sealClient((size_t)8192, 218);
    SealWrapperServer sealServer(sealClient.getParams(), sealClient.getPublicKey());
    Comparator comparator;

    /////////////////////////////////////////////////////////////////////////////////////
    // Client Side
    
	Ciphertext x_0_encrypted = sealClient.encrypt(0);
	Ciphertext x_1_encrypted = sealClient.encrypt(1);

	vector<Ciphertext> A = {x_0_encrypted, x_0_encrypted, x_1_encrypted};
	vector<Ciphertext> B = {x_1_encrypted, x_0_encrypted, x_0_encrypted};

    int num_bits = A.size();
    cout << num_bits << endl;

	auto _prev_logic_res = make_tuple(x_0_encrypted, x_1_encrypted, x_0_encrypted);
    tuple<Ciphertext, Ciphertext, Ciphertext> _next_logic_res;

    // Server Side
    
    Plaintext r;
    Plaintext r1;
    Plaintext r2;

    Ciphertext d;
    Ciphertext d1;
    Ciphertext d2;

    for(int i = num_bits-1; i>=0; i--){
        _next_logic_res = comparator.compare(_prev_logic_res, A[i], B[i], *sealServer._evaluator);

        r = sealClient.decrypt(get<0>(_next_logic_res));
        r1 = sealClient.decrypt(get<1>(_next_logic_res));
        r2 = sealClient.decrypt(get<2>(_next_logic_res));

        d = sealClient.encrypt_fromPlaintext(r);
        d1 = sealClient.encrypt_fromPlaintext(r1);
        d2 = sealClient.encrypt_fromPlaintext(r2);

        _prev_logic_res = make_tuple(d, d1, d2);
    }
    
    // Client Side
    cout << "A>B:" << sealClient.decrypt_toString(get<0>(_next_logic_res)) <<\
            " A=B:" << sealClient.decrypt_toString(get<1>(_next_logic_res)) <<\
            " A<B:" << sealClient.decrypt_toString(get<2>(_next_logic_res)) << endl;

    return 0;
}