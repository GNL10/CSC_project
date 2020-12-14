#include "FHE.h"
#include <list>
#include <iterator>

char const *fhe_out_fname  = "fhe_to_client.txt";
char const *fhe_in_fname  = "fhe_to_server.txt"; //"fhe_from_server.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

char const *cmd_out_fname = "answer_to_client.txt";
char const *cmd_in_fname = "cmds_out_enc.txt"; //"cmds_in_enc.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

char const *int_placeholder = "_int_";


static const string CREATE_TABLE = "CREATE TABLE ";
static const string INSERT_INTO = "INSERT INTO ";
static const string SELECT = "SELECT ";
static const string LINE = "LINE ";
static const string DELETE = "DELETE ";
static const string FROM = "FROM ";
static const string WHERE = "WHERE ";
static const string SUM = "SUM(";
static const string SPACE = " ";


void delete_char_in_str (string &str, char c);
void read_command (SEALContext context, ifstream &cmd_file_in, ifstream &fhe_file_in);

class Table {
    public:
        string owner;
        string tablename;
        list <string> col_names;
        int col_num;
        list <list<int>> rows; //change int to ciphertext struct
        Table (string _owner, string _tablename, list <string>& _col_names);
        bool insert_row (list<int> &row);
    };

    Table::Table (string _owner, string _tablename, list <string>& _col_names) {
        owner = _owner;
        tablename = _tablename;
        col_names = list(_col_names);
        col_num = col_names.size(); // number of columns in the list
    }

    bool Table::insert_row (list<int> &row) {
        //cout << "read row: " << row << endl;
        if (row.size() != col_num) // all rows must have the same number of rows
            return false; // insertion was not successful
        rows.push_back(row);
        return true; // insertion was successful
}

int main () {
    bool flag = true; // set to false to close client

    SEALContext context = create_context();

    // files to write to
    ofstream fhe_file_out, cmd_file_out;
    fhe_file_out.open(fhe_out_fname, ios::binary);
    cmd_file_out.open(cmd_out_fname, ios::binary);

    // files to read from
    ifstream fhe_file_in, cmd_file_in;
    fhe_file_in.open(fhe_in_fname, ios::binary);
    cmd_file_in.open(cmd_in_fname, ios::binary);


    /*
    list <string> aux = {"col1", "col2", "col3", "col4"};

    Table x("Joao", "Tabela do joao", aux);
    std::list<string>::iterator it = x.col_names.begin();

    while(it != x.col_names.end())
    {
        std::cout<<(*it)<<"  ";
        it++;
    }
    std::cout<<std::endl;

    for(int z=0 ; z < 5; z++) {
        list<int> i;
        i.push_back(z+1);
        i.push_back(z+2);
        i.push_back(z+3);
        i.push_back(z+4);
        x.insert_row(i);
    }

    for (list<list<int>>::iterator it = x.rows.begin(); it != x.rows.end(); it++){
        for (list<int>::iterator number = (*it).begin(); number != (*it).end(); number++)
            cout << *number << " ";
        cout << endl;
    }*/

    //read_command(context, cmd_file_in, fhe_file_in);


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

bool find_and_del_in_str (string &str, string str_to_find) {
    size_t pos = 0;
    //if (flag != 1 || flag != 0)
    //    throw "find_and_del_in_str flag not valid!";
    if ((pos = str.find(str_to_find)) != std::string::npos) {
        str = str.substr(pos + str_to_find.size(), str.size());
        return true;
    }
    return false;
}

void read_command (SEALContext context, ifstream &cmd_file_in, ifstream &fhe_file_in) {
    string line;
    size_t pos = 0;
    int val;
    // CREATE TABLE tablename (col1name, col2name, … , colNname)
    // INSERT INTO tablename VALUES (value1, .., valueN)
    // SELECT LINE linenum FROM tablename
    // DELETE linenum FROM tablename
    // SELECT col1name, .., colNname FROM tablename WHERE col1name =|<|>value1 AND|OR col2name =|<|> value2
    // SELECT SUM(colname) FROM tablename WHERE col1name =|<|> value AND|OR col2name =|<|> value


    while ( getline (cmd_file_in,line) ) { //run through all lines of file
        if (DEBUG)
            cout << "[DEBUG] Command read from file: " << line << endl;

        std::string token;
        size_t pos = 0;
        string tablename;

        if (find_and_del_in_str(line, CREATE_TABLE)) {
            if ((pos = line.find(SPACE)) != std::string::npos) {
                tablename = line.substr(0, pos);
                find_and_del_in_str(line, tablename + SPACE);

                line.erase(remove(line.begin(), line.end(), ' '), line.end()); // erase all spaces from string
                std::cout << "args: " << line << std::endl;
                if (find_and_del_in_str(line, "(")) {
                    cout << "after deleting (: " << line << endl;
                    do {
                        //string token =
                        cout << "after reading ,: " << line << endl;
                    } while (find_and_del_in_str(line, ","));
                }
                else{
                    cout << "Failed" << endl;
                    //failed
                }

            }
            cout << "READ: " << CREATE_TABLE << endl;
        }
        else if (find_and_del_in_str(line, INSERT_INTO)) {
            cout << "READ : " << INSERT_INTO << endl;
        }
        else if (find_and_del_in_str(line, SELECT)) {
            cout << "READ : " << SELECT << endl;
        }
        else if (find_and_del_in_str(line, DELETE)) {
            cout << "READ : " << DELETE << endl;
        }
        else {
            cout << "Command:" << line << " not recognized." << endl;
        }
        /*
        if ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(pos + delimiter.size(), line.size());
            std::cout << "TOKEN: " << token << std::endl;
            delimiter = "TABLE ";
            if ((pos = token.find(delimiter)) != std::string::npos){
                token = token.substr(pos + delimiter.size(), token.size());
                std::cout << "TOKEN: " << token << std::endl;
            }

        }*/

        while ((pos = line.find(int_placeholder)) != string::npos ) { // for each placeholder decrypt int
            cout << "Loaded a ciphertext in pos : " << pos  << endl;
            line.replace(pos, strlen(int_placeholder), " ");
            //val = decrypt_value(context, secret_key, fhe_file_in);
            //line.replace(pos, strlen(int_placeholder), to_string(val));
        }



    }
}
