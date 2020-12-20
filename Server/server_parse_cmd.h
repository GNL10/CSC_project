#pragma once

#ifndef PARSE_CMD_H
#define PARSE_CMD_H

#include "server.h"
#include "api.h"
#include "data_base.h"

class ServerParseCmd{

    public:

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
            else
                throw "ERROR: get_tablename was unable to identify the table name";
        }

        CondInfo get_one_condition (string str) {
            size_t pos = 0, pos_and = 0, pos_or = 0;
            string curr_cond;
            CondInfo condition;

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
            return condition;
        }

        void parse_conditions (string line, list<CondInfo> &conditions) {
            string curr_cond;
            CondInfo cond;

            //analysing 1st condition
            conditions.push_back(get_one_condition(line));
            while (line.find(AND) != std::string::npos || line.find(OR) != std::string::npos) {
                if (find_and_del_in_str(line, AND)) {
                    cond = get_one_condition(line);
                    cond.logical_op = 1;
                    conditions.push_back(cond);
                }
                else if (find_and_del_in_str(line, OR)) {
                    cond = get_one_condition(line);
                    cond.logical_op = -1;
                    conditions.push_back(cond);
                }
            }
        }

        void read_command (Api &api, int clinum, SealWrapperServer &sealServer, Comparator *comparator) {
            string line;
            size_t pos;
            
            while ( getline (api.cmd_to_server[clinum],line)) { //run through all lines of file
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

                        //Table::create_table(db, "defaultown", tablename, col_list);
                        database.createTable("client" + to_string(clinum), tablename, col_list);
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
                            //Table::insert_into_table(db, tablename, arg_list, api.fhe_to_server[clinum], api.cmd_to_client[clinum], sealServer);
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
                                //Table::select_line(db, tablename, linenum, api.cmd_to_client[clinum], api.fhe_to_client[clinum]);
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
                                //Table::select_sum_with_conditions(db, comparator, tablename, sum, conditions);
                            }
                            else { // there are no condit
                                // sum every line
                                //Table::select_sum_all (db, tablename, sum);
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
                                //Table::select_colnames_with_conditions(db, tablename, col_list, conditions);
                            }
                            else { // there are no condit
                                // all lines
                                //Table::select_colnames_all(db, tablename, col_list);
                                list<string> res;
                                database.search_all_columns(tablename, res); 
                            }
                        }
                    }

                }
                else if (find_and_del_in_str(line, DELETE)) {
                    if ((pos = line.find(SPACE)) != std::string::npos) {
                        linenum = stoi(line.substr(0, pos));
                        if (find_and_del_in_str(line, FROM)) {
                            tablename = line;
                            //Table::delete_line(db, tablename, linenum, api.cmd_to_client[clinum]);
                        }
                    }
                }
                else {
                    api.cmd_to_client[clinum] << "Command : " << line << " not recognized." << endl;
                    cout << "Command:" << line << " not recognized." << endl;
                }
            }
            api.cmd_to_server[clinum].clear(); // clear the EOF from ifstream, so the file updates can later be read
        }

};

#endif
