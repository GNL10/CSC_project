#include "FHE.h"

char const *PK_fname = "public_key.txt";
char const *SK_fname = "secret_key.txt";

char const *fhe_out_fname  = "fhe_to_server.txt";
char const *fhe_in_fname  = "fhe_to_server.txt"; //"fhe_from_server.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

char const *cmd_out_fname = "cmds_out_enc.txt";
char const *cmd_in_fname = "cmds_out_enc.txt"; //"cmds_in_enc.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

char const *int_placeholder = "_int_";

void delete_char_in_str (string &str, char c);
void send_command (SEALContext context, PublicKey public_key, ofstream &cmd_file_out, ofstream &fhe_file_out);
void read_command (SEALContext context, SecretKey secret_key, ifstream &cmd_file_in, ifstream &fhe_file_in);

int main () {

    bool flag = true; // set to false to close client
    
    SEALContext context = create_context();
    PublicKey public_key = load_PK_from_file(context, PK_fname);
    SecretKey secret_key = load_SK_from_file(context, SK_fname);

    // files to write to
    ofstream fhe_file_out, cmd_file_out;
    fhe_file_out.open(fhe_out_fname, ios::binary);
    cmd_file_out.open(cmd_out_fname, ios::binary);

    // files to read from
    ifstream fhe_file_in, cmd_file_in;
	fhe_file_in.open(fhe_in_fname, ios::binary);
    cmd_file_in.open(cmd_in_fname, ios::binary);

    while (flag) {
        string input;

        cout << "\nThis is the Options Menu:\n" << endl;
        cout << "- PRESS 1 to create a security key;" << endl;
        cout << "- PRESS 2 to send a command;" << endl;
        cout << "- PRESS 3 to read responses." << endl;
        cout << "- PRESS 4 to quit." << endl;

        
        getline(cin, input, '\n');
        
        int i = 0;
        std::istringstream(input) >> i;
        
        switch (i) {
            case 1:
                gen_keys(context, SK_fname, PK_fname);
                break;
            case 2:
                send_command(context, public_key, cmd_file_out, fhe_file_out);                
                break;
            case 3:
                read_command(context, secret_key, cmd_file_in, fhe_file_in);
                break;
            case 4:
                flag = false;
                break;
            default:
                cout << "ERROR: Input value not valid!";
            
        }
    }

    fhe_file_out.close();
    cmd_file_out.close();
    fhe_file_in.close();
    cmd_file_in.close();
}


void delete_char_in_str (string &str, char c) {
    size_t pos = str.find(c);
    if (pos != std::string::npos)
        str.replace(pos, 1, "");
}

void send_command (SEALContext context, PublicKey public_key, ofstream &cmd_file_out, ofstream &fhe_file_out) {
    string str; 
    stringstream str_strm;
    int temp_int;
    string temp_str;
    size_t pos = 0;

    cout << "Enter command:" << endl;
    getline(cin, str, ';');    
    
    
    str_strm << str; //convert the string s into stringstream

    while(!str_strm.eof()) {
        str_strm >> temp_str; //take words into temp_str one by one
        
        delete_char_in_str(temp_str, '(');
        delete_char_in_str(temp_str, ',');
        delete_char_in_str(temp_str, ')');

        if(stringstream(temp_str) >> temp_int) { //try to convert string to int
            encrypt_value(context, temp_int, public_key, fhe_file_out); 
            
            pos = str.find(to_string(temp_int));
            str.replace(pos, to_string(temp_int).length(), int_placeholder);
        }
        temp_str = ""; //clear temp string
    }

    cmd_file_out << str << endl;
    if (DEBUG)
        cout << "[DEBUG] Command after removing ints: " << str << endl;
    getline(cin, str, '\n'); // ignore the trash until the \n
}

void read_command (SEALContext context, SecretKey secret_key, ifstream &cmd_file_in, ifstream &fhe_file_in) {
    string line;
    size_t pos = 0;
    int val;

    while ( getline (cmd_file_in,line) ) { //run through all lines of file
        if (DEBUG)
            cout << "[DEBUG] Command before changing int placeholders: " << line << endl;

        while ((pos = line.find(int_placeholder)) != string::npos ) { // for each placeholder decrypt int
            val = decrypt_value(context, secret_key, fhe_file_in);
            line.replace(pos, strlen(int_placeholder), to_string(val));
        }
        cout << line << endl;
    }
}