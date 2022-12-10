#ifndef SQL_H
#define SQL_H

#include "../parser/parser.h"

using namespace std;

class SQL {
public:
    SQL() {}

    Table command(string s) { // get a SQL command and run it
        int n = s.length();
        char c[n+1];
        strcpy(c, s.c_str());
        Parser p = Parser(c);
        pMap = p.parse_tree();
        cmd = pMap["command"][0];
        name = pMap["table_name"][0];
        if (cmd == "select") {
            return select();
        } else if (cmd == "make" || cmd == "select") {
            return make();
        } else if (cmd == "insert") {
            return insert();
        } else {
            cout << "Invalid string passed into SQL Command()" << endl;
        }
    }

    vector<long> select_recnos() {
        return last_recnos;
    }

    // function to read batch file (each line is one command)
    // function to read terminal with cin get_line

private:
    Table make() {
        return Table(name, pMap["fields"]);
    }

    Table insert() {
        vector<string> values = pMap["values"];
        Table table = Table(name);
        // cout << "Pre-Table: " << table << endl;
        table.insert_into(values);
        // cout << "Values: " << values << endl;
        // cout << "Post-Table: " << table << endl;
        return table;
    }

    Table select() {
        vector<string> fields = pMap["fields"];
        cout << "Selecting fields: " << fields << endl;
        vector<string> conditions = pMap["condition"];
        Table table = Table(name);
        if (fields.size() == 1 && fields[0] == "*") {
            // cout << "Selecting all fields" << endl;
            fields = table.get_fields();
        }
        if (!pMap.contains("where")) {
            // cout << "There is no condition" << endl;
            Table t = table.select(fields);
            cout << "Selection Table: " << t << endl;
            last_recnos = table.select_recnos();
            return t;
        }
        Table t = table.select(fields, conditions);
        cout << "Selection Table: " << t << endl;
        last_recnos = table.select_recnos();
        return t;
    }

    string name, cmd;
    mmap_ss pMap;
    vector<long> last_recnos;
};

#endif