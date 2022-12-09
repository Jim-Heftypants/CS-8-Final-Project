#ifndef LINKED_LIST_FUNCTIONS_TEMPLATED_H
#define LINKED_LIST_FUNCTIONS_TEMPLATED_H

#include "../node/node.h"

//Linked List General Functions:
template <typename T>
void _print_list(node<T>* head) {
    std::cout << "printing linked list with head: " << *(head) << std::endl;
    auto current = head;
    while (current != NULL) {
        std::cout << *(current);
        current = current->_next;
    }
    std::cout << std::endl;
}

//recursive fun! :)
template <typename T>
void _print_list_backwards(node<T> *head) {
    auto current = head;
    while (current->_next != NULL) {
        current = current->_next;
    }
    while (current != NULL) {
        std::cout << current << ", ";
        current = current->prev;
    }
    std::cout << std::endl;
}

//return ptr to key or NULL
template <typename T>
node<T>* _search_list(node<T>* head, T key) {
    std::cout << "calling search list with head " << *(head) << " and key: " << key << std::endl;
    auto current = head;
    while (current->_item != key && current != NULL) {
        current = current->_next;
    }
    return current;
}


template <typename T>
node<T>* _insert_head(node<T> *&head, T insert_this) {
    // std::cout << "inserting head" << std::endl;
    // if (head != NULL) std::cout << "head node == " << head << " new value: " << insert_this << std::endl;
    node<T>* newHead = new node<T>(insert_this, head, NULL);
    if (head != NULL) head->_prev = newHead;
    head = newHead;
    return newHead;
}

//insert after ptr: insert head if marker null
template <typename T>
node<T>* _insert_after(node<T>*& head, node<T> *after_this, T insert_this) {
    if (after_this == NULL) return _insert_head(head, insert_this);
    auto nextNode = after_this->_next;
    node<T>* newNode = new node<T>(insert_this, nextNode, after_this);
    
    if (after_this != NULL) after_this->_next = newNode;
    if (nextNode != NULL) nextNode->_prev = newNode;

    return newNode;
}

//insert before ptr: insert head if marker null
template <typename T>
node<T>* _insert_before(node<T>*& head, node<T>* before_this, T insert_this) {
    if (before_this == NULL) return _insert_head(head, insert_this);
    auto prevNode = before_this->_prev;
    node<T>* newNode = new node<T>(insert_this, before_this, prevNode);
    
    prevNode->_next = newNode;
    before_this->_prev = newNode;

    return newNode;
}

//ptr to previous node
template <typename T>
node<T>* _previous_node(node<T>* prev_to_this) {
    return prev_to_this->_prev;
}

//delete, return item
template <typename T>
T _delete_node(node<T>*& head, node<T>* delete_this) {
    auto data = delete_this->_item;
    if (delete_this == NULL) {
        return NULL;
    }
    auto prior = delete_this->_prev;
    auto after = delete_this->_next;
    if (prior != NULL) {
        prior->_next = after;
    }
    if (after != NULL) {
        after->_prev = prior;
    }

    delete delete_this;

    return data;
}

//duplicate the list...
template <typename T>
node<T>* _copy_list(node<T>* head) {
    if (head == NULL) {
        return NULL;
    }
    auto current = head->_next;
    auto newHead = new node<T>(head->_item);
    auto copyCurrent = newHead;
    while (current != NULL) {
        auto newNode = new node<T>(current->_item);
        copyCurrent->_next = newNode;
        newNode->_prev = copyCurrent;
        current = current->_next;
        copyCurrent = copyCurrent->_next;
    }
    return newHead;
    
}

//duplicate the list, return pointer to last node in dest... 
//     use this function in your queue big three 
template <typename T>
node<T> *_copy_list(node<T> *&dest, node<T> *src) {
    if (dest == NULL) {
        dest = new node<T>();
    }
    if (src == NULL) {
        dest = NULL;
        return dest;
    }
    auto current = src->_next;
    dest->_item = src->_item;
    auto copyCurrent = dest;
    auto copyPrior = copyCurrent; // might be the error
    while (current != NULL) {
        auto newNode = new node<T>(current->_item);
        copyCurrent->_next = newNode;
        newNode->_prev = copyCurrent;

        copyPrior = copyCurrent;

        current = current->_next;
        copyCurrent = copyCurrent->_next;
    }
    copyPrior = copyPrior->_next;
    // std::cout << "copy prior after copy list: " << *copyPrior << std::endl;
    return copyPrior;
}

//delete all the nodes
template <typename T>
void _clear_list(node<T>*& head) {
    auto current = head;
    while (current != NULL) {
        auto next = current->_next;
        delete current;
        current = next;
    }
}

//_item at this position
template <typename T>
T& _at(node<T>* head, int pos) {
    if (pos < 1) return head;
    auto current = head;
    while (current != NULL) {
        current = current->_next;
        --pos;
        if (pos == 0) {
            return current;
        }
    }
    return NULL;
}

#endif