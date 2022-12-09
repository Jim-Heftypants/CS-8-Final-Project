#ifndef MY_STACK_H
#define MY_STACK_H

#include "../linked_list_functions/linkedlistfunctionsTemplated.h"

template <typename ITEM_TYPE>
class Stack{
public:
    class Iterator{
    public:
        friend class Stack;                 //give access to list to access _ptr
        Iterator(){_ptr = NULL;}            //default ctor
        Iterator(node<ITEM_TYPE>* p){ _ptr = p; }      //Point Iterator to where p is pointing to
        ITEM_TYPE operator *(){ return _ptr->_item; }    //dereference operator
        bool is_null(){return _ptr == NULL;}            //true if _ptr is NULL
        friend bool operator !=(const Iterator& left, const Iterator& right)  //true if left != right
        {return left._ptr != right._ptr;}

        friend bool operator ==(const Iterator& left, const Iterator& right)  //true if left == right
        {return left._ptr != right._ptr; }


        Iterator& operator++(){                        //member operator:++it or ++it = new_value
            _ptr = _ptr->_next;
            return *this;
        }

        friend Iterator operator++(Iterator& it, int unused) {        //friend operator: it++
            assert(it._ptr!=NULL);
            auto clone = Iterator(it._ptr);
            ++it;
            return clone;
        }

    private:
        node<ITEM_TYPE>* _ptr = NULL;    //pointer being encapsulated
    };

    Stack();
    Stack(const Stack<ITEM_TYPE>& copyMe);
    ~Stack();
    Stack<ITEM_TYPE>& operator=(const Stack<ITEM_TYPE>& RHS);
    ITEM_TYPE top();
    bool empty();
    void push(ITEM_TYPE item);
    ITEM_TYPE pop();
    template<typename T>
    friend std::ostream& operator<<(std::ostream& outs, const Stack<T>& printMe);
    Iterator begin() const;                   //Iterator to the head node
    Iterator end() const;                     //Iterator to NULL
    int size() const { return _size; }

private:
    node<ITEM_TYPE>* _top = NULL;
    int _size = 0;
};

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack() {}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::Stack(const Stack<ITEM_TYPE>& copyMe) {  // what about s = Stack<int>()
    if (this == &copyMe){
        return;
    }
    if (copyMe._top == NULL) {
        _top = NULL;
        _size = 0;
        return;
    }
    auto newTop = new node<ITEM_TYPE>();
    _top = newTop;
    _copy_list(newTop, copyMe._top);
    _size = copyMe._size;
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>::~Stack() {
    _clear_list(_top);
    _top = NULL;
    _size = 0;
}

template <typename ITEM_TYPE>
Stack<ITEM_TYPE>& Stack<ITEM_TYPE>::operator=(const Stack<ITEM_TYPE>& RHS) {
    std::cout << "stack = called" << std::endl;
    if (this == &RHS){
        return *this;
    }
    _clear_list(_top);
    if (RHS._top == NULL) {
        _top = NULL;
        _size = 0;
        return *this;
    }
    _top = _copy_list(RHS._top);
    _size = RHS._size;
    return *this;
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::top() {
    if (_top == NULL) return NULL;
    return _top->_item;
}

template <typename ITEM_TYPE>
bool Stack<ITEM_TYPE>::empty() {
    return _size == 0;
}

template <typename ITEM_TYPE>
void Stack<ITEM_TYPE>::push(ITEM_TYPE item) {
    ++_size;
    _top = _insert_head(_top, item);
}

template <typename ITEM_TYPE>
ITEM_TYPE Stack<ITEM_TYPE>::pop() {
    --_size;
    auto newTop = _top->_next;
    auto retItem = _delete_node(_top, _top);
    _top = newTop;
    return retItem;
}

template<typename T>
std::ostream& operator<<(std::ostream& outs, const Stack<T>& printMe) {
    // auto fIter = printMe.begin();
    // auto eIter = printMe.end();
    // while (fIter != eIter) {
    //     outs << *fIter;
    //     fIter++;
    // }
    // return outs;
    auto current = printMe._top;
    while (current != NULL) {
        outs << *(current);
        current = current->_next;
    }
    outs << std::endl;
    return outs;
}

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::begin() const {
    return Iterator(_top);
}                   //Iterator to the head node

template <typename ITEM_TYPE>
typename Stack<ITEM_TYPE>::Iterator Stack<ITEM_TYPE>::end() const {
    return Iterator();
}                     //Iterator to NULL

#endif