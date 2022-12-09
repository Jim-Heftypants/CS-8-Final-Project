#ifndef MAP_H
#define MAP_H

#include "pair.h"
#include "bplustree.h"

template <typename K, typename V>
class Map {
public:
    typedef BPlusTree<Pair<K, V>> map_base;
    class Iterator{
    public:
        friend class Map;
        
        Iterator() {
            // cout << "default map iter called" << endl;
            _it = typename map_base::Iterator();
        }

        Iterator(typename map_base::Iterator it) {
            // cout << "made new map iter" << endl;
            _it = it;
            // cout << "new map iter is_null? " << _it.is_null() << endl;
        }
        Iterator operator ++(int unused) {
            _it++;
            return *this;
        }
        Iterator operator ++() {
            _it++;
            return *this;
        }
        Pair<K, V> operator *() {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }
        bool is_null(){return _it.is_null();}
    private:
        typename map_base::Iterator _it;
    };

    Map() {}
//  Iterators
    Iterator begin() {
        auto iter = map.begin();
        // cout << "is_null? " << iter.is_null() << endl;
        return Iterator(iter);
    }
    Iterator end() {
        return Iterator(map.end());
    }

//  Capacity
    int size() const {
        return map.size();
    }
    bool empty() const {
        return map.empty();
    }

//  Element Access
    V& operator[](const K& key) {
        return map.get(Pair<K, V>(key, V())).value;
    }
    V& at(const K& key) {
        return map.get_existing(Pair<K, V>(key, V())).value;
    }
    const V& at(const K& key) const {
        return map.get(Pair<K, V>(key, V())).value;
    }


//  Modifiers
    void insert(const K& k, const V& v) {
        map.insert(Pair<K, V>(k, v));
    }
    void erase(const K& key) {
        map.remove(Pair<K, V>(key, V()));
    }
    void clear() {
        map.clear_tree();
    }
    V get(const K& key) {
        Pair<K, V> foundPair = map.get(Pair<K, V>(key, V()));
        return foundPair.value;
    }

//  Operations:
    Iterator find(const K& key) {
        return map.find(Pair<K, V>(key, V()));
    }
    bool contains(const Pair<K, V>& target) const {
        return map.contains(target);
    }

    int count(const K& key) {
        auto iter = begin();
        if (iter == Iterator()) return -1;
        int count = 0;
        auto pair = Pair<K, V>(key, V());
        while (*iter != pair) {
            ++count;
        }
        return count;
    }
    Iterator lower_bound(const K& key) {
        return map.lower_bound(Pair<K, V>(key, V()));
    }
    Iterator upper_bound(const K& key) {
        return map.upper_bound(Pair<K, V>(key, V()));
    }
    void equal_range() {
        // ???
    }

    bool is_valid(){return map.is_valid();}

    friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }

    friend Map<K, V>& operator += (Map<K, V>& lhs, const Map<K, V>& rhs) {
        return lhs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V>> map;
};


#endif