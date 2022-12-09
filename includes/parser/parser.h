#ifndef PARSER_H
#define PARSER_H

#include "../table/table.h"
#include "../tokenizer/stokenize.h"

using namespace std;

class Parser {
public:
    Parser(char s[]) {
        set_string(s);
    }

    void set_string(char s[]) {
        st = STokenizer(s);
        tVec = get_token_vec(st);
        command = vector<string>();
        table_name = vector<string>();
        fields = vector<string>();
        values = vector<string>();
        conditions = vector<string>();
        // cout << "tVec set as: " << tVec << endl;
    }

    mmap_ss parse_tree() {
        if (tVec[0].type_string() == "Command") {
            command.push_back(tVec[0].token_str());
            auto s = tVec[0].token_str();
            if (s == "select") {
                return select();
            } else if (s == "insert") {
                return insert();
            } else if (s == "make" || s == "create") {
                return make();
            } else {
                cout << "Impossible token passed into parser" << endl;
            }
        } else {
            cout << "Invalid input passed into parser" << endl;
        }
    }

private:
    mmap_ss make() {
        table_name.push_back(tVec[2].token_str());
        string s = tVec[3].token_str();
        if (s == "field" || s == "fields") {
            for (int i = 4; i < tVec.size(); ++i) {
                fields.push_back(tVec[i].token_str());
            }
        } else if (s == "as") {
            // select statement from another table
        } else {
            cout << "invalid tVec in make()" << endl;
        }
        // cout << "Set all Make Multimap fields" << endl;
        return make_multimap();
    }

    mmap_ss insert() {
        table_name.push_back(tVec[2].token_str());
        for (int i = 4; i < tVec.size(); ++i) {
            values.push_back(tVec[i].token_str());
        }
        return make_multimap();
    }

    mmap_ss select() {
        int i;
        for (i = 1; i < tVec.size(); ++i) {
            if (tVec[i].token_str() == "from") {
                ++i;
                break;
            }
            fields.push_back(tVec[i].token_str());
        }
        for (i; i < tVec.size(); ++i) {
            if (tVec[i].token_str() == "where") {
                ++i;
                break;
            }
            table_name.push_back(tVec[i].token_str());
        }
        for (i; i < tVec.size(); ++i) {
            conditions.push_back(tVec[i].token_str());
        }
        return make_multimap();
    }

    mmap_ss make_multimap() {
        mmap_ss multi;
        for (auto item : command) {
            multi.insert("command", item);
        }
        for (auto item : table_name) {
            multi.insert("table_name", item);
        }
        for (auto item : fields) {
            multi.insert("fields", item);
        }
        for (auto item : values) {
            multi.insert("values", item);
        }
        for (auto item : conditions) {
            multi.insert("conditions", item);
        }

        // multi.insert("where", "");
        // if (fields.size() == 0) multi.insert("fields", "");

        // cout << "Multimap: " << endl << multi << endl;
        return multi;
    }


    vector<Token> get_token_vec(STokenizer& st) {
        vector<Token> vec;
        Token t;
        st >> t;
        while (st.more()) {
            if (t.type_string() != "UNKNOWN") {
                vec.push_back(t);
            }
            t = Token();
            st >> t;
        }
        return vec;
    }

    STokenizer st;
    vector<Token> tVec;

    vector<string> command, table_name, fields, values, conditions;
};

#endif