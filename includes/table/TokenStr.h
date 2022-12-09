#ifndef TOKEN_STR_H
#define TOKEN_STR_H

#include "../tokenizer/token.h"

class TokenStr : public Token {
public:
    TokenStr(string value) : Token(value, 1) {
        // cout << "TokenStr type: " << type_string() << endl;
    }
};

#endif