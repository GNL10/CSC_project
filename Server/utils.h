#include "comparator.h"
#include "data_base.h"
#include "server.h"

class Utils{

    public:
        // SELECT SUM(colname) FROM table_example WHERE col1name = _int_ OR col2name > _int_
        Ciphertext selected_sum_with_conditions(Comparator* comp, string tablename, string col_to_sum, list<CondInfo> &conds){
            TableCol* table_col = database.getColumn(tablename, col_to_sum);
            if(!table_col) { cout << "[Error] Table does have this column not exist. Try another" << endl;}

            Ciphertext res;
            
            int l = -1;
            for(auto cond : conds){
                if(cond.logical_op == -1){
                    auto value_zero = comp->_and(cond.bits_num[0], comp->_not(cond.bits_num[0]));
                    /*
                    for(auto col_elem : table_col->col_elem){
                        tuple<Ciphertext,Ciphertext,Ciphertext> c_n_bits = comp->compareNBits(col_elem->elem_bits, cond.bits_num);
                        Ciphertext mult_res = comp->mult(get<1>(c_n_bits), col_elem->elem_value);
                        comp->_eval->add_inplace(value_zero, mult_res);

                        l++;
                        if(l == 1) break;
                    }*/

                    int size = table_col->col_elem.size();
                    int i = 0;

                    if(i > size) return value_zero;

                    auto c_n_bits_1 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto mult_res_1 = comp->mult(get<2>(c_n_bits_1), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, mult_res_1);
                    
                    i++;
                    if(i > size) return value_zero;
                    auto _c_n_bits_2 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto _mult_res_2 = comp->mult(get<2>(_c_n_bits_2), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, _mult_res_2);
/*
                    i++;
                    if(i > size) return value_zero;
                    auto c_n_bits_3 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto mult_res_3 = comp->mult(get<2>(c_n_bits_3), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, mult_res_3);

                    i++;
                    if(i > size) return value_zero;
                    auto _c_n_bits_4 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto _mult_res_4 = comp->mult(get<2>(_c_n_bits_4), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, _mult_res_4);

                    i++;
                    if(i > size) return value_zero;
                    auto c_n_bits_5 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto mult_res_5 = comp->mult(get<2>(c_n_bits_5), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, mult_res_5);
                    
                    i++;
                    if(i > size) return value_zero;
                    auto _c_n_bits_6 = comp->compareNBits(table_col->col_elem[i]->elem_bits, cond.bits_num);
                    auto _mult_res_6 = comp->mult(get<2>(_c_n_bits_6), table_col->col_elem[i]->elem_value );
                    comp->_eval->add_inplace(value_zero, _mult_res_6);
*/
                    return value_zero;
                }
                
                if(cond.logical_op == 0){
                    for(auto col_elem : table_col->col_elem){
                        // auto comp_res = comp->compareNBits(col_elem->elem_bits, cond.bits_num);
                        // auto mult_res = comp->mult( get<1>(comp_res), col_elem->elem_value );

                        // *res = comp->sum(mult_res, *res);
                    }
                }
                if(cond.logical_op == 1){
                    for(auto col_elem : table_col->col_elem){
                        // auto comp_res = comp->compareNBits(col_elem->elem_bits, cond.bits_num);
                        // auto mult_res = comp->mult( get<0>(comp_res), col_elem->elem_value );

                        // *res = comp->sum(mult_res, *res);
                    }
                }
            }
            
            return res;
        }
};