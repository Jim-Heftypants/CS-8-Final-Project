#include "./stokenize.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];

STokenizer::STokenizer() {
    make_table();
}
STokenizer::STokenizer(char str[]) : STokenizer() {
    for (int i = 0; i < strlen(str); i++) {
        _buffer[i] = str[i];
    }
    if (strlen(str) < strlen(_buffer)) {
        _buffer[strlen(str)] = '\0';
    }
    shouldContinue = true;
}

bool STokenizer::done() {   //true: there are no more tokens
    return !shouldContinue;
}            
bool STokenizer::more() {   //true: there are more tokens
    return !done();
}

//---------------
//extract one token (very similar to the way cin >> works)
STokenizer& operator >> (STokenizer& s, Token& t) {
    // cout << "STK getting next token" << endl;
    string tokenString = "";
    s.shouldContinue = s.get_token(0, tokenString);
    if (s.shouldContinue == false) {
        return s;
    }

    if (tokenString[0] == '\"') {
        tokenString.erase(tokenString.size() - 1);
        tokenString.erase(0, 1);
        // cout << tokenString << endl;
    }

    int tokenType = 0;
    if (find(LOGICALS.begin(), LOGICALS.end(), tokenString) != LOGICALS.end()) {
        tokenType = TOKEN_LOGICAL;
    } else if (find(RELATIONALS.begin(), RELATIONALS.end(), tokenString) != RELATIONALS.end()) {
        tokenType = TOKEN_RELATIONAL;
    } else if (find(COMMANDS.begin(), COMMANDS.end(), tokenString) != COMMANDS.end()) {
        tokenType = TOKEN_COMMAND;
    } else if (find(SPECIFIERS.begin(), SPECIFIERS.end(), tokenString) != SPECIFIERS.end()) {
        tokenType = TOKEN_SPECIFIER;
    } else if (find(GARBAGE.begin(), GARBAGE.end(), tokenString) == GARBAGE.end()) {
        tokenType = TOKEN_STR;
    } else {
        tokenType = TOKEN_UNKNOWN;
    }

    t = Token(tokenString, tokenType);
    // cout << "found token: " << t << endl;
    return s;
}

//set a new string as the input string
void STokenizer::set_string(char str[]) {
    std::fill(_buffer, _buffer+MAX_BUFFER, 0);
    for (int i = 0; i < strlen(str); i++) {
        _buffer[i] = str[i];
    }
    if (strlen(str) < strlen(_buffer)) {
        _buffer[strlen(str)] = '\0';
    }
    _pos = 0;
    shouldContinue = true;
}



//create table for all the tokens we will recognize
//                      (e.g. doubles, words, etc.)
void STokenizer::make_table() {
    int numStates = 8;
    for (int i = 0; i < numStates; i++) {
        for (int j = 0; j < MAX_COLUMNS; j++) {
            _table[i][j] = TOKEN_UNKNOWN;
        }
    }

    // traversals
    for (auto c : ALFA) _table[0][c] = 1;
    for (auto c : ALFA) _table[1][c] = 1;
    for (auto c : DIGITS) _table[1][c] = 1;

    for (auto c : DIGITS) _table[0][c] = 2;
    for (auto c : DIGITS) _table[2][c] = 2;
    _table[2]['.'] = 6;
    for (auto c : DIGITS) _table[6][c] = 7;
    for (auto c : DIGITS) _table[7][c] = 7;

    for (auto c : OPERATORS) _table[0][c] = 3;
    for (auto c : SUB_OPERATORS) _table[0][c] = 10;
    for (auto c : SUB_OPERATORS) _table[10][c] = 10;

    for (auto c : SPACES) _table[0][c] = 4;
    for (auto c : SPACES) _table[4][c] = 4;

    for (auto c : PUNC) _table[0][c] = 5;

    _table[0]['\"'] = 8;

    for (auto c : ALFA) _table[8][c] = 8;
    for (auto c : PUNC) _table[8][c] = 8;
    for (auto c : DIGITS) _table[8][c] = 8;
    for (auto c : SPACES) _table[8][c] = 8;
    _table[8]['\"'] = 9;

    // success states
    _table[0][0] = 0;
    _table[1][0] = 1;
    _table[2][0] = 1;
    _table[3][0] = 1;
    _table[4][0] = 1;
    _table[5][0] = 1;
    _table[6][0] = 0;
    _table[7][0] = 1;
    _table[8][0] = 0;
    _table[9][0] = 1;
    _table[10][0] = 1;

//    for (int i = 0; i < numStates; ++i)
//     {
//         for (int j = 0; j < MAX_COLUMNS; ++j)
//         {
//             std::cout << _table[i][j] << ' ';
//         }
//         std::cout << std::endl << std::endl << "================================================" << std::endl << std::endl;
//     }
}

//extract the longest string that match
//     one of the acceptable token types
bool STokenizer::get_token(int start_state, string& token) {
    if (_pos == strlen(_buffer) || _buffer[_pos] == '\0') return false;
    // cout << "buffer string: " << _buffer << endl;
    // cout << "start position: " << _pos << endl;
    int state = start_state;
    int startPos = _pos;
    int lastSuccessfulPos = _pos;
    bool hadSuccess = false;
    while (state != TOKEN_UNKNOWN) {
        // cout << "current position: " << _pos << endl;
        // cout << "current state: " << state << endl;

        if (_table[state][0] == 1) {
            hadSuccess = true;
            // std::cout << "success state reached" << endl;
            token = "";
            lastSuccessfulPos = _pos - 1;
            for (int i = startPos, j = 0; i < lastSuccessfulPos + 1; i++, j++) {
                token += _buffer[i];
            }
            // cout << "current token: " << token << endl << endl;
        }

        if (_pos == strlen(_buffer) || _buffer[_pos] == '\0') break;
        
        // cout << "buffer[pos] -> " << _buffer[_pos] << endl;
        state = _table[state][_buffer[_pos]];
        if (_buffer[_pos] < 1) {
            // cout << "invalid character pointed to 0 -- correcting state to fail" << endl;
            state = TOKEN_UNKNOWN;
        }
        if (state == 0) {
            // cout << "Invalid state" << endl;
            state = TOKEN_UNKNOWN;
        }
        
        // cout << "post state: " << state << endl;
        ++_pos;
    }
    if (!hadSuccess) {
        token += _buffer[startPos];
    }
    _pos = lastSuccessfulPos + 1;
    
    // token is the string of characters between startPos and lastSuccessfulPos
    // std::cout << "token found with length: " << (lastSuccessfulPos - startPos) + 1 << std::endl;
    return true;
}