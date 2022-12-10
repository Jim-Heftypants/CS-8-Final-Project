#ifndef MULTIMAP_H
#define MULTIMAP_H

#include "mpair.h"
#include "bplustree.h"

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator() {
            _it = typename map_base::Iterator();
        }

        Iterator(typename map_base::Iterator it) {
            _it = it;
        }
        Iterator operator ++(int unused) {
            _it++;
            return *this;
        }
        Iterator operator ++() {
            _it++;
            return *this;
        }
        MPair<K, V> operator *() {
            return *_it;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it == rhs._it;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            return lhs._it != rhs._it;
        }
        void print_iterator() {
            _it.print_Iterator();
        }

    private:
        typename map_base::Iterator _it;
    };

    MMap() {}

//  Iterators
    Iterator begin() {
        return Iterator(mmap.begin());
    }
    Iterator end() {
        return Iterator(mmap.end());
    }

//  Capacity
    int size() const {
        return mmap.size();
    }
    bool empty() const {
        return mmap.empty();
    }

//  Element Access
    const vector<V>& operator[](const K& key) const {
        return mmap.get(MPair<K, V>(key)).value_list;
    }
    vector<V>& operator[](const K& key) {
        return mmap.get(MPair<K, V>(key)).value_list;
    }
    const vector<V>& at(const K& key) const {
        // cout << "MMap get with key: " << key << endl;
        return mmap.get(MPair<K, V>(key)).value_list;
    }

//  Modifiers
    void insert(const K& k, const V& v) {
        mmap.insert(MPair<K, V>(k, v));
    }
    void erase(const K& key) {
        mmap.remove(MPair<K, V>(key));
    }
    void clear() {
        mmap.clear_tree();
    }

//  Operations:
    bool contains(const K& key) const {
        return mmap.contains(MPair<K, V>(key));
    }
    vector<V> &get(const K& key) {
        return mmap.get(MPair<K, V>(key)).value_list;
    }

    Iterator find(const K& key) {
        return mmap.find(MPair<K, V>(key));
    }
    
    int count(const K& key) {
        auto iter = begin();
        if (iter == Iterator()) return -1;
        int count = 0;
        auto pair = MPair<K, V>(key);
        while (*iter != pair) {
            ++count;
        }
        return count;
    }
    Iterator lower_bound(const K& key) {
        return mmap.lower_bound(MPair<K, V>(key));
    }
    Iterator upper_bound(const K& key) {
        return mmap.upper_bound(MPair<K, V>(key));
    }
    void equal_range() {

    }

    bool is_valid(){return mmap.is_valid();}

    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }

    friend MMap<K, V>& operator += (MMap<K, V>& lhs, const MMap<K, V>& rhs) {
        return lhs;
    }

    void print_lookup() {
        cout << "Parse Table:" << endl;
        for (auto it = begin(); it != end(); it++) {
            cout << *it << endl;
        }
        cout << endl;
    }

private:
    BPlusTree<MPair<K, V>> mmap = BPlusTree<MPair<K, V>>(true);
};

#endif