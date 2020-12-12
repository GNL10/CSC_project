#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"

int main(){

    SealWrapperClient sealClient((size_t)32768, 881);
    SealWrapperServer sealServer;
    Comparator comparator;

    int A = 1;
    int B = 1;

    Ciphertext x_0_encrypted = sealClient.encrypt(0);
    Ciphertext x_1_encrypted = sealClient.encrypt(1);

    Ciphertext value_x_encrypted = sealClient.encrypt(A);
    Ciphertext value_y_encrypted = sealClient.encrypt(B);

    // Server Side
    auto logic_res = comparator.compare(\
            tuple<Ciphertext, Ciphertext, Ciphertext>{x_0_encrypted, x_0_encrypted, x_1_encrypted},\
            value_x_encrypted, value_y_encrypted, *sealServer._evaluator);
    
    // Client Side
    cout << "A>B:" << sealClient.decrypt_toString(get<0>(logic_res)) <<\
            " A=B:" << sealClient.decrypt_toString(get<1>(logic_res)) <<\
            " A<B:" << sealClient.decrypt_toString(get<2>(logic_res)) << endl;

    return 0;

}

