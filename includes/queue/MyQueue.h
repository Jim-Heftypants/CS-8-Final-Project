#ifndef MY_QUEUE_H
#define MY_QUEUE_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename T>
class Queue {
public:
    class Iterator {
    public:
        friend class Queue;                               //give access to list to access _ptr
        Iterator(){_ptr = NULL;}                          //default ctor
        Iterator(node<T>* p){ _ptr = p;}                  //Point Iterator to where p is pointing to
        T operator *() {    // dereference operator
            if (_ptr == NULL) return NULL;
            return _ptr->_item;
        }
        bool is_null(){ return _ptr == NULL; }            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right) {
            return left._ptr != right._ptr;
        } //true if left != right
        friend bool operator ==(const Iterator& left, const Iterator& right) { return left._ptr == right._ptr; } //true if left == right
        Iterator& operator++() {
            assert(this->_ptr!=NULL);
            _ptr = _ptr->_next;
            return *this;
        }                         //member operator:++it or ++it = new_value

        friend Iterator operator++(Iterator& it, int unused){         //friend operator: it++
            assert(it._ptr!=NULL);
            auto clone = Iterator(it._ptr);
            ++it;
            return clone;
        }

    private:
        node<T>* _ptr = NULL;                          //pointer being encapsulated
    };

    Queue();

    Queue(const Queue<T>& copyMe);
    ~Queue();
    Queue& operator=(const Queue<T>& RHS);

    bool empty();
    T front();
    T back();

    void push(T item);
    T pop();

    Iterator begin() const;                                     //Iterator to the head node
    Iterator end() const;                                       //Iterator to NULL
    void print_pointers();
    int size() const { return _size; }
    template<typename TT> friend std::ostream& operator << (std::ostream& outs, const Queue<TT>& printMe);
private:
    node<T>* _front = NULL;
    node<T>* _rear = NULL;
    int _size = 0;
};

template <typename T>
Queue<T>::Queue() {}

template <typename T>
Queue<T>::Queue(const Queue<T>& copyMe) {
    if (this == &copyMe){
        return;
    }
    if (copyMe._front == NULL) {
        _front = NULL;
        _rear = NULL;
        _size = 0;
        return;
    }
    node<T>* newFront = new node<T>();
    _front = newFront;
    _rear = _copy_list(newFront, copyMe._front);
    _size = copyMe._size;
}

template <typename T>
Queue<T>::~Queue() {
    _clear_list(_front);
    _front = NULL;
    _rear = NULL;
    _size = 0;
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue<T>& RHS) {
    // std::cout << "queue = called" << std::endl;
    if (this == &RHS){
        return *this;
    }
    _clear_list(_front);
    if (RHS._front == NULL) {
        _front = NULL;
        _rear = NULL;
        _size = 0;
        return *this;
    }
    node<T>* newFront = new node<T>();
    _front = newFront;
    _rear = _copy_list(newFront, RHS._front);
    _size = RHS._size;
    return *this;
}

template <typename T>
bool Queue<T>::empty() {
    return _size == 0;
}

template <typename T>
T Queue<T>::front() {
    if (_front == NULL) return NULL;
    return _front->_item;
}
template <typename T>
T Queue<T>::back() {
    if (_rear == NULL) return NULL;
    return _rear->_item;
}

template <typename T>
void Queue<T>::push(T item) {
    _rear = _insert_after(_front, _rear, item);
    _size++;
    if (_size == 1) _front = _rear;
}
template <typename T>
T Queue<T>::pop() {
    if (_size == 0) return T();
    if (_size == 1) _rear = NULL;
    this->_size--;
    auto oldFront = this->_front;
    this->_front = this->_front->_next;
    return _delete_node(oldFront, oldFront); // what if there is only one node?
}

template <typename T>
typename Queue<T>::Iterator Queue<T>::begin() const {     //Iterator to the head node
    return Iterator(this->_front);
}            

template <typename T>
typename Queue<T>::Iterator Queue<T>::end() const {       //Iterator to NULL
    return Iterator();
}

template <typename T>
void Queue<T>::print_pointers() {

}

template<typename TT>
std::ostream& operator<< (std::ostream& outs, const Queue<TT>& printMe) {
    // auto fIter = printMe.begin();
    // auto eIter = printMe.end();
    // while (fIter != eIter) {
    //     outs << *fIter;
    //     fIter++;
    // }
    auto current = printMe._front;
    while (current != NULL) {
        outs << *(current);
        current = current->_next;
    }
    outs << std::endl;
    return outs;
}

#endif