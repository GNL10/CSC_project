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

#define NUM_MAX_BITS 8
#define DEBUG 0

using namespace seal;
using namespace std;

typedef struct _CondInfo{
    string colname;
    int op; // -1 for <    0 for =    1 for >
    int logical_op; // -1 for OR   1 for AND
    Ciphertext full_num;
    list <Ciphertext> bits_num;
} CondInfo;

static const char *PK_fname = "public_key.txt";
static const char *SK_fname = "secret_key.txt";
static const char *RK_fname = "relin_key.txt";

static const char *fhe_out_fname  = "fhe_to_server.txt";
static const char *fhe_in_fname  = "fhe_to_server.txt"; //"fhe_from_server.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

static const char *cmd_out_fname = "cmds_out_enc.txt";
static const char *cmd_in_fname = "cmds_out_enc.txt"; //"cmds_in_enc.txt"; CHANGE TO THIS WHEN SERVER IS WORKING!!!

static const char *int_placeholder = "_int_";

static const string CREATE_TABLE = "CREATE TABLE ";
static const string INSERT_INTO = "INSERT INTO ";
static const string VALUES = "VALUES ";
static const string SELECT = "SELECT ";
static const string LINE = "LINE ";
static const string DELETE = "DELETE ";
static const string FROM = "FROM ";
static const string WHERE = "WHERE ";
static const string SUM = "SUM(";
static const string SPACE = " ";
static const string AND = " AND ";
static const string OR = " OR ";
static const string BIGGER = ">";
static const string SMALLER = "<";
static const string EQUAL = "=";

#endif
