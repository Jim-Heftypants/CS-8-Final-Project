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
    Table() {}
    Table(const string name);
    Table(const string name, const vector<string> names);
    Table(const string name, const vector<string> names, bool special);
    Table(const Table& copyTable);
    ~Table() {};

    void insert_without_io(vector<string> row);
    void insert_into(vector<string> row);
    Table select_all();
    Table select(const vector<string> fields, string fieldName, string operation, string fieldValue);
    Table select(const vector<string> fields, vector<string> conditions);
    Table select(const vector<string> fields, Queue<Token*> conditions);
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

    void sortField(string fieldName);
    void reindex();

    friend ostream& operator<<(ostream& outs, const Table& print_me){
        outs << "Table:" << endl;
        outs << "ID" << "\t";
        for (auto field : print_me.field_names) {
            outs << field << "\t";
        }
        outs << endl;
        for (int i = 0; i < print_me.records.size(); ++i) {
            cout << i << "\t";
            for (auto field : print_me.field_names) {
                outs << print_me.records[i].at(field) << "\t";
            }
            outs << endl;
        }
        // outs << endl << print_me.field_orders.at("age") << endl;
        return outs;
    }

private:
    FileRecord record;
    string table_name;
    // vector to hold all record data
    // {id: values, ...}
    vector<Map<string, string>> records;
    vector<string> field_names;
    // maps field to a map of field value and ID
    // inner map will be sorted by field values
    Map<string, MMap<string, int>> field_orders;
    bool empty = true;
    // global serial number -->> class variable
    static int serial_number;
    int id = 0;
    // int last_record_num = 0;

    vector<int> last_recnos;

    fstream f;
};

#endif