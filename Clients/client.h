#pragma once

#ifndef CLIENT_H
#define CLIENT_H

#include "seal/seal.h"
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include <iterator>
#include <math.h>
#include <fstream>

#define DEBUG 0

using namespace seal;
using namespace std;

#define NUM_MAX_BITS 8

static const char *PK_fname = "./files/keys/public_key.txt";
static const char *SK_fname = "./files/keys/secret_key.txt";
static const char *RK_fname = "./files/keys/relin_key.txt";

static const string FHE_OUT_FNAME  = "../files/com/fhe_to_server.txt";
static const string FHE_IN_FNAME  = "../files/com/fhe_from_server.txt";

static const string CMD_OUT_FNAME = "../files/com/cmds_out_enc.txt";
static const string CMD_IN_FNAME = "../files/com/cmds_in_enc.txt";

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