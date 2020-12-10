#include "FHE.h"

char const *PK_fname = "public_key.txt";
char const *SK_fname = "secret_key.txt";
char const *input_fname  = "fhe_encrypted.txt";

int main () {

    bool flag = true; // set to false to close client
    
    SEALContext context = create_context();
    PublicKey public_key = load_PK_from_file(context, PK_fname);
    SecretKey secret_key = load_SK_from_file(context, SK_fname);

    //open 
    ofstream encrypted_file_out;
    encrypted_file_out.open(input_fname, ios::binary);

    ifstream encrypted_file_in;
	encrypted_file_in.open(input_fname);


    while (flag) {
        string input;

        cout << "\n\nPlease enter an integer value: " << endl;
        cout << "\nThis is the Options Menu:\n" << endl;
        cout << "- PRESS 1 to create a security key;" << endl;
        cout << "- PRESS 2 to send a command;" << endl;
        cout << "- PRESS 3 to read responses." << endl;
        cout << "- PRESS 4 to quit." << endl;

        cin >> input;
        int i;
        std::istringstream(input) >> i;
        
        switch (i) {
            case 1:
                gen_keys(context, SK_fname, PK_fname);
                break;
            case 2:
                encrypt_value(context, 3, public_key, encrypted_file_out);
                break;
            case 3:
                cout << "Decrypted value : " << decrypt_value(context, secret_key, encrypted_file_in);
                break;
            case 4:
                flag = false;
                break;
            default:
                cout << "ERROR: Input value not valid!";
            
        }
    }

    encrypted_file_out.close();
    encrypted_file_in.close();
}