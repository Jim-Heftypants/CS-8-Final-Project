#ifndef TABLE_H
#define TABLE_H

#include "typedefs.h"
#include "../binary_files/file_record.h"
#include "../bplustree/map.h"
#include "../bplustree/multimap.h"
#include "../queue/MyQueue.h"
#include "../stack/MyStack.h"
#include "recordSet.h"
#include "Logical.h"
#include "TokenStr.h"
#include "Relational.h"
#include <algorithm>

using namespace std;

class Table {
public:
    Table() {};
    Table(const string name);
    Table(const string name, const vector<string> names);
    Table(const string name, const vector<string> names, bool special);
    Table(const Table& RHS);
    Table& operator=(const Table &RHS) {
        table_name = RHS.table_name;
        field_names = RHS.field_names;
        records = {};
        for (auto item : RHS.records) {
            vector<string> vec;
            for (auto field : field_names) {
                vec.push_back(item.at(field));
            }
            insert_without_io(vec);
        }
        return *this;
    }
    void copy_table(string name);
    ~Table() {};

    void insert_without_io(vector<string> row);
    void insert_into(vector<string> row);
    Table select_all();
    Table select(const vector<string> fields, string fieldName, string operation, string fieldValue);
    Table select(const vector<string> fields, vector<string> conditions);
    Table select(const vector<string> fields, Queue<Token*> conditions);
    Table select(const vector<string> fields);
    vector<int> select_recs(string fieldName, string operation, string fieldValue);
    vector<long> select_recnos() {
        vector<long> newVec;
        for (auto item : last_recnos) {
            newVec.push_back((long) item);
        }
        return newVec;
    }
    Table make_table(const vector<string> fields, vector<int> record_numbers);

    void set_fields(vector<string> names);
    vector<string> get_fields();

    friend ostream& operator<<(ostream& outs, const Table& print_me){
        outs << "Table:" << endl;
        outs << "ID" << "\t" << "\t";
        for (auto field : print_me.field_names) {
            outs << field << "\t" << "\t";
        }
        outs << endl;
        for (int i = 0; i < print_me.records.size(); ++i) {
            cout << i << "\t" << "\t";
            for (auto field : print_me.field_names) {
                // cout << "Field: " << field << " : ";
                outs << print_me.records[i].at(field) << "\t" << "\t";
            }
            outs << endl;
        }
        return outs;
    }

private:
    FileRecord record;
    string table_name;
    vector<Map<string, string>> records;
    vector<string> field_names;
    Map<string, MMap<string, int>> field_orders;
    bool empty = true;
    static int serial_number;
    int id = 0;
    vector<int> last_recnos;
    fstream f;
};

#endif