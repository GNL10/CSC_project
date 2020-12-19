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

        static void insert_into_table(list<Table>* db, string _tablename, list <string>& arg_list, ifstream &fhe_to_server, SealWrapperServer &sealServer) {
            Table *table;
            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                // if no element was found
                if(DEBUG) cout << "[DEBUG] Failed Insert into table : " << _tablename << endl;
                return;
            }
            list<TableElement> row;

            for (list<string>::iterator it = arg_list.begin(); it != arg_list.end(); it++){
                if (it->compare(int_placeholder) != 0 )
                    throw "ERROR: insert_into_table argument : placeholder does not match.";
                TableElement aux;
                aux.full_num = sealServer.load_ciphertext(fhe_to_server); // load full number
                for(int x=0; x < NUM_MAX_BITS; x++)
                    aux.bits.push_back(sealServer.load_ciphertext(fhe_to_server));
                row.push_back(aux);
            }

            table->insert_row(row);
        }

        static void select_line(list<Table>* db, string _tablename, int linenum) {
            Table *table;
            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                // if no element was found
                if(DEBUG) cout << "[DEBUG] Failed select line from : " << _tablename << endl;
                return;
            }            
        }

        static void select_sum_with_conditions(list<Table>* db, string _tablename, string col_to_sum, list<CondInfo> &conds){

        }

        static void select_sum_all (list<Table>* db, string _tablename, string col_to_sum){
        }

        static void select_colnames_with_conditions (list<Table>* db, string _tablename, list<string> col_list, list<CondInfo> &conds){

        }
        
        static void select_colnames_all (list<Table>* db, string _tablename, list<string> col_list) {

        }

        static void delete_line (list<Table>* db, string _tablename, int linenum) {

        }
};

list<Table> db;

#endif
