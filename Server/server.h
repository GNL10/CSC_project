#pragma once

#ifndef SERVER_H
#define SERVER_H

#include "seal/seal.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <iterator>
#include <math.h>
#include <fstream>
#include "../config.h"

using namespace seal;
using namespace std;


typedef struct _CondInfo{
    string colname;
    int op; // -1 for <    0 for =    1 for >
    int logical_op; // -1 for OR   1 for AND
    Ciphertext full_num;
    list <Ciphertext> bits_num;
} CondInfo;


void read_command (ifstream &cmd_file_in, ifstream &fhe_file_in);

#endif
