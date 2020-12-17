#include "read_cmd_server.h"

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
