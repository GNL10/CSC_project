#ifndef _CONFIG_H_
#define _CONFIG_H_

using namespace std;

#define DEBUG 1
#define NUM_MAX_BITS 8
#define CLI_NUM 4

static size_t POLY = 32768;
static int COEFF = 881;

static const char *TEST_fname = "../client/client0/test.txt";

static const char *PK_fname = "public_key.txt";
static const char *SK_fname = "secret_key.txt";
static const char *RK_fname = "relin_key.txt";

static const char *fhe_out_fname  = "fhe_to_server.txt";
static const char *fhe_in_fname  = "fhe_from_server.txt";

static const char *cmd_out_fname = "cmds_to_server.txt";
static const char *cmd_in_fname = "cmds_from_server.txt";

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
