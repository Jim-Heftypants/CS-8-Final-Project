#ifndef BTREE_ARRAY_FUNCTIONS_H
#define BTREE_ARRAY_FUNCTIONS_H

#include <iostream>
#include <vector>

using namespace std;

template <class T>
T maximal(const T& a, const T& b) {                      //return the larger of the two items
    return max(a, b);
}

template <class T>
void swap(T& a, T& b) {  //swap the two items
    auto tmp = b;
    b = a;
    a = tmp;
}

template <class T>
int index_of_maximal(T data[], int n) {                 //return index of the largest item in data
    auto max = data[0];
    int idx = 0;
    for (int i = 1; i < n; ++i) {
        if (data[i] > max) {
            idx = i;
            max = data[i];
        }
    }
    return idx;
}

template <class T>
int first_ge(const T data[], int n, const T& entry) {   //return the first element in data that is not less than entry
    for (int i = 0; i < n; ++i) {
        if (data[i] >= entry) return i;
    }
    return n;
}

template <class T>
void attach_item(T data[], int& n, const T& entry) {    //append entry to the right of data
    data[n++] = entry;
}

template <class T>
void insert_item(T data[], int i, int& n, T entry) {    //insert entry at index i in data
    T tmp;
    auto item = entry;
    ++n;
    for (i; i < n; ++i) {
        tmp = data[i];
        data[i] = item;
        item = tmp;
    }
}

template <class T>
void ordered_insert(T data[], int& n, T entry) {        //insert entry into the sorted array data with length n
    int i = 0;
    T item = entry;
    for (i = 0; i < n; ++i) {
        if (data[i] > entry) break;
    }
    insert_item(data, i, n, entry);
}

template <class T>
void detach_item(T data[], int& n, T& entry) {          //remove the last element in data and place it in entry
    --n;
    entry = data[n];
}

template <class T>
void delete_item(T data[], int i, int& n, T& entry) {   //delete item at index i and place it in entry
    if (i >= n || i < 0) {
        entry = NULL;
        return;
    }
    entry = data[i];
    --n;
    for (i; i < n; ++i) {
        data[i] = data[i+1];
    }
}

template <class T>
void merge(T data1[], int& n1, T data2[], int& n2) {   //append data2 to the right of data1
    for (int i = 0; i < n2; ++i) {
        data1[n1++] = data2[i];
    }
}

template <class T>
void split(T src[], int& n1, T dest[], int& n2) {   //move n/2 elements from the right of data1 and move to data2
    n2 = 0;
    // cout << "splitting with n1: " << n1 << '\t';
    for (int i = (n1+1)/2; i < n1; ++i) {
        dest[n2] = src[i];
        ++n2;
    }
    n1 -= n2;
    // cout << "n2 after split: " << n2 << endl;
}

template <class T>
void copy_array(T dest[], const T src[], int& dest_size, int src_size) {  //copy src[] into dest[]
    dest_size = 0;
    for (int i = 0; i < src_size; ++i) {
        dest[dest_size] = src[i];
        ++dest_size;
    }
}

template <class T>
void print_array(const T data[], int n, int pos = -1) {  //print array data
    for (int i = 0; i < n; ++i) {
        cout << data[i];
        if (i != n-1) cout << ", ";
    }
    cout << endl;
}

template <class T>
bool is_gt(const T data[], int n, const T& item) {       //item > all data[i]
    for (int i = 0; i < n; ++i) {
        if (item <= data[i]) return false;
    }
    return true;
}

template <class T>
bool is_le(const T data[], int n, const T& item) {       //item <= all data[i]
    for (int i = 0; i < n; ++i) {
        if (item > data[i]) return false;
    }
    return true;
}

//-------------- Vector Extra operators: ---------------------

template <typename T>
ostream& operator <<(ostream& outs, const vector<T>& list) { //print vector list
    outs << "[";
    for (int i = 0; i < list.size(); ++i) {
        outs << list[i];
        if (i < list.size()-1) {
            outs << ", ";
        }
    }
    outs << "]";
    return outs;
}

template <typename T>
vector<T>& operator +=(vector<T>& list, const T& addme) { //list.push_back addme
    list.push_back(addme);
    return list;
}


#endif