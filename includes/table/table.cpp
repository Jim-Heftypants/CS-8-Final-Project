#include "table.h"

int Table::serial_number = 0;

Table::Table(const string name) {
    copy_table(name);
}
Table::Table(const string name, const vector<string> names) {
    ++Table::serial_number;
    id = Table::serial_number;
    table_name = name;
    set_fields(names);
    string s = string(table_name + "_fields.txt");
    open_fileW(f, s.c_str());
    record = FileRecord(names);
    record.write(f);
    f.close();
    open_fileW(f, string(name+".tbl").c_str());
    f.close();
}

Table::Table(const string name, const vector<string> names, bool special) {
    ++Table::serial_number;
    id = Table::serial_number;
    table_name = name;
    set_fields(names);
    string s = string(table_name+"_fields"+to_string(id)+".txt");
    open_fileRW(f, s.c_str());
    record = FileRecord(names);
    record.write(f);
    f.close();
    // s = string(name+to_string(id)+".tbl").c_str();
    // open_fileW(f, s.c_str());
}

Table::Table(const Table& RHS) {
    table_name = RHS.table_name;
        field_names = RHS.field_names;
        records = {};
        for (auto item : RHS.records) {
            vector<string> vec;
            for (Pair<string, string> p : item) {
                vec.push_back(p.value);
            }
            insert_without_io(vec);
        }
}

void Table::copy_table(string name) {
    // cout << "Copying table with name: " << name << endl;
    // need to set up table copying data from tbl file
    ++Table::serial_number;
    id = Table::serial_number;
    table_name = name;
    string s = string(name + "_fields.txt");
    // cout << "Copy Table fields with name: " << s << endl;
    open_fileRW(f, s.c_str());
    record = FileRecord();
    record.read(f, 0);
    field_names = record.get_record();
    f.close();
    s = string(name+".tbl");
    open_fileRW(f, s.c_str());
    int i = 0;
    long bytes = record.read(f, i);
    if (bytes == 0) {
        // cout << "Nothing in file to read" << endl;
        return;
    }
    while (bytes>0){
        vector<string> rec = record.get_record();
        // cout << "inseting rec: " << rec << endl;
        insert_without_io(rec);
        i++;
        bytes = record.read(f, i);
    }
    // vector<string> rec = record.get_record();
    // if (rec.size() > 0) {
    //     cout << "inseting rec into copy: " << rec << endl;
    //     insert_without_io(rec);
    // }
    f.close();
}

void Table::insert_without_io(vector<string> row) {
    empty = false;
    Map<string, string> m;
    for (int i = 0; i < row.size(); ++i) {
        m.insert(field_names[i], row[i]);
    }
    records.push_back(m);
    for (int i = 0; i < field_names.size(); ++i) {
        field_orders[field_names[i]].insert(m[field_names[i]], records.size()-1);
    }
}

void Table::insert_into(vector<string> row) {
    if (f.is_open()) {
        // cout << "File is already open -- closing it" << endl;
        f.close();
    }
    string s = string(table_name+".tbl");
    // cout << "Inserting " << row << " into " << s << endl;
    open_fileRW(f, s.c_str());
    empty = false;
    record = FileRecord(row);
    record.write(f);
    f.close();
    insert_without_io(row);
}
Table Table::select_all() {
    Table newTable = Table(*this);
    return newTable;
}
Table Table::select(const vector<string> fields, string fieldName, string operation, string fieldValue) {
    vector<int> recordNumbers = select_recs(fieldName, operation, fieldValue);
    return make_table(fields, recordNumbers);
}

Table Table::select(const vector<string> fields) {
    vector<int> vec;
    for (int i = 0; i < records.size(); ++i) {
        vec.push_back(i);
    }
    return make_table(fields, vec);
}

Table Table::select(const vector<string> fields, vector<string> conditions) {
    Queue<Token*> q;
    Stack<Token*> stk;
    for (auto condition : conditions) {
        // cout << "condition: " << condition << endl;
        if (condition == "(") {
            stk.push(new Token(condition, 5));
        } else if (condition == ")") {
            // cout << stk << endl;
            Token* ptr = stk.top();
            stk.pop();
            while (ptr && ptr->token_str() != "(") {
                // cout << "pushing ptr: " << ptr << endl;
                q.push(ptr);
                ptr = stk.top();
                stk.pop();
            }
            // cout << stk << endl;
        } else if (condition.find(">") != string::npos || condition.find("<") != string::npos || condition.find("=") != string::npos) {
            Relational* r = new Relational(condition);
            stk.push(r);
        } else if (condition == "and") {
            Token* ptr = stk.top();
            Logical* l = new Logical(condition);
            if (ptr && ptr->type_string() == "Relational") {
                Token* ptr = stk.top();
                stk.pop();
                q.push(ptr);
            }
            stk.push(l);
        } else if (condition == "or") {
            Token* ptr = stk.top();
            Logical* l = new Logical(condition);
            if (ptr && ptr->type_string() == "Relational" || ptr->token_str() == "and") {
                Token* ptr = stk.top();
                stk.pop();
                q.push(ptr);
            }
            stk.push(l);
        } else {
            TokenStr* tstr = new TokenStr(condition);
            q.push(tstr);
        }
    }
    while (!stk.empty()) {
        Token* ptr = stk.top();
        stk.pop();
        q.push(ptr);
    }
    return select(fields, q);
}

