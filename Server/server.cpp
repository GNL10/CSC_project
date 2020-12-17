#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"
#include "table.h"

#define NUM_MAX_BITS 64

int* parseInt_toBinary(int value){
    static int a[NUM_MAX_BITS];

    for (int i = 0; value>0; i++){
        a[i] = value%2;
        value = value/2;
    }

    return a;
}

int main(){
	// client side
    SealWrapperClient sealClient((size_t)32768, 881);
    SealWrapperServer sealServer(sealClient._poly_modulus_degree, sealClient._plain_modulus, sealClient._relin_key);
    Comparator comparator(sealServer._evaluator, sealServer._relin_keys);

	list <string> aux = {"col1", "col2", "col3", "col4"};
	Table db("Joao", "Tabela do joao", aux);

    int *binary_result;

    binary_result = parseInt_toBinary(60);
    bool begin = false;
    for(int i=NUM_MAX_BITS-1; i>=0; i--){
        if(*(binary_result+i) != 0) begin = true;
        if(begin) cout << *(binary_result+i) << endl;
    }

    return 0;

}

