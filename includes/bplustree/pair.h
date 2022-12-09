#ifndef PAIR_H
#define PAIR_H

#include <iostream>

using namespace std;

template <typename K, typename V>
struct Pair{
    K key;
    V value;

    Pair(const K& k=K(), const V& v=V()): key(k), value(v) {}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me) {
        outs << "<" << print_me.key << ", " << print_me.value << ">";
        return outs;
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key == rhs.key;
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key < rhs.key;
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key > rhs.key;
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key <= rhs.key;
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        return lhs.key >= rhs.key;
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        auto newPair = Pair<K, V>();
        newPair.value = lhs.value + rhs.value;
        newPair.key = lhs.key;
        return newPair;
    }
    friend Pair<K, V>& operator += (Pair<K, V>& lhs, Pair<K, V>& rhs) {
        lhs.value += rhs.value;
        return lhs;
    }
    friend Pair<K, V>& operator += (Pair<K, V>& lhs, const Pair<K, V>& rhs) {
        lhs.value += rhs.value;
        return lhs;
    }
};

#endif