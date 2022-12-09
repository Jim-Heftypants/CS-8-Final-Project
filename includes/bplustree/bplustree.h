#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "btree_array_funcs.h"

template <class T>
class BPlusTree {
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(){
            //  cout<<"btree default ctor"<<endl;
             it = nullptr;
             key_ptr = 0;
        }

        Iterator(BPlusTree<T>* _it, int _key_ptr): it(_it), key_ptr(_key_ptr){
            // cout << "made new btree iterator with _it = " << it << endl;
        }

        T operator *() {
            return it->data[key_ptr];
        }

        Iterator operator++(int un_used) {
            ++key_ptr;
            if (it->data_count <= key_ptr) {
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }
        Iterator operator++() {
            ++key_ptr;
            if (it->data_count <= key_ptr) {
                key_ptr = 0;
                it = it->next;
            }
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs) {
            // return *lhs == *rhs;
            // cout << "lhs ptr: " << lhs.it << " rhs ptr: " << rhs.it << endl;
            return lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs) {
            // return *lhs != *rhs;
            return lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr;
        }
        void print_Iterator(){
            if (it){
                cout<<"iterator: ";
                print_array(it->data, it->data_count, key_ptr);
            }
            else{
                cout<<"iterator: NULL, key_ptr: "<<key_ptr<<endl;
            }
        }
        bool is_null(){return it == NULL;}
        void info(){
            cout<<endl<<"Iterator info:"<<endl;
            cout<<"key_ptr: "<<key_ptr<<endl;
            cout<<"it: "<<*it<<endl;
        }

    private:
        BPlusTree<T>* it;
        int key_ptr;
    };

    BPlusTree() : dups_ok(false), data_count(0), child_count(0) {
        // cout << "BTree default constructor called with dups: " << dups_ok << endl;
    }
    BPlusTree(bool dups) : dups_ok(dups), data_count(0), child_count(0) {
        // cout << "BTree dups constructor called with dups: " << dups_ok << endl;
    }
    BPlusTree(T *a, int size, bool dups = false) : BPlusTree(dups) {
        for (int i = 0; i < size; ++i) {
            // cout << "inserting " << a[i] << endl;
            insert(a[i]);
        }
    }

