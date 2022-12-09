#ifndef RELATIONAL_H
#define RELATIONAL_H

#include "../tokenizer/token.h"

class Relational : public Token {
public:
    Relational(string value) : Token(value, 2) {
    }
};

#endif