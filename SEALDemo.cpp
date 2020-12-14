#include "seal/seal.h"
#include <iostream>
#include <fstream>
#include <bitset>
#include <climits>

using namespace std;
using namespace seal;

stringstream str_to_hex_bin (string str){
    int i = 0;
    stringstream bin;
    while(str[i])
    {
        switch(str[i])
        {
            case '0':
                bin<<"0000";
                break;
            case '1':
                bin<<"0001";
                break;
            case '2':
                bin<<"0010";
                break;
            case '3':
                bin<<"0011";
                break;
            case '4':
                bin<<"0100";
                break;
            case '5':
                bin<<"0101";
                break;
            case '6':
                bin<<"0110";
                break;
            case '7':
                bin<<"0111";
                break;
            case '8':
                bin<<"1000";
                break;
            case '9':
                bin<<"1001";
                break;
            case 'A':
            case 'a':
                bin<<"1010";
                break;
            case 'B':
            case 'b':
                bin<<"1011";
                break;
            case 'C':
            case 'c':
                bin<<"1100";
                break;
            case 'D':
            case 'd':
                bin<<"1101";
                break;
            case 'E':
            case 'e':
                bin<<"1110";
                break;
            case 'F':
            case 'f':
                bin<<"1111";
                break;
            default:
                cout<<"ERROR --Invalid Hex Digit ("<<str[i]<<")--";
        }
        i++;
    }
    return bin;
}

int main() {
    /*
    stringstream hex_num;
    hex_num << "FF";
    stringstream bin = str_to_hex_bin(hex_num.str());
    cout << "HEX: " << hex_num.str() << "\t BIN: " << bin.str() << endl; */

	EncryptionParameters parms(scheme_type::bfv);

	size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);

	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

	parms.set_plain_modulus(1024);

	SEALContext context(parms);

	KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

	Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    //BatchEncoder batch_encoder(context);



	// This x is the value that is going to be encrypted
    ofstream encrypted_file;
    encrypted_file.open("encrypted.txt");
    int N = 20; // N numbers encrypted in file
    for (int i = 1; i < N; i++) {
        int x = i;
        Ciphertext x_encrypted;
        stringstream stream;
        stream << std::hex << x;
        cout << "Int to be encrypted: " << x << endl;
        //cout << "Int to be encrypted in hex: " << stream.str() << endl;
        Plaintext x_plain(stream.str());
        encryptor.encrypt(x_plain, x_encrypted);
        x_encrypted.save(encrypted_file);
        
    }
    encrypted_file.close();

    //IntegerEncoder encoder(context);
    
    /*
    Plaintext plain_matrix;
    print_line(__LINE__);
    cout << "Encode plaintext matrix:" << endl;
    batch_encoder.encode(pod_matrix, plain_matrix);
    */
    /*
    cout << "Express x = " + stream.str() + " as a plaintext polynomial 0x" + x_plain.to_string() + "." << endl;

	Ciphertext x_encrypted;
    cout << "Encrypt x_plain to x_encrypted." << endl;
    encryptor.encrypt(x_plain, x_encrypted);
    
	// saving the encrypted values into the file
    ofstream encrypted_file;
    encrypted_file.open("encrypted.txt");
    x_encrypted.save(encrypted_file);

    ////////////////////////////////////////////////////////
    Ciphertext x_sq_plus_on;
    evaluator.square(x_encrypted, x_sq_plus_on);
    Plaintext plain_on("1");
    evaluator.add_plain_inplace(x_sq_plus_on, plain_on);
    x_sq_plus_on.save(encrypted_file); //TODO testing saving 2x
    ///////////////////////////////////////////////////////
    encrypted_file.close();
    cout << "    + number of bytes of ciphertext: " << x_encrypted.save_size() << endl;
    cout << "    + size of freshly encrypted x: " << x_encrypted.size() << endl;

    cout << "    + noise budget in freshly encrypted x: " << decryptor.invariant_noise_budget(x_encrypted) << " bits" << endl;


	Ciphertext input;
	ifstream input_encrypted_file;
	input_encrypted_file.open("encrypted.txt");
	input.unsafe_load(context, input_encrypted_file);
    */

    ifstream input_encrypted_file;
	input_encrypted_file.open("encrypted.txt");
    for (int i = 1; i< N; i++) {
        Ciphertext input;
        Plaintext x_decrypted;
        input.load(context, input_encrypted_file);
        
        decryptor.decrypt(input, x_decrypted);
        cout << "    + decryption of x_encrypted: ";
        cout << (int)strtol(x_decrypted.to_string().c_str(), NULL, 16) << endl;
    }
    
    /*
    Plaintext x_decrypted;
    cout << "    + decryption of x_encrypted: ";
    decryptor.decrypt(input, x_decrypted);
    cout << (int)strtol(x_decrypted.to_string().c_str(), NULL, 16) << endl;
    */
    /*
    ///////////////////////////////////////////////////
    Ciphertext input_xsq_1;
    Plaintext decrypted_xsq_1;
	input_xsq_1.unsafe_load(context, input_encrypted_file);
    cout << "    + decryption of xsq_1: ";
    decryptor.decrypt(input_xsq_1, decrypted_xsq_1);

    cout << (int)strtol(decrypted_xsq_1.to_string().c_str(), NULL, 16) << " ...... Correct." << endl;
    /////////////////////////////////////////////////////

	input_encrypted_file.close();
    */
	
    /*
    Ciphertext x_sq_plus_one;
    evaluator.square(x_encrypted, x_sq_plus_one);
    Plaintext plain_one("1");
    evaluator.add_plain_inplace(x_sq_plus_one, plain_one);

    Plaintext x_decrypted_aux;
    cout << "    + decryption of x_encrypted: ";
    decryptor.decrypt(x_sq_plus_one, x_decrypted_aux);

    cout << (int)strtol(x_decrypted_aux.to_string().c_str(), NULL, 16) << " ...... Correct." << endl;
    cout << "    + noise budget in freshly encrypted x: " << decryptor.invariant_noise_budget(x_sq_plus_one) << " bits" << endl;
    */
    



    /*
    Plaintext plain_result;
    print_line(__LINE__);
    cout << "Decrypt and decode result." << endl;
    decryptor.decrypt(encrypted_matrix, plain_result);
    batch_encoder.decode(plain_result, pod_result);
    */


    // hex to int (c_str : string to const string)
    //int decrypted_int = (int)strtol(x_decrypted.to_string().c_str(), NULL, 16);
	//cout << "Decrypted value : " << decrypted_int << endl;

	return 0;
}
