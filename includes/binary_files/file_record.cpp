#include "file_record.h"

string convertToString(char* a, int size) {
    int i;
    string s = "";
    for (i = 0; i < size; i++) {
        if (a[i] == '\0') return s;
        s = s + a[i];
    }
    return s;
}

vector<string> FileRecord::get_record() {
    vector<string> vec;
    for (int i = 0; i < MAX; ++i) {
        if (_record[i][0] == '\0') return vec;
        vec.push_back(convertToString(_record[i], MAX));
    }
    return vec;
}

long FileRecord::write(fstream &outs){   //returns the record number
    //write to the end of the file.
    outs.seekg(0, outs.end);
    long pos = outs.tellp();    //retrieves the current position of the file pointer
    for (int i = 0; i < MAX+1; ++i) {
        outs.write(_record[i], sizeof(_record[i]));
    }
    return pos/((MAX+1)*sizeof(_record[0]));  //record number
}

long FileRecord::read(fstream &ins, long recno){
    //returns the number of bytes read.
    //      read = MAX, or zero if read passed the end of file
    long pos = recno * (MAX+1)*sizeof(_record[0]);
    ins.seekg(pos, ios_base::beg);
    for (int i = 0; i < (MAX+1); ++i) {
        ins.read(_record[i], sizeof(_record[i]));
    }
    return ins.gcount();

}

ostream& operator<<(ostream& outs, const FileRecord& r){
    outs << "File Record:" << endl;
    for (int i = 0; i < (r.MAX+1); ++i) {
        outs << i << ": ";
        for (int j = 0; j < sizeof(r._record[i]); ++j) {
            if (r._record[i][j] == '\0') {
                if (j == 0) {
                    outs << "NULL" << endl;
                    return outs;
                }
                outs << endl;
                break;
            }
            outs << r._record[i][j];
        }
    }
    return outs;
}

