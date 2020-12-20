#pragma once

#ifndef DATA_BASE_H
#define DATA_BASE_H

#include "server.h"

typedef struct _TableElement{
    Ciphertext elem_value;
    vector<Ciphertext> elem_bits;
}TableElement;

typedef struct _TableCol{
    string col_name;
    vector<TableElement*> col_elem;
}TableCol;

typedef struct _Table{
    string table_creator;
    string table_name;
    vector<TableCol*> table_col;
}Table;

typedef struct _DB{
    string db_name;
    vector<Table*> db_tables;
}DB;

class DataBase{
    public:
        DB* db;

        // tested
        DataBase(string db_name){
            // create static database
            static DB* _db = new DB();
            db = _db;
            
            // give a name to the database
            db->db_name = db_name;

            // log
            cout << "[STATUS] Creating DataBase with DB Name: { " << db->db_name << " }\n" << endl;
        }

        // tested
        void createNewDataBase(string db_new_name){
            delete(db);

            static DB* _db = new DB();
            db = _db;

            // give a name to the database
            db->db_name = db_new_name;

            // log
            cout << "[DEBUG] Created New DataBase :: DB Name -> " << db->db_name << endl;
        }

        // tested
        bool searchTable(string tablename){
            for (auto& table : db->db_tables) {
                if(table->table_name.compare(tablename) == 0){
                    return 1;
                }
            }
            return 0;
        }

        // tested
        Table* getTable(string tablename){
            for (auto& table : db->db_tables) {
                if(table->table_name.compare(tablename) == 0) return table;
            }
            return nullptr;
        }

        // tested
        bool createTable(string creator, string tablename, list<string> col_names){
            // search to see if a Table with name tablename already exists
            if(searchTable(tablename)) { cout << "[Error] Table name already registered. Try another" << endl; return 0;}

            static Table* _table = new Table();
            _table->table_name = tablename;
            _table->table_creator = creator;

            for (auto& col : col_names) {
                TableCol* _table_col = new TableCol();
                _table_col->col_name = col;
                _table->table_col.push_back(_table_col);
            }
            
            db->db_tables.push_back(_table);
            return 1;
        }

        // tested
        bool search_all_columns(string tablename, list<string>& res){
            Table* table = getTable(tablename);
            if(!table) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            for (auto col : table->table_col) {
                res.push_back(col->col_name);
            }
            return 1;
        }

        // tested
        bool createColumn(string tablename, string col_name){
            Table* table = getTable(tablename);
            if(!table) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            list<string> found_col;
            search_all_columns(tablename, found_col);
            bool found = (std::find(found_col.begin(), found_col.end(), col_name) != found_col.end());
            
            if(!found){
                cout << "[Error] Column already exists" << endl;
                return 0;
            }

            TableCol* table_col = new TableCol();
            table_col->col_name = col_name;

            table->table_col.push_back(table_col);
            return 1;
        }

        // tested
        TableCol* getColumn(string tablename, string colname){
            Table* _table = getTable(tablename);
            if(!_table) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            for (auto& col : _table->table_col) {
                if(col->col_name.compare(colname) == 0) return col;
            }

            return nullptr;
        }

        // tested
        bool insert_elem_in_column(string tablename,  string colname, TableElement* content){
            TableCol* table_column =  getColumn(tablename, colname);
            if(!table_column) { cout << "[Error] Column does not exist in this table. Try another" << endl; return 0;}
            
            table_column->col_elem.push_back(content);
            return 1;
        }

        // tested
        bool get_all_elem_in_column(string tablename,  string colname, vector<TableElement*> &res){
            TableCol* table_column =  getColumn(tablename, colname);
            if(!table_column) { cout << "[Error] Column does not exist in this table. Try another" << endl; return 0;}
            
            cout << "\n[STATUS] :: Searching in column: { " << table_column->col_name << " } :: Table: { " << tablename << " }\n" << endl;
            
            for (auto& elem : table_column->col_elem) {
                res.push_back(elem);
            }

            return 1;
        }

        // tested
        bool get_all_elem_in_row(string tablename,  int row, vector<TableElement*> &res){
            Table* _table = getTable(tablename);
            if(!_table) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            cout << "\n[STATUS] :: Searching in row: { " << row << " } :: Table: { " << _table->table_name << " }\n" << endl;
            
            for (auto _col = _table->table_col.begin(); _col != _table->table_col.end(); ++_col) {
                for (auto _row = (*_col)->col_elem.begin(); _row != (*_col)->col_elem.end(); ++_row) {
                    int row_number = std::distance((*_col)->col_elem.begin(), _row);
                    if(row_number == row) {
                        res.push_back( (*_row) );
                        break;
                    }
                }
            }

            return 1;
        }

        // tested
        TableElement* get_elem(string tablename, int row, string col){
            Table* _table = getTable(tablename);
            if(!_table) { cout << "[Error] Table does not exist. Try another" << endl; return nullptr;}

            cout << "\n[STATUS] :: Searching for element in position: (" << row << ", " << col << ") :: Table: {" << _table->table_name << " }\n" << endl;
        
            int l = 0;
            for (auto _col : _table->table_col){
                if(_col->col_name.compare( col ) == 0){
                    for(auto _row : _col->col_elem){
                        if(l == row){
                            return _row;
                        }
                        l++;
                    }
                }
            }

            return nullptr;
        }

};

DataBase database("db_test");

#endif
