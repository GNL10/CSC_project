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
        
};

#endif
