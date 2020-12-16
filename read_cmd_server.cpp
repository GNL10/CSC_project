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
static const string VALUES = "VALUES ";
static const string SELECT = "SELECT ";
static const string LINE = "LINE ";
static const string DELETE = "DELETE ";
static const string FROM = "FROM ";
static const string WHERE = "WHERE ";
static const string SUM = "SUM(";
static const string SPACE = " ";
static const string AND = " AND ";
static const string OR = " OR ";
static const string BIGGER = ">";
static const string SMALLER = "<";
static const string EQUAL = "=";

struct cond_info {
    string colname;
    int op; // -1 for <    0 for =    1 for >
    int logical_op; // -1 for OR   1 for AND
    Ciphertext full_num;
    list <Ciphertext> bits_num;
};


void delete_char_in_str (string &str, char c);
void read_command (ifstream &cmd_file_in, ifstream &fhe_file_in);


class Table {
    public:
        string owner;
        string tablename;
        list <string> col_names;
        int col_num;
        list <list<int>> rows; //change int to Ciphertext struct
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

    read_command(cmd_file_in, fhe_file_in);


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

list <string> read_within_commas (string str) {
    list<string> info;
    size_t pos = 0;

    str.erase(remove(str.begin(), str.end(), ' '), str.end());// erase all spaces from string
    while ((pos = str.find(',')) != std::string::npos) {
        info.push_back(str.substr(0 ,pos)); // push column name in list
        str = str.substr(pos+1, str.size()); // move to next colname
    }
    info.push_back(str); // after while runs, last argument is last col
    return info;
}

list <string> read_within_parenthisis (string str) {
    list<string> info;
    string token;
    size_t pos = 0;
    str.erase(remove(str.begin(), str.end(), ' '), str.end()); // erase all spaces from string
    if (find_and_del_in_str(str, "(")) {
        do {
            if ((pos = str.find(',')) != std::string::npos){
                token = str.substr(0, pos);
                info.push_back(token);
            }
            else if ((pos = str.find(')')) != std::string::npos){
                token = str.substr(0, pos);
                info.push_back(token);
                break;
            }
        } while (find_and_del_in_str(str, ","));
    }
    else{
        throw "FAILED: in create table no '(' was found";
    }
    return info;
}

string get_tablename (string &line) {
    size_t pos = 0;
    string tablename;
    if (find_and_del_in_str(line, FROM)) { // extracting tablename
        if ((pos = line.find(SPACE)) != std::string::npos) {
            tablename = line.substr(0, pos);
            find_and_del_in_str(line, tablename + SPACE);
        }
        return tablename;
    }
    return "ERROR";
}

struct cond_info get_one_condition (string str) {
    size_t pos = 0, pos_and = 0, pos_or = 0;
    string curr_cond;
    struct cond_info condition;

    //analysing condition
    pos_and = str.find(AND);
    pos_or = str.find(OR);

    if (pos_and == string::npos && pos_or == string::npos) //if there are no ands or ors
        curr_cond = str;
    else if (pos_and > pos_or) // read an OR first
        curr_cond = str.substr(0, pos_or);
    else // read an AND first
        curr_cond = str.substr(0, pos_and);

    curr_cond.erase(remove(curr_cond.begin(), curr_cond.end(),' '), curr_cond.end()); // erase all spaces from string

    cout << "curr_cond : " << curr_cond << endl;

