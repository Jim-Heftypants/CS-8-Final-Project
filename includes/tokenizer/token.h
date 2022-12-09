#ifndef TOKEN_H
#define TOKEN_H

#include "constants.h"
#include <iostream>
#include "cstring"
#include <algorithm>

using namespace std;

class Token {
public:
    Token() {}
    Token(string str, int type) {
        _token = str;
        _type = type;
    }
    int type() const { return _type; }
    string type_string() const {
        switch (_type) {
            case 1:
                return "TokenStr";
            case 2:
                return "Relational";
            case 3:
                return "Logical";
            case 4:
                return "Command";
            case 5:
                return "Specifier";
            default:
                return "UNKNOWN";
        }
    }
    string token_str() const { return _token; }
    friend ostream& operator <<(ostream& outs, const Token& t) {
        outs << '[' << t._token << ", " << t.type_string() << ']';
        return outs;
    }
private:
    string _token = "";
    int _type = -1;
};

#endif