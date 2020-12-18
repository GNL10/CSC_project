#ifndef READ_CMD_SERVER_H
#define READ_CMD_SERVER_H

#include "server.h"

bool find_and_del_in_str (string &str, string str_to_find);
list <string> read_within_commas (string str);
string get_tablename (string &line);
CondInfo get_one_condition (string str);
void parse_conditions (string line, list<CondInfo> &conditions);

#endif
