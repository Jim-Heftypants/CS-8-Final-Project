#ifndef SORTS_H
#define SORTS_H

#include <vector>
#include <string.h>
#include <iostream>

using namespace std;

template <typename T>
vector<T> slice(vector<T>& arr, int X, int Y) { // Exclusive slice -> [ )
    auto start = arr.begin() + X;
    auto end = arr.begin() + Y;
    vector<T> result(Y - X);
    copy(start, end, result.begin());
    return result;
}

template <typename T>
vector<T> merge(vector<T> vec1, vector<T> vec2) {
    vector<T> vec3(vec1.size() + vec2.size());
    int ptr1 = 0;
    int ptr2 = 0;
    int ptr3 = 0;
    while (ptr1 < vec1.size() && ptr2 < vec2.size()) {
        if (vec1[ptr1] < vec2[ptr2]) vec3[ptr3++] = vec1[ptr1++];
        else vec3[ptr3++] = vec2[ptr2++];
    }
    while (ptr1 < vec1.size()) vec3[ptr3++] = vec1[ptr1++];
    while (ptr2 < vec2.size()) vec3[ptr3++] = vec2[ptr2++];
    return vec3;
}

template <typename T>
vector<T> mergeSort(vector<T> vec, int start, int length) {
    if (length < 2) {
        return vec;
    }
    int mid = (length+1)/2;
    auto left = mergeSort(vec, 0, mid);
    auto right = mergeSort(vec, mid, length - mid);
    return merge(left, right);
}

#endif