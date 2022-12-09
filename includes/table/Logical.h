#ifndef LOGICAL_H
#define LOGICAL_H

#include "../tokenizer/token.h"

class Logical : public Token {
public:
    Logical(string value) : Token(value, 3) {
        
    }
};

#endif