Table Table::select(const vector<string> fields, Queue<Token*> conditions) {
    // cout << "Selecing with token queue and with fields: " << fields << endl;
    string fieldValue, fieldName, operation;
    Stack<Token*> stk;
    RecordSet* set1;
    RecordSet* set2;
    while (!conditions.empty()) {
        //read elements and perform postfix evaluation
        Token* t = conditions.pop();
        auto type = t->type_string();
        // cout << "String popped with type: " << type << endl;
        if (type == "Logical") {
            // pop the two record sets and compare them
            // OR or AND
            // push new record set into the stack
            set1 = (RecordSet*) stk.top();
            stk.pop();
            set2 = (RecordSet*) stk.top();
            stk.pop();
            set<int> s2 = set2->get_set();
            if (t->token_str() == "or") {
                // cout << "ORing two record sets" << endl;
                for (auto item : s2) {
                    set1->insert(item);
                }
                // cout << "Pushing " << *set1 << endl;
                stk.push(set1);
            } else if (t->token_str() == "and") {
                // cout << "ANDing two record sets" << endl;
                RecordSet* s3 = new RecordSet();
                for (auto item : s2) {
                    if (set1->contains(item)) {
                        s3->insert(item);
                    }
                }
                // cout << "Pushing " << *s3 << endl;
                stk.push(s3);
            }
        } else if (type == "Relational") {
            // pop the fieldValue
            // pop the fieldName
            // use relational to compare
            // push record set into the stack
            fieldValue = stk.top()->token_str();
            stk.pop();
            fieldName = stk.top()->token_str();
            stk.pop();
            operation = t->token_str();
            auto recs = select_recs(fieldName, operation, fieldValue);
            // cout << "Recs selected: " << recs << endl;
            set1 = new RecordSet(recs);
            // cout << "Pushing " << *set1 << endl;
            stk.push(set1);
        } else if (type == "TokenStr") {
            // cout << "Pushing " << *t << endl;
            stk.push(t);
        }
    }
    RecordSet* finalRecSet = (RecordSet*) stk.top();
    set<int> set = finalRecSet->get_set();
    vector<int> finalRecs;
    for (auto item : set) {
        finalRecs.push_back(item);
    }
    return make_table(fields, finalRecs);
}

Table Table::make_table(const vector<string> fields, vector<int> record_numbers) {
    last_recnos = record_numbers;
    Table newTable = Table(table_name, fields, true);
    for (auto index : record_numbers) {
        auto record = records[index];
        vector<string> valueSet;
        for (auto field : fields) {
            valueSet.push_back(record[field]);
        }
        newTable.insert_without_io(valueSet);
    }
    return newTable;
}

vector<int> Table::select_recs(string fieldName, string operation, string fieldValue) {
    // cout << "Select " << fieldName << " " << operation << " " << fieldValue << endl;
    if (find(field_names.begin(), field_names.end(), fieldName) == field_names.end()) {
        return vector<int>();
    }
    MMap<string, int> order = field_orders[fieldName];
    vector<int> recordNumbers = vector<int>();
    MMap<string, int>::Iterator it;
    if (operation == "=") {
        auto val = order.at(fieldValue);
        if (val != vector<int>()) {
            for (auto item : val) {
                recordNumbers.push_back(item);
            }
        }
    } else if (operation == ">") {
        it = order.upper_bound(fieldValue);
        for (it; it != order.end(); it++) {
            auto list = (*it).value_list;
            // cout << "item: " << list << endl;
            for (auto item : list) {
                recordNumbers.push_back(item);
            }
        }
    } else if (operation == "<") {
        for (it = order.begin(); it != order.upper_bound(fieldValue); it++) {
            if ((*it).key >= fieldValue) break;
            auto list = (*it).value_list;
            for (auto item : list) {
                recordNumbers.push_back(item);
            }
        }
    } else if (operation == ">=") {
        for (it = order.lower_bound(fieldValue); it != order.end(); it++) {
            auto list = (*it).value_list;
            for (auto item : list) {
                recordNumbers.push_back(item);
            }
        }
    } else if (operation == "<=") {
        for (it = order.begin(); it != order.upper_bound(fieldValue); it++) {
            if ((*it).key > fieldValue) break;
            auto list = (*it).value_list;
            for (auto item : list) {
                recordNumbers.push_back(item);
            }
        }
    }
    return recordNumbers;
}

void Table::set_fields(vector<string> names) {
    field_names = names;
    field_orders.clear();
    for (auto field : field_names) {
        field_orders.insert(field, MMap<string, int>());
    }
}
vector<string> Table::get_fields() {
    return field_names; // optionally read from the file of field names;
}

void Table::sortField(string fieldName) {
    
}

void Table::reindex() {
    for (string& field : field_names) {
        sortField(field);
    }
}