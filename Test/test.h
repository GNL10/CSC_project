#pragma once

#ifndef TEST_H
#define TEST_H

#include "seal/seal.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <iterator>
#include <math.h>
#include <fstream>

using namespace seal;
using namespace std;

#define DEBUG 1;

// DATA BASE STRUCTS

typedef struct _CondInfo{
    string colname;
    int op; // -1 for <    0 for =    1 for >
    int logical_op; // -1 for OR   1 for AND
    Ciphertext full_num;
    vector <Ciphertext> bits_num;
} CondInfo;

#endif