#pragma once

#ifndef TABLE_H
#define TABLE_H

#include "server.h"
typedef struct _TableElement{
    Ciphertext elem;
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

        static bool search_tablename(string tn, list<Table>* db){
            for (list<Table>::const_iterator i = db->begin(); i != db->end(); ++i){
                if(DEBUG) cout << "[DEBUG] Search Tablename :: " << i->tablename << endl;
                if(i->tablename.compare(tn) == 0){
                    if(DEBUG) cout << "[DEBUG] Tablename Compare :: Status (Equal)" << endl;
                    return 1;
                }
            }
            return 0;
        }

        static void insert_table_in_list(list<Table>* db, string _owner, string _tablename, list <string>& _col_names){
            if(DEBUG) cout << "[DEBUG] Insert Table In List ::"<< endl;
            if(!Table::search_tablename(_tablename, db)){
                if(DEBUG) cout << "[DEBUG] Table Name :: " << _tablename << endl;
                Table t(_owner, _tablename, _col_names);
                db->push_back(t);
            }
        }
        
};

list<Table> db;

#endif
