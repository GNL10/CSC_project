#include "seal_client_interface.h"
#include "seal_server_interface.h"
#include "comparator.h"
#include "table.h"
#include "read_cmd_server.h"

int main(){
    SealWrapperClient sealClient((size_t)32768, 881); // TODELETE
    SealWrapperServer sealServer(POLY, COEFF, sealClient._relin_key);
    Comparator comparator(sealServer._evaluator, sealServer._relin_keys);

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
    return 0;

}

void read_command (ifstream &cmd_file_in, ifstream &fhe_file_in) {
    string line;
    size_t pos;

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
                line.erase(remove(line.begin(), line.end(), ')'), line.end());
                line.erase(remove(line.begin(), line.end(), '('), line.end());
                list<string> col_list = read_within_commas(line);
                // CALL FUNCTION HERE
            }
        }
        // INSERT INTO tablename VALUES (value1, .., valueN)
        else if (find_and_del_in_str(line, INSERT_INTO)) {
            if ((pos = line.find(SPACE)) != std::string::npos) {
                tablename = line.substr(0, pos);
                find_and_del_in_str(line, tablename + SPACE);
                if (find_and_del_in_str(line, VALUES)) {
                    line.erase(remove(line.begin(), line.end(), ')'), line.end());
                    line.erase(remove(line.begin(), line.end(), '('), line.end());
                    list<string> arg_list = read_within_commas(line);
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
            // SELECT SUM(colname) FROM tablename WHERE col1name = _int_ OR col2name = value
            else if (find_and_del_in_str(line, SUM)){
                string sum;
                if ((pos = line.find(')')) != std::string::npos){
                    sum = line.substr(0, pos);
                    tablename = get_tablename(line);
                    if (find_and_del_in_str(line, WHERE)) { // there are conditions to read
                        list<CondInfo> conditions;
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
                        list<CondInfo> conditions;
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
