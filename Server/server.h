#pragma once

#ifndef _SERVER_H_
#define _SERVER_H_

#include "seal/seal.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <iterator>
#include <math.h>

#include "../FHE.h"
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

#endif