    if ((pos = curr_cond.find(BIGGER)) != std::string::npos) {
        condition.colname = str.substr(0, pos);
        condition.op = 1;
    }
    else if ((pos = curr_cond.find(SMALLER)) != std::string::npos) {
        condition.colname = str.substr(0, pos);
        condition.op = -1;
    }
    else if ((pos = curr_cond.find(EQUAL)) != std::string::npos) {
        condition.colname = str.substr(0, pos);
        condition.op = 0;
    }
    else
        throw "ERROR: get_one_condition was unable to identify >, < or =";
    cout << "Colname : " << condition.colname << " condition : " << condition.op << endl;
    return condition;
}

void parse_conditions (string line, list<cond_info> &conditions) {
    size_t pos = 0;
    string curr_cond;
    struct cond_info cond;

    cout << "Parse conditions : " << line << endl;
    //analysing 1st condition
    conditions.push_back(get_one_condition(line));
    while (line.find(AND) != std::string::npos || line.find(OR) != std::string::npos) {
        if (find_and_del_in_str(line, AND)) {
            cout << "Found AND condition" << endl;
            cond = get_one_condition(line);
            cond.logical_op = 1;
            conditions.push_back(cond);
        }
        else if (find_and_del_in_str(line, OR)) {
            cout << "Found OR condition" << endl;
            cond = get_one_condition(line);
            cond.logical_op = -1;
            conditions.push_back(cond);
        }
    }
}

void read_command (ifstream &cmd_file_in, ifstream &fhe_file_in) {
    string line;
    size_t pos = 0;
    int val;
    // CREATE TABLE tablename (col1name, col2name, … , colNname)
    // INSERT INTO tablename VALUES (value1, .., valueN)
    // DELETE linenum FROM tablename
    // SELECT LINE linenum FROM tablename
    // SELECT col1name, .., colNname FROM tablename WHERE col1name =|<|>value1 AND|OR col2name =|<|> value2
    // SELECT SUM(colname) FROM tablename WHERE col1name =|<|> value AND|OR col2name =|<|> value


    while ( getline (cmd_file_in,line)) { //run through all lines of file
        if (DEBUG)
            cout << "[DEBUG] Command read from file: " << line << endl;

        string tablename;
        int linenum;

        // CREATE TABLE tablename (col1name, col2name, … , colNname)
        if (find_and_del_in_str(line, CREATE_TABLE)) {
            if ((pos = line.find(SPACE)) != std::string::npos) {
                tablename = line.substr(0, pos);
                find_and_del_in_str(line, tablename + SPACE);
                list<string> col_list = read_within_parenthisis (line); // list with column names
                // CALL FUNCTION HERE
            }
        }
        // INSERT INTO tablename VALUES (value1, .., valueN)
        else if (find_and_del_in_str(line, INSERT_INTO)) {
            if ((pos = line.find(SPACE)) != std::string::npos) {
                tablename = line.substr(0, pos);
                find_and_del_in_str(line, tablename + SPACE);
                if (find_and_del_in_str(line, VALUES)) {
                    list<string> arg_list = read_within_parenthisis (line); // list with arguments in parenthisis
                    // CALL FUNCTION HERE
                }
            }
        }
        // SELECT CONDITIONS
        else if (find_and_del_in_str(line, SELECT)) {
            // SELECT LINE linenum FROM tablename
            if (find_and_del_in_str(line, LINE)) {
                if ((pos = line.find(SPACE)) != std::string::npos){
                    linenum = stoi(line.substr(0, pos));
                    if (find_and_del_in_str(line, FROM)) { // extracting tablename
                        tablename = line;
                        // CALL FUNCTION HERE
                    }

                }
            }
            // SELECT SUM(colname) FROM tablename WHERE col1name =|<|> value AND|OR col2name =|<|> value
            else if (find_and_del_in_str(line, SUM)){
                string sum;
                if ((pos = line.find(')')) != std::string::npos){
                    sum = line.substr(0, pos);
                    tablename = get_tablename(line);
                    if (find_and_del_in_str(line, WHERE)) { // there are conditions to read
                        list<cond_info> conditions;
                        parse_conditions(line, conditions);
                        // CALL FUNCTION HERE
                    }
                    else { // there are no condit
                        // sum every line
                        cout << "No conditions " << endl;
                        // CALL FUNCTION HERE
                    }
                }

            }
            // SELECT col1name, .., colNname FROM tablename WHERE col1name =|<|>value1 AND|OR col2name =|<|> value2
            else{
                // get columns
                if ((pos = line.find(FROM)) != std::string::npos){
                    list <string> col_list = read_within_commas(line.substr(0, pos));
                    tablename = get_tablename(line);
                    // get condit
                    if (find_and_del_in_str(line, WHERE)) { // there are conditions to read
                        list<cond_info> conditions;
                        parse_conditions(line, conditions);
                        // CALL FUNCTION HERE
                    }
                    else { // there are no condit
                        // sum every line
                        cout << "No conditions " << endl;
                        // CALL FUNCTION HERE
                    }
                }
            }

        }
        else if (find_and_del_in_str(line, DELETE)) {
            if ((pos = line.find(SPACE)) != std::string::npos) {
                linenum = stoi(line.substr(0, pos));
                if (find_and_del_in_str(line, FROM)) {
                    tablename = line;
                    // CALL FUNCTION HERE
                }
            }
        }
        else {
            cout << "Command:" << line << " not recognized." << endl;
        }
    }
}
