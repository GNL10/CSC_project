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

        DataBase(string db_name){
            // create static database
            static DB* _db = new DB();
            db = _db;
            
            // give a name to the database
            db->db_name = db_name;

            // log
            cout << "[DEBUG] Created DataBase :: DB Name -> " << db->db_name << endl;
        }

        void createNewDataBase(string db_new_name){
            delete(db);

            static DB* _db = new DB();
            db = _db;

            // give a name to the database
            db->db_name = db_new_name;

            // log
            cout << "[DEBUG] Created New DataBase :: DB Name -> " << db->db_name << endl;
        }

        Table* searchTable(string tablename){
            for (auto& table : db->db_tables) {
                if(table->table_name.compare(tablename) == 0) return table;
            }
            return NULL;
        }

        bool createTable(string creator, string tablename, list<string> col_names){
            // search to see if a Table with name tablename already exists
            if(searchTable(tablename) != NULL) { cout << "[Error] Table name already registered. Try another" << endl; return 0;}

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

        bool deleteTable(string tablename){
            Table* _table;
            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            delete(_table);
            return 1;
        }

        bool createColumns(string tablename, string col_name){
            Table* _table;
            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            TableCol* _table_col = new TableCol();
            _table_col->col_name = col_name;

            _table->table_col.push_back(_table_col);

            return 1;
        }

        bool search_all_columns(string tablename, list<string>& res){
            Table* _table;

            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            for (auto& col : _table->table_col) {
                res.push_back(col->col_name);
            }
            return 1;
        }

        bool search_column(string tablename, string colname, TableCol* col_res){
            Table* _table;
            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            for (auto col : _table->table_col) {
                if(col->col_name.compare(colname)) col_res = col;
            }
            return 1;
        }

        bool insert_table_elem(string tablename,  string colname, TableElement* content){
            TableCol* table_column;
            if(!search_column(tablename, colname, table_column)) { cout << "[Error] Column does not exist in this table. Try another" << endl; return 0;}

            table_column->col_elem.push_back(content);
            return 1;
        }

        bool search_all_columns_elem(string tablename,  string colname, vector<TableElement*> elem_res){
            TableCol* table_column;
            if(!search_column(tablename, colname, table_column)) { cout << "[Error] Column does not exist in this table. Try another" << endl; return NULL;}

            for (auto& elem : table_column->col_elem) {
                elem_res.push_back(elem);
            }

            return 1;
        }

        bool search_all_row_elem(string tablename,  int row, vector<TableElement*> elem_res){
            Table* _table;
            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            int _l = 0;

            for (auto it = _table->table_col.begin(); it != _table->table_col.end(); ++it) {
                int index = std::distance(_table->table_col.begin(), it);
            }

            for (auto col : _table->table_col) {
                
            }

        }
/*
        TableElement* get_elem(string tablename, int row, int col){
            Table* _table;
            if( (_table = searchTable(tablename)) == NULL) { cout << "[Error] Table does not exist. Try another" << endl; return 0;}

            for (auto _col = _table->table_col.begin(); _col != _table->table_col.end(); _col++) {
                int col_index = std::distance(_table->table_col.begin(), _col);
                if(col_index = col){
                    for (auto _row = (*_col)->col_elem.begin(); _row != (*_col)->col_elem.end(); _row++) {
                        int row_index = std::distance((*_col)->col_elem.begin(), _row);
                        if(row_index = row){
                            return (*_row);
                        }
                    }
                }
            }

        }
        */

};

DataBase database("db_test");

#endif
