#include "utils.h"

int main(){

    EncryptionParameters parms(scheme_type::bfv);

	size_t poly_modulus_degree = 32768;
    parms.set_poly_modulus_degree(poly_modulus_degree);

	parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));

	parms.set_plain_modulus(881);

	SEALContext context(parms);

	KeyGenerator keygen(context);
    SecretKey secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);

    RelinKeys relin_keys;

    keygen.create_relin_keys(relin_keys);

	Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);

    Comparator comparator(&evaluator, &relin_keys);
    database.createTable("Steve", "MyTable", {"col1", "col2", "col3", "col4"});


    Plaintext _0_plain(to_string(0));
    Plaintext _1_plain(to_string(1));

    Plaintext _3_plain(to_string(3));

    Ciphertext _0_encrypted;
    Ciphertext _1_encrypted;

    encryptor.encrypt(_0_plain, _0_encrypted);
    encryptor.encrypt(_1_plain, _1_encrypted);

    vector<Ciphertext> a = {_1_encrypted};
    vector<Ciphertext> b = {_0_encrypted};

    TableElement* _A = new TableElement();
    Comparator::insert_bits(a, _A->elem_bits);
    
    encryptor.encrypt(_3_plain, _A->elem_value);
    
    TableElement* _B = new TableElement();
    Comparator::insert_bits(b, _B->elem_bits);
    
    TableElement* _C = new TableElement();
    Comparator::insert_bits(a, _C->elem_bits);

    TableElement* _D = new TableElement();
    Comparator::insert_bits(b, _D->elem_bits);

    database.insert_elem_in_column("MyTable", "col1", _A);
    database.insert_elem_in_column("MyTable", "col1", _B);
    database.insert_elem_in_column("MyTable", "col1", _C);
    database.insert_elem_in_column("MyTable", "col1", _D);

    for (auto el : _A->elem_bits){
        Plaintext decrypted;
        decryptor.decrypt(el, decrypted);
        cout << decrypted.to_string() << endl;
    }

    Utils utils;

    CondInfo condition;

    condition.colname = "col1";
    condition.op = 0;
    condition.logical_op = -1;
    Comparator::insert_bits(b, condition.bits_num);
    
    list<CondInfo> conds;
    conds.push_back(condition);
    

    Plaintext cipher_result_decrypted;

    decryptor.decrypt( utils.selected_sum_with_conditions(&comparator, "MyTable", "col1", conds) , cipher_result_decrypted);

    cout << "Result: " << cipher_result_decrypted.to_string() << endl;
}




// Test Comparator
/*

    Comparator comparator(&evaluator, &relin_keys);

    cout << ":: FUNCIONA ::" << endl;

    Plaintext _0_plain(to_string(0));
    Plaintext _1_plain(to_string(1));

    Ciphertext _0_encrypted;
    Ciphertext _1_encrypted;

    encryptor.encrypt(_0_plain, _0_encrypted);
    encryptor.encrypt(_1_plain, _1_encrypted);

    vector<Ciphertext> A = {_0_encrypted, _1_encrypted, _1_encrypted, _0_encrypted};
    vector<Ciphertext> B = {_0_encrypted, _1_encrypted, _1_encrypted, _0_encrypted};

    auto res = comparator.compareNBits(A, B);

    Plaintext grater_decrypted;
    Plaintext equal_decrypted;
    Plaintext smaller_decrypted;

    decryptor.decrypt(get<0>(res), grater_decrypted);
    decryptor.decrypt(get<1>(res), equal_decrypted);
    decryptor.decrypt(get<2>(res), smaller_decrypted);

    cout << "A > B: " << grater_decrypted.to_string() <<\
            " A = B: " << equal_decrypted.to_string() <<\
            " A < B: " << smaller_decrypted.to_string() << endl;
*/


