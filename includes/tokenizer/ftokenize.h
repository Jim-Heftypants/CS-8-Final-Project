#ifndef FTOKENIZE_H
#define FTOKENIZE_H

#include "stokenize.h"
#include "fstream"

using namespace std;

class FTokenizer {
public:
    const int MAX_BLOCK = MAX_BUFFER;
    FTokenizer(char* fname);
    Token next_token();
    bool more();        //returns the current value of _more
    int pos();          //returns the value of _pos
    int block_pos();     //returns the value of _blockPos
    friend FTokenizer& operator >> (FTokenizer& f, Token& t);
private:
    bool get_new_block(); //gets the new block from the file
    ifstream _f;   //file being tokenized
    STokenizer _stk;     //The STokenizer object to tokenize current block
    int _pos = 0;           //Current position in the file
    int _blockPos = 0;      //Current position in the current block
    bool _more = true;         //false if last token of the last block
                        //  has been processed and now we are at
                        //  the end of the last block.
};

#endif