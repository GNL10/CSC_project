#include "seal_wrapper.h"

int main(){
    SealWrapperClient sealClient((size_t)4096, 1024);
    SealWrapperServer sealServer(sealClient.getParams(), sealClient.getPublicKey());
    Comparator comparator;


    /////////////////////////////////////////////////////////////////////////////////////
    // Client Side
    int A = 1;
    int B = 1;

    Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);

    Ciphertext value_x_encrypted = sealClient.encrypt(A);
    Ciphertext value_y_encrypted = sealClient.encrypt(B);


    // Server Side
    auto [ logic_res1, logic_res2, logic_res3 ] = comparator.compare(\
            tuple<Ciphertext, Ciphertext, Ciphertext>{x_0_encrypted, x_0_encrypted, x_1_encrypted},\
            value_x_encrypted, value_y_encrypted, *sealServer._evaluator);
    
    // Client Side
    cout << "A>B:" << sealClient.decrypt_toString(logic_res1) <<\
            " A=B:" << sealClient.decrypt_toString(logic_res2) <<\
            " A<B:" << sealClient.decrypt_toString(logic_res3) << endl;

    return 0;
}