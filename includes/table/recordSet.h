#ifndef RECORD_SET_H
#define RECORD_SET_H

#include <set>
#include <vector>
#include <iostream>

#include "../tokenizer/token.h"

using namespace std;

class RecordSet : public Token {
public:
    RecordSet() : Token("", 4) {}
    RecordSet(vector<int> vec) : RecordSet() {
        for (auto i : vec) {
            s.insert(i);
        }
    }

    void insert(int i) {
        s.insert(i);
    }

    void remove(int pos) {
        s.erase(pos);
    }

    set<int> get_set() {
        return s;
    }

    bool contains(int item) {
        return s.find(item) != s.end();
    }

    int get_size() { return s.size(); }

    friend ostream& operator<<(ostream& outs, const RecordSet& print_me){
        outs << "<";
        for (auto item : print_me.s) {
            outs << item << ", ";
        }
        outs << ">";
        return outs;
    }

private:
    set<int> s;
};

#endif