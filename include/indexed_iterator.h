#ifndef INDEXED_ITERATOR_H
#define INDEXED_ITERATOR_H

#include <cstddef>

// allows begin/end sementics for [] operator
namespace IndexedIterator {
    template <typename T, typename reference>
    struct iterator {
        
        T* origin = nullptr;
        mutable std::size_t index = 0;

        iterator(T* origin, std::size_t index) : origin(origin), index(index) {}
        iterator(const iterator<T, reference> & o) : origin(o.origin), index(o.index) {}

        iterator operator ++(int) {
            iterator old(origin, index);
            index++;
            return old;
        }

        iterator operator --(int) {
            iterator old(origin, index);
            index--;
            return old;
        }
        
        iterator operator ++() {
            index++;
            return iterator(origin, index);
        }

        iterator operator --() {
            index--;
            return iterator(origin, index);
        }

        iterator operator +(std::size_t i) {
            return iterator <T, reference>(origin, index+i);
        }

        const iterator operator +(std::size_t i) const {
            return iterator <T, reference>(origin, index+i);
        }

        iterator operator -(std::size_t i) {
            return iterator <T, reference>(origin, index-i);
        }

        const iterator operator -(std::size_t i) const {
            return iterator <T, reference>(origin, index-i);
        }

        reference operator *() {
            return origin->operator[](index);
        }

        reference operator *() const {
            return origin->operator[](index);
        }

        const bool operator == (const iterator<T, reference> & other) const {
            return origin == other.origin && index == other.index;
        }

        const bool operator != (const iterator<T, reference> & other) const {
            return !(*this == other);
        }
    };

    template <typename T, typename reference>
    struct reverse_iterator {
        
        T* origin = nullptr;
        mutable std::size_t index = 0;

        reverse_iterator(T* origin, std::size_t index) : origin(origin), index(index) {}
        reverse_iterator(const reverse_iterator<T, reference> & o) : origin(o.origin), index(o.index) {}

        reverse_iterator operator ++(int) {
            reverse_iterator old(origin, index);
            index--;
            return old;
        }

        reverse_iterator operator --(int) {
            reverse_iterator old(origin, index);
            index++;
            return old;
        }
        
        reverse_iterator operator ++() {
            index--;
            return reverse_iterator(origin, index);
        }

        reverse_iterator operator --() {
            index++;
            return reverse_iterator(origin, index);
        }

        reverse_iterator operator +(std::size_t i) {
            return reverse_iterator <T, reference>(origin, index-i);
        }

        const reverse_iterator operator +(std::size_t i) const {
            return reverse_iterator <T, reference>(origin, index-i);
        }

        reverse_iterator operator -(std::size_t i) {
            return reverse_iterator <T, reference>(origin, index+i);
        }

        const reverse_iterator operator -(std::size_t i) const {
            return reverse_iterator <T, reference>(origin, index+i);
        }

        reference operator *() {
            return origin->operator[](index);
        }

        reference operator *() const {
            return origin->operator[](index);
        }

        const bool operator == (const reverse_iterator<T, reference> & other) const {
            return origin == other.origin && index == other.index;
        }

        const bool operator != (const reverse_iterator<T, reference> & other) const {
            return !(*this == other);
        }
    };
}

#endif