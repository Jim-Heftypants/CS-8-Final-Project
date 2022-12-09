#include "ftokenize.h"

FTokenizer::FTokenizer(char* fname) {
    _f.open(fname);
    if (_f.fail()){
        cout<<"cant see the file..."<<endl;
        exit(1);
    }
    _stk = STokenizer();
    cout << "initialized tokenizer" << endl;
}
Token FTokenizer::next_token() {
    auto t = Token();
    _stk >> t;
    return t;
}
bool FTokenizer::more() {        //returns the current value of _more
    return _more;
}
int FTokenizer::pos() {          //returns the value of _pos
    return _pos;
}
int FTokenizer::block_pos() {     //returns the value of _blockPos
    return _blockPos;
}
FTokenizer& operator >> (FTokenizer& f, Token& t) { // do the next token
    // cout << ">> operator" << endl;
    if (!f._stk.more()) {
        if (!f.more()) {
            t = Token();
            return f;
        }
        if (!f.get_new_block()) {
            f._more = false;
            t = Token();
            return f;
        }
    }
    // cout << "Getting new token" << endl;
    t = f.next_token();
    return f;
}

bool FTokenizer::get_new_block() {
    // cout << "getting new text block" << endl;
    char chars[MAX_BUFFER];
    _f.read(chars, MAX_BUFFER - 1);
    chars[_f.gcount()] = '\0';
    cout << "Text Block:" << endl << chars << endl << endl;
    _pos += MAX_BUFFER-1;
    if (_f.gcount() == 0) return false;
    _stk.set_string(chars);
    return true;
}