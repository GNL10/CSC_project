#include "comparator.h"
#include "data_base.h"
#include "test.h"

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


    DataBase database("db_test");

    database.createTable("Steve", "MyTable", {"altura", "comprimento", "lol"});

    list<string> col;
    database.search_all_columns("MyTable", col);

    for (auto el : col) {
        cout << el << endl;
    }
    
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


