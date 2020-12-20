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
        list<list<TableElement>> rows; //change int to ciphertext struct

        Table (string _owner, string _tablename, list <string> &_col_names) {
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
                if(DEBUG) cout << "[DEBUG] Table successfully created: " << _tablename << endl;
                return;
            }
            if(DEBUG) cout << "[DEBUG] Failed creating table: " << _tablename << endl;
        }

        static void insert_into_table(list<Table>* db, string _tablename, \
                    list <string>& arg_list, ifstream &fhe_to_server, SealWrapperServer &sealServer) {
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
    
        // SELECT SUM(colname) FROM table_example WHERE col1name = _int_ OR col2name > _int_
        static void select_sum_with_conditions(list<Table>* db, Comparator* comp, string _tablename, string col_to_sum, list<CondInfo> &conds){
            /*
            string res = "";
            Table *table;

            Ciphertext result;

            if(( table = Table::search_tablename(_tablename, db)) == NULL) {
                // db no element was found
                if(DEBUG) cout << "[DEBUG] select_sum_with_conditions() :: Failed to find Table :: " << _tablename << endl;
                return;
            }

            for (list<CondInfo>::iterator cond = conds.begin(); cond != conds.end(); cond++){
                // OR
                if(cond->logical_op == -1){

                    int _row = 0;
                    int _col = 0;

                    for (list<list>::iterator row = table->rows.begin(); row != table->rows.end(); row++){
                        
                        for (list<TableElement>::iterator element = row->begin() ; element != row->end(); element++){
                            
                            // iterate each value of the same col (vertical iteration) and for each
                            // colname do:
                            auto comp_res = comp->compareNBits(element->bits, cond->bits_num);
                            auto mult_res = comp->mult( get<1>(comp_res), element->full_num );

                        }
                        
                        cout << "row  :: " << _row <<endl;
                        
                        cout << "col  :: " << _col << endl;
                        
                        _col = _col + 1;
                        
                        result = comp->sum( result,   )
                        _row = _row + 1;



                    }

                }

                // AND
                if(cond->logical_op == 1){
                    
                }

                // comp->compare()

            }
*/
        }
        
        // SELECT SUM(colname) FROM tablename WHERE col1name =|<|> value AND|OR col2name =|<|> value
        static void select_sum_all (list<Table>* db, string _tablename, string col_to_sum){

            // see if _tablename exist in the db
        // -1 for <    0 for =    1 for >
            // make 2 dimension vertical iteration, one in the col col1name and
            // another in the col colname, for each index do:
            // auto comp_res = comp->compareNBits(col1name_element->bits, value->bits_num);
            /* switch(cond->op){
                case -1:
                    l = 2;
                    break;
                case 0:
                    l = 1;
                    break;
                case 1:
                    l = 0;
                    break;
                default:    // error
                    l = -3;
                    break;
            }
            */
            // auto mult_res = comp->mult( get<l>(comp_res), element->full_num );

        }

        static void select_colnames_with_conditions (list<Table>* db, string _tablename, \
                                                    list<string> col_list, list<CondInfo> &conds){

        }
        
        static void select_colnames_all (list<Table>* db, string _tablename, list<string> col_list) {

        }

        static void delete_line (list<Table>* db, string _tablename, int linenum) {

        }
};

list<Table> db;

#endif
