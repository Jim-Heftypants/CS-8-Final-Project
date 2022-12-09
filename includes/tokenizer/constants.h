#include <iostream>
#include <vector>

using namespace std;

const int MAX_COLUMNS = 256;
const int MAX_ROWS = 100;
const int MAX_BUFFER = 200;

const char ALFA[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char DIGITS[] = "0123456789";
const char OPERATORS[] = "><=!+-%&|";
const char SUB_OPERATORS[] = "<>=";
const char SPACES[] = {' ', '\t', '\n', '\0'};
const char PUNC[] = "?.,:;'`~!";
const int START_DOUBLE = 0;
const int START_SPACES = 4;
const int START_ALPHA = 6;
const int START_OPERATOR = 20;
const int START_PUNC = 10;

const vector<string> LOGICALS = {"or", "and"};
const vector<string> RELATIONALS = {"<", ">", "=", "<=", ">="};
const vector<string> COMMANDS = {"select", "make", "insert"};
const vector<string> SPECIFIERS = {"from", "table", "fields", "values"};
const vector<string> GARBAGE = {" ", ",", "\t", "\n", "\0"};

//token types:
const int TOKEN_RELATIONAL = 2;
const int TOKEN_LOGICAL = 3;
const int TOKEN_STR = 1;
const int TOKEN_COMMAND = 4;
const int TOKEN_SPECIFIER = 5;

const int TOKEN_UNKNOWN = -1;
const int TOKEN_END = -2;