    //big three:
    BPlusTree(const BPlusTree<T>& other) {
        data_count = 0;
        child_count = 0;
        dups_ok = other.dups_ok;
        copy_tree(other);
    }
    ~BPlusTree() {
        for (int i = 0; i < child_count; ++i) {
            delete subset[i];
        }
    }
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS) {
        data_count = 0;
        child_count = 0;
        dups_ok = RHS.dups_ok;
        copy_tree(RHS);
    }

    void copy_tree(const BPlusTree<T>& other) {  //copy other into this object
        if (!other.is_leaf()) {
            for (int i = 0; i < other.child_count; ++i) {
                copy_tree(*(other.subset[i]));
            }
        } else {
            for (int i = 0; i < other.data_count; ++i) {
                insert(other.data[i]);
            }
        }
    }
    void copy_tree(const BPlusTree<T>& other, BPlusTree<T>*& last_node) {

    }

    void insert(const T& entry) {   //insert entry into the tree
        // cout << "inserting " << entry << endl;
        // cout << *this << endl;
        loose_insert(entry);
        if (data_count > MAXIMUM) {
            // cout << "Growing tree from root" << endl;
            // cout << "Pre grow tree:" << endl << *this << endl;

            BPlusTree<T>* temp = new BPlusTree<T>;
            copy_array(temp->data, data, temp->data_count, data_count);
            copy_array(temp->subset, subset, temp->child_count, child_count);
            subset[0] = temp;
            temp->dups_ok = dups_ok;
            data_count = 0;
            child_count = 1;
            // cout << "Pre fix excess:" << endl << *this << endl;
            fix_excess(0);

            // cout << "Post grow tree:" << endl << *this << endl;
        }
        // cout << *this << endl;
    }
    void remove(const T& entry) {   //remove entry from the tree
        // cout << "Deleting " << entry << endl;
        loose_remove(entry);
        if (data_count < 1 && child_count == 1) {
            // cout << "Shrinking BPlusTree Root" << endl;
            BPlusTree<T>* ptr = subset[0];
            copy_array(data, ptr->data, data_count, ptr->data_count);
            copy_array(subset, ptr->subset, child_count, ptr->child_count);
            ptr->child_count = 0;

            delete ptr;
        }
    }
    void clear_tree() {             //clear this object (delete all nodes etc.)
        data_count = 0;
        BPlusTree<T>* tmp;
        for (int i = 0; i < child_count; ++i) {
            tmp = subset[i];
            tmp->clear_tree();
            delete tmp;
        }
        child_count = 0;
    }

    bool contains(const T& key) const { //true if entry can be found
        auto i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i] == key;
        if (found) {
            return true;
        } else if (is_leaf()) {
            return false;
        } else {
            return subset[i]->contains(key);
        }
    }
    T& get(const T& entry) {              //return a reference to entry
        if (!contains(entry)) insert(entry);
        return get_existing(entry);
    }
    const T& get(const T& entry)const {   //return a reference to entry
        auto i = first_ge(data, data_count, entry);
        bool found = i<data_count && data[i] == entry;
        if (found && is_leaf()) {
            return data[i];
        } else if (!found && is_leaf()) {
            return T();
        } else if (found) {
            return subset[i+1]->get_existing(entry);
        } else {
            return subset[i]->get_existing(entry);
        }
    }
    T& get_existing(const T& entry) {     //return a reference to entry
        auto i = first_ge(data, data_count, entry);
        bool found = i<data_count && data[i] == entry;
        if (found && is_leaf()) {
            return data[i];
        } else if (found) {
            return subset[i+1]->get_existing(entry);
        } else {
            return subset[i]->get_existing(entry);
        }
    }
    Iterator find(const T& key) {         //return an iterator to this key. NULL if not there.
        auto i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i] == key;
        if (found && is_leaf()) {
            return Iterator(this, i);
        } else if (!found && is_leaf()) {
            return Iterator();
        } else if (found) {
            return subset[i+1]->find(key);
        } else {
            return subset[i]->find(key);
        }
    }
    Iterator lower_bound(const T& key) {  //return first that goes NOT BEFORE key entry or next if does not exist: >= entry
        int i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i] == key;
        if (found && is_leaf()) {
            return Iterator(this, i);
        } else if (!found && is_leaf()) {
            if (i >= data_count) {
                return Iterator(next, 0);
            }
            if (i > 0 && data[i-1] > key) {
                return Iterator(this, i-1);
            }
            return Iterator(this, i);
        } else if (found) {
            return subset[i+1]->lower_bound(key);
        } else {
            return subset[i]->lower_bound(key);
        }
    }
    Iterator upper_bound(const T& key) {  //return first that goes AFTER key exist or not, the next entry  >entry
        auto i = first_ge(data, data_count, key);
        bool found = i<data_count && data[i] == key;
        if (!found && is_leaf()) {
            if (i >= data_count) {
                return Iterator(next, 0);
            }
            if (i > 0 && data[i-1] > key) {
                return Iterator(this, i-1);
            }
            return Iterator(this, i);
        } else if (found && is_leaf()) {
            if (i < data_count-1) {
                return Iterator(this, i+1);
            }
            return Iterator(next, 0);
        } else if (found) {
            return subset[i+1]->upper_bound(key);
        } else {
            return subset[i]->upper_bound(key);
        }
    }

    int size() const {                    //count the number of elements
        int count = 0;
        count += data_count;
        for (int i = 0; i < child_count; ++i) {
            count += subset[i]->size();
        }
        return count;
    }

    bool empty() const {                  //true if the tree is empty
        return data_count == 0;
    }

    void print_tree(int level = 0, ostream &outs=cout) const {
        for (int j = 0; j < level; ++j) {
            outs << '\t';
        }
        outs << "^" << " next: " << next << endl;
        for (int z = data_count-1; z > -1; --z) {
            if (child_count > z+1) subset[z+1]->print_tree(level+1, outs);
            for (int j = 0; j < level; ++j) {
                outs << '\t';
            }
            outs << data[z] << endl;
        }
        if (child_count > 0) subset[0]->print_tree(level+1, outs);
        for (int j = 0; j < level; ++j) {
            outs << '\t';
        }
        outs << "_" << endl;
    }
    friend ostream& operator<<(ostream& outs, const BPlusTree<T>& print_me) {
        print_me.print_tree(0, outs);
        return outs;
    }

    bool is_valid() {
        return true;
    }
    string in_order() {
        string str = "";
        if (child_count > 0) {
            str += subset[0]->in_order();
        }
        for (int z = 0; z < data_count; ++z) {
            string tmp = to_string(data[z]);
            str += tmp + "|";
            if (child_count > z+1) {
                str += subset[z+1]->in_order();
            }
        }
        return str;
    }

    string pre_order() {
        // print order:
        // do first data, then first child, etc
        // end on last data + 1 child
        string str = "";
        int z;
        for (z = 0; z < data_count; ++z) {
            string tmp = to_string(data[z]);
            str += tmp + "|";
            if (z < child_count) {
                str += subset[z]->pre_order();
            }
        }
        if (z < child_count) {
            str += subset[z]->pre_order();
        } 
        return str;
    }

    string post_order() {
        string str = "";
        int z;
        if (child_count > 0) {
            str += subset[0]->post_order();
        }
        for (z = 0; z < data_count; ++z) {
            if (z+1 < child_count) {
                str += subset[z+1]->post_order();
            }
            string tmp = to_string(data[z]);
            str += tmp + "|";
        }
        return str;
    }

    Iterator begin() {
        if (is_leaf()) {
            return Iterator(this, 0);
        } else {
            return subset[0]->begin();
        }
    }
    Iterator end() {
        return Iterator();
    }
    ostream& list_keys(Iterator from = NULL, Iterator to = NULL){
        if (from == NULL) from = begin();
        if (to == NULL) to = end();
        for (Iterator it = from; it != to; it++)
            cout<<*it<<" ";
        return cout;

    }

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool dups_ok;                        //true if duplicate keys are allowed
    int data_count;                      //number of data elements
    T data[MAXIMUM + 1];                 //holds the keys
    int child_count;                     //number of children
    BPlusTree* subset[MAXIMUM + 2];      //subtrees
    BPlusTree* next = NULL;
    bool is_leaf() const {return child_count==0;}             //true if this is a leaf node

    T* find_ptr(const T& entry) {         //return a pointer to this key. NULL if not there.

    }

    //insert element functions
    void loose_insert(const T& entry) {   //allows MAXIMUM+1 data elements in the root
        // cout << "dups: " << dups_ok << endl;
        int i = first_ge(data, data_count, entry);
        bool found = (i<data_count && data[i] == entry);
        if (found && is_leaf()) {
            if (dups_ok) data[i] += entry;
            else data[i] = entry;
            return;
        }
        if (found) {
            if (dups_ok) {
                data[i] += entry;
            }
            subset[i+1]->loose_insert(entry);
            fix_excess(i+1);
            return;
        }
        if (is_leaf()) {
            // cout << "adding entry " << entry << " to leaf node" << endl;
            ordered_insert(data, data_count, entry);
            return;
        }
        // cout << "loose inserting item into child tree" << endl;
        subset[i]->loose_insert(entry);
        fix_excess(i);
    }
    void fix_excess(int i) {              //fix excess in child i
        if (subset[i]->data_count < MAXIMUM+1) return;

        auto newTree = new BPlusTree<T>();
        newTree->dups_ok = dups_ok;
        insert_item(subset, i+1, child_count, newTree);

        split(subset[i]->data, subset[i]->data_count, subset[i+1]->data, subset[i+1]->data_count);
        split(subset[i]->subset, subset[i]->child_count, subset[i+1]->subset, subset[i+1]->child_count);
        
        subset[i]->next = subset[i+1];
        if (i+2 < child_count) {
            subset[i+1]->next = subset[i+2];
        }
        

        T item;
        detach_item(subset[i]->data, subset[i]->data_count, item);
        ordered_insert(data, data_count, item);
        if (subset[i]->is_leaf()) {
            ordered_insert(subset[i+1]->data, subset[i+1]->data_count, item);
        }
    }

    //remove element functions:
    void loose_remove(const T& entry) {  //allows MINIMUM-1 data elements in the root
        auto i = first_ge(data, data_count, entry);
        bool found = i<data_count && data[i] == entry;
        if (!found && is_leaf()) return;
        if (is_leaf()) {
            // cout << "Deleting leaf item" << endl;
            T item;
            delete_item(data, i, data_count, item);
            return;
        }
        if (!found) {
            subset[i]->loose_remove(entry);
            fix_shortage(i);
            return;
        }
        // cout << "Deleting inner tree item" << endl;
        // cout << "Deletion sub tree: " << endl << *this << endl;
        subset[i+1]->loose_remove(entry);
        fix_shortage(i+1);
        // cout << "Mid-tree" << *this << endl;
        // need to remove entry from either data or subset[i+1]->data
        int j = first_ge(data, data_count, entry);
        // cout << "first ge 1: " << j << endl;
        T item;
        if (j >= data_count && i+1 < child_count) {
            j = first_ge(subset[i+1]->data, subset[i+1]->data_count, entry);
            // cout << "first ge 2: " << j << endl;
            if (j >= subset[i+1]->data_count) {
                cout << "Inner item has already been deleted" << endl;
                return;
            }
            subset[i+1]->subset[i+1]->get_smallest(item);
            subset[i+1]->data[j] = item;
        } else {
            subset[i+1]->get_smallest(item);
            data[j] = item;
        }
        // cout << "Item: " << item << endl;
        // cout << "Post deletion sub tree: " << endl << *this << endl;
    }

    BPlusTree<T>* fix_shortage(int i) {  //fix shortage in child i and return the smallest key in this subtree
        if (subset[i]->data_count >= MINIMUM) return NULL;
        if (i < (child_count-1) && subset[i+1]->data_count > MINIMUM) {
            cout << "Rotating left" << endl;
            transfer_left(i);
        } else if (i > 0 && subset[i-1]->data_count > MINIMUM) {
            cout << "Rotating right" << endl;
            transfer_right(i);
        } else if (i < (child_count-1)) {
            cout << "Merging left" << endl;
            merge_with_next_subset(i);
        } else {
            cout << "Merging right" << endl;
            merge_with_next_subset(i-1);
        }

        if ((i<child_count-1) && (subset[i+1]->data_count > MINIMUM)){
            //borrow from right subtree if you can
            cout << "borrowing extra from right subtree" << endl;
            transfer_left(i);
            return subset[i];
        }
        return NULL;
    }
    BPlusTree<T>* get_smallest_node() {
        if (is_leaf()) return NULL;
        return subset[0];
    }
    void get_smallest(T& entry) {      //entry := leftmost leaf
        if (is_leaf()) {
            entry = data[0];
            return;
        }
        subset[0]->get_smallest(entry);
        // fix_shortage(0);
    }
    void get_biggest(T& entry) {       //entry := rightmost leaf

    }
    void remove_biggest(T& entry) {    //remove the biggest child of tree->entry
        if (is_leaf()) {
            int i = index_of_maximal(data, data_count);
            delete_item(data, i, data_count, entry);
            return;
        }
        subset[child_count-1]->remove_biggest(entry);
        fix_shortage(child_count-1);
    }
    void transfer_left(int i) {        //transfer one element LEFT from child i + 1
        if (i < 0 || i >= child_count - 1 || subset[i+1]->data_count <= MINIMUM) return;
        T item;
        if (subset[i]->is_leaf()) {
            delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
            ordered_insert(subset[i]->data, subset[i]->data_count, item);
        } else {
            delete_item(data, i, data_count, item);
            ordered_insert(subset[i]->data, subset[i]->data_count, item);
            delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
            ordered_insert(data, data_count, item);
        }

        // attach children
        BPlusTree<T>* subSubset;
        delete_item(subset[i+1]->subset, 0, subset[i+1]->child_count, subSubset);
        if (subSubset != NULL) {
            attach_item(subset[i]->subset, subset[i]->child_count, subSubset);
        }
        if (subset[i+1]->is_leaf()) {
            data[i] = subset[i+1]->data[0];
        }
    }
    void transfer_right(int i) {       //transfer one element RIGHT from child i-1
        if (i < 1 || i >= child_count || subset[i-1]->data_count <= MINIMUM) return;

        T item;
        if (subset[i]->is_leaf()) {
            delete_item(subset[i-1]->data, subset[i-1]->data_count-1, subset[i-1]->data_count, item);
            ordered_insert(subset[i]->data, subset[i]->data_count, item);
        } else {
            delete_item(data, i-1, data_count, item);
            ordered_insert(subset[i]->data, subset[i]->data_count, item);
            delete_item(subset[i-1]->data, subset[i-1]->data_count-1, subset[i-1]->data_count, item);
            ordered_insert(data, data_count, item);
        }

        // attach children
        BPlusTree<T>* subSubset;
        delete_item(subset[i-1]->subset, subset[i-1]->child_count-1, subset[i-1]->child_count, subSubset);
        if (subSubset != NULL) {
            cout << "inserting item" << endl;
            insert_item(subset[i]->subset, 0, subset[i]->child_count, subSubset);
        }
        if (subset[i]->is_leaf()) {
            data[i-1] = subset[i]->data[0];
        }
    }
    BPlusTree<T> *merge_with_next_subset(int i) { //merge subset i with  i+1
        T item;
        delete_item(data, i, data_count, item);
        if (!subset[i]->is_leaf() && item != NULL) {
            attach_item(subset[i]->data, subset[i]->data_count, item);
        }
        // moves all data from right to left
        int temp = subset[i+1]->data_count;
        for (int j = 0; j < temp; ++j) {
            delete_item(subset[i+1]->data, 0, subset[i+1]->data_count, item);
            attach_item(subset[i]->data, subset[i]->data_count, item);
        }
        // moves all children from right to left
        BPlusTree<T>* subSubset;
        temp = subset[i+1]->child_count;
        for (int j = 0; j < temp; j++) {
            delete_item(subset[i+1]->subset, 0, subset[i+1]->child_count, subSubset);
            attach_item(subset[i]->subset, subset[i]->child_count, subSubset);
        }
        delete_item(subset, i+1, child_count, subSubset);
        delete subSubset;
        if (child_count > i+1) 
            subset[i]->next = subset[i+1];
        else
            subset[i]->next = NULL;

        // cout << "Post Merge" << *this << endl;
        
        return subset[i];
    }

};

#endif