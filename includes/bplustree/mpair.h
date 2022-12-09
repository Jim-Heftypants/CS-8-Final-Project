#ifndef MPAIR_H
#define MPAIR_H

#include <vector>
#include "btree_array_funcs.h"

template <typename K, typename V>
struct MPair{
    K key;
    vector<V> value_list;

    //--------------------------------------------------------------------------------
    /*      MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()): key(k) {
        value_list = vector<V>(0);
    }
    MPair(const K& k, const V& v): key(k) {
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist): key(k), value_list(vlist) {}
    //--------------------------------------------------------------------------------

    //You'll need to overlod << for your vector:
    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me) {
        outs << "<" << print_me.key << ", " << print_me.value_list << ">";
        return outs;
    }
    
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        return lhs.key >= rhs.key;
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        auto newPair = MPair<K, V>();
        newPair.key = lhs.key;
        for (auto item : lhs.value_list) {
            newPair.value_list += item;
        }
        for (auto item : rhs.value_list) {
            newPair.value_list += item;
        }
        return newPair;
    }
    friend MPair<K, V>& operator += (MPair<K, V>& lhs, const MPair<K, V>& rhs) {
        // cout << "+= operator. lhs size: " << lhs.value_list.size() << " rhs size: " << rhs.value_list.size() << endl;;
        for (auto item : rhs.value_list) {
            lhs.value_list += item;
        }
        return lhs;
    }
};

#endif