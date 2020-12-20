#pragma once

#ifndef TABLE_H
#define TABLE_H

#include "server.h"
#include "seal_server_interface.h"

typedef struct _TableElement{
    Ciphertext full_num;
    vector<Ciphertext> bits;
}TableElement;

class Table {
    public:
        string owner;
        string tablename;
        list <string> col_names;
        int col_num;
        list <list<TableElement>> rows; //change int to ciphertext struct

        Table (string _owner, string _tablename, list <string>& _col_names) {
            owner = _owner;
            tablename = _tablename;
            col_names = list(_col_names);
            col_num = col_names.size(); // number of columns in the list
        }

        bool insert_row (list<TableElement> &row) {
            //cout << "read row: " << row << endl;
            if (row.size() != col_num) // all rows must have the same number of rows
                return false; // insertion was not successful
            rows.push_back(row);
            return true; // insertion was successful
        }

        static Table * search_tablename(string tn, list<Table>* db){
            for (list<Table>::iterator i = db->begin(); i != db->end(); ++i){
                if(i->tablename.compare(tn) == 0)
                    return &*i;
            }
            return NULL;
        }

        static void create_table(list<Table>* db, string _owner, string _tablename, list <string>& _col_names){
            if(Table::search_tablename(_tablename, db) == NULL){ // no element was found
                Table t(_owner, _tablename, _col_names);
                db->push_back(t);
                if(DEBUG) cout << "[DEBUG] Inserted successfully table : " << _tablename << endl;
                return;
            }
            if(DEBUG) cout << "[DEBUG] Failed insertion of table : " << _tablename << endl;
        }

        static void insert_into_table(list<Table>* db, string _tablename, list <string>& arg_list, ifstream &fhe_to_server, 
                                        ofstream &cmd_to_cli, SealWrapperServer &sealServer) {
            Table *table;
            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                // if no element was found
                cmd_to_cli << "INSERT INTO TABLE: Table does not exist!" << endl;
                if(DEBUG) cout << "[DEBUG] Failed Insert into table : " << _tablename << endl;
                return;
            }
            list<TableElement> row;
            if(DEBUG) cout << "[DEBUG] Inserting into table." << endl;
            
            for (list<string>::iterator it = arg_list.begin(); it != arg_list.end(); it++){
                if (it->compare(int_placeholder) != 0 )
                    throw "ERROR: insert_into_table argument : placeholder does not match.";
                TableElement aux;
                aux.full_num = sealServer.load_ciphertext(fhe_to_server); // load full number
                
                for(int x=0; x < NUM_MAX_BITS; x++){
                    aux.bits.push_back(sealServer.load_ciphertext(fhe_to_server));
                }
                row.push_back(aux);
            }

            table->insert_row(row);
            cmd_to_cli << "INSERT INTO TABLE: OK!" << endl;
        }

        static void select_line(list<Table>* db, string _tablename, int linenum, ofstream &cmd_to_cli, ofstream &fhe_to_cli) {
            Table *table;
            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                // if no element was found
                cmd_to_cli << "SELECT LINE : Table does not exist!" << endl;
                if(DEBUG) cout << "[DEBUG] Failed select line from : " << _tablename << endl;
                return;
            }        

            int l = 0;
            for (list<list<TableElement>>::iterator row = table->rows.begin(); row != table->rows.end(); row++, l++){
                if(l == linenum){
                    cmd_to_cli << "SELECT LINE ANSWER : ";
                    for(list<TableElement>::iterator element = row->begin(); element != row->end(); element++){
                        element->full_num.save(fhe_to_cli);
                        cmd_to_cli << int_placeholder << ", ";
                    }
                    cmd_to_cli << endl;
                    return;
                }
            }    
            cmd_to_cli << "SELECT LINE : Line does not exist!" << endl; 
        }

        static void select_sum_with_conditions(list<Table>* db, string _tablename, string col_to_sum, list<CondInfo> &conds){
            //gil
        }

        static void select_sum_all (list<Table>* db, string _tablename, string col_to_sum){
        }

        static void select_colnames_with_conditions (list<Table>* db, string _tablename, list<string> col_list, list<CondInfo> &conds){
            //gil
        }
        
        static void select_colnames_all (list<Table>* db, string _tablename, list<string> col_list) {

        }

        static void delete_line (list<Table>* db, string _tablename, int linenum, ofstream &cmd_to_cli) {
            Table *table;
            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                cmd_to_cli << "DELETE LINE : Table does not exist!" << endl;
                if(DEBUG) cout << "[DEBUG] Failed select line from : " << _tablename << endl;
                return;
            }

            int l = 0;
             for (list<list<TableElement>>::iterator row = table->rows.begin(); row != table->rows.end(); row++, l++){
                if(l == linenum){
                    table->rows.erase(row);
                    if(DEBUG) cout << "[DEBUG] DELETED ROW FROM : " << _tablename << endl;
                    cmd_to_cli << "DELETE LINE : OK!" << endl;
                    return;
                }
             }
             cmd_to_cli << "DELETE LINE : Row does not exist!" << endl;
             if(DEBUG) cout << "[DEBUG] FAILED TO DELETE ROW FROM : " << _tablename << endl;
        }
};

list<Table> db;

#endif
