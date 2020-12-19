#ifndef PARSE_CMD_H
#define PARSE_CMD_H

class ClientParseCmd{

    public:
    
        void delete_char_in_str (string &str, char c) {
            size_t pos = str.find(c);
            if (pos != std::string::npos)
                str.replace(pos, 1, "");
        }

        void send_command (SealWrapperClient *sealClient, ofstream &cmd_file_out, ofstream &fhe_file_out) {
            string str;
            stringstream str_strm;
            int temp_int;
            string temp_str;
            size_t pos = 0;

            cout << "Enter command:" << endl;
            getline(cin, str);
            std::for_each(str.begin(), str.end(), [](char & c) {c = ::toupper(c);}); // converts the whole string to upper case
            // ints in commands that specify a specific line, should not be homomorphically encrypted
            if (str.find(SELECT + LINE) == string::npos && str.find(DELETE + LINE) == string::npos) {
                str_strm << str; //convert the string s into stringstream
                while(!str_strm.eof()) {
                    str_strm >> temp_str; //take words into temp_str one by one
                    delete_char_in_str(temp_str, '(');
                    delete_char_in_str(temp_str, ',');
                    delete_char_in_str(temp_str, ')');

                    if(stringstream(temp_str) >> temp_int) { //try to convert string to int
                        sealClient->encrypt_and_save(temp_int,fhe_file_out);
                        pos = str.find(to_string(temp_int));
                        str.replace(pos, to_string(temp_int).length(), int_placeholder);
                    }
                    temp_str = ""; //clear temp string
                }
            }
            cmd_file_out << str << endl; // write to file
            if (DEBUG)
                cout << "[DEBUG] Command after removing ints: " << str << endl;
        }

        void read_command (SealWrapperClient *sealClient, ifstream &cmd_file_in, ifstream &fhe_file_in) {
            string line;
            size_t pos = 0;
            int val;

            while ( getline (cmd_file_in,line) ) { //run through all lines of file
                if (DEBUG)
                    cout << "[DEBUG] Command before changing int placeholders: " << line << endl;

                while ((pos = line.find(int_placeholder)) != string::npos ) { // for each placeholder decrypt int
                    val = sealClient->decrypt_from_file(fhe_file_in);
                    line.replace(pos, strlen(int_placeholder), to_string(val));
                }
                cout << line << endl;
            }
        }

};


#endif