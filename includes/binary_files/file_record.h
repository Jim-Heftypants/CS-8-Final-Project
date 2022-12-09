#ifndef FILE_RECORD_H
#define FILE_RECORD_H

#include <iostream>
#include <iomanip>
#include <string.h>
#include <vector>
#include "utilities.h"

using namespace std;

class FileRecord{
public:
    FileRecord(){
        _record[0][0] = '\0';
        _num_of_fields = 0;
    }

    FileRecord(string str) {
        vec_rec = vector<string>();
        vec_rec.push_back(str);
        int i;
        for (i = 0; i < str.length(); ++i) {
            _record[0][i] = str[i];
        }
        _record[0][i] = '\0';
        _num_of_fields = 1;
        _record[1][0] = '\0';
    }

    FileRecord(vector<string> vec) {
        vec_rec = vec;
        int i;
        for (i = 0; i < vec.size(); ++i) {
            int j;
            for (j = 0; j < vec[i].length(); ++j) {
                _record[i][j] = vec[i][j];
            }
            ++_num_of_fields;
            _record[i][j] = '\0';
        }
        _record[i][0] = '\0';
        // cout << *this << endl;
    }

    string operator[] (int idx) {
        return vec_rec[idx];
    }

    vector<string> get_record();

    long write(fstream& outs);
    long read(fstream& ins, long recno);

    friend ostream& operator<<(ostream& outs, const FileRecord& r);

    static const int MAX = 100;
    char _record[MAX+1][MAX+1]; //NULL char
    int _num_of_fields = 0;
    vector<string> vec_rec;
    
};

#endif