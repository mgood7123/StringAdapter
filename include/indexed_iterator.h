#ifndef INDEXED_ITERATOR_H
#define INDEXED_ITERATOR_H

#include <cstddef>

// allows begin/end sementics for [] operator
namespace IndexedIterator {

    struct iterator_base {
        virtual const void* get_origin() const = 0;
        virtual const std::size_t get_index() const = 0;
        virtual const bool operator == (const iterator_base & other) const = 0;
        virtual const bool operator != (const iterator_base & other) const = 0;
    };

    template <typename T, typename reference>
    struct iterator : iterator_base {
        
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

        reference & operator *() {
            return origin->operator[](index);
        }

        reference & operator *() const {
            return origin->operator[](index);
        }

        reference* operator -> () {
            return &origin->operator[](index);
        }

        reference* operator -> () const {
            return &origin->operator[](index);
        }

        const void* get_origin() const override {
            return origin;
        }

        const std::size_t get_index() const override {
            return index;
        }

        const bool operator == (const iterator_base & other) const override {
            return get_origin() == other.get_origin() && get_index() == other.get_index();
        }

        const bool operator != (const iterator_base & other) const override {
            return !(*this == other);
        }
    };

    template <typename T, typename reference>
    struct reverse_iterator : iterator_base {
        
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

        reference & operator *() {
            return origin->operator[](index);
        }

        reference & operator *() const {
            return origin->operator[](index);
        }

        reference* operator -> () {
            return &origin->operator[](index);
        }

        reference* operator -> () const {
            return &origin->operator[](index);
        }
        
        const void* get_origin() const override {
            return origin;
        }

        const std::size_t get_index() const override {
            return index;
        }

        const bool operator == (const iterator_base & other) const override {
            return get_origin() == other.get_origin() && get_index() == other.get_index();
        }

        const bool operator != (const iterator_base & other) const override {
            return !(*this == other);
        }
    };

    template <typename T>
    struct ptr_iterator : iterator_base {
        
        T* origin = nullptr;
        mutable std::size_t index = 0;

        ptr_iterator(T* origin, std::size_t index) : origin(origin), index(index) {}
        ptr_iterator(const ptr_iterator<T> & o) : origin(o.origin), index(o.index) {}

        ptr_iterator operator ++(int) {
            ptr_iterator old(origin, index);
            index++;
            return old;
        }

        ptr_iterator operator --(int) {
            ptr_iterator old(origin, index);
            index--;
            return old;
        }
        
        ptr_iterator operator ++() {
            index++;
            return ptr_iterator(origin, index);
        }

        ptr_iterator operator --() {
            index--;
            return ptr_iterator(origin, index);
        }

        ptr_iterator operator +(std::size_t i) {
            return ptr_iterator <T>(origin, index+i);
        }

        const ptr_iterator operator +(std::size_t i) const {
            return ptr_iterator <T>(origin, index+i);
        }

        ptr_iterator operator -(std::size_t i) {
            return ptr_iterator <T>(origin, index-i);
        }

        const ptr_iterator operator -(std::size_t i) const {
            return ptr_iterator <T>(origin, index-i);
        }

        T & operator *() {
            return origin[index];
        }

        T & operator *() const {
            return origin[index];
        }

        T* operator -> () {
            return &origin[index];
        }

        T* operator -> () const {
            return &origin[index];
        }

        const void* get_origin() const override {
            return origin;
        }

        const std::size_t get_index() const override {
            return index;
        }

        const bool operator == (const iterator_base & other) const override {
            return get_origin() == other.get_origin() && get_index() == other.get_index();
        }

        const bool operator != (const iterator_base & other) const override {
            return !(*this == other);
        }
    };

    template <typename T>
    struct reverse_ptr_iterator : iterator_base {
        
        T* origin = nullptr;
        mutable std::size_t index = 0;

        reverse_ptr_iterator(T* origin, std::size_t index) : origin(origin), index(index) {}
        reverse_ptr_iterator(const reverse_ptr_iterator<T> & o) : origin(o.origin), index(o.index) {}

        reverse_ptr_iterator operator ++(int) {
            reverse_ptr_iterator old(origin, index);
            index--;
            return old;
        }

        reverse_ptr_iterator operator --(int) {
            reverse_ptr_iterator old(origin, index);
            index++;
            return old;
        }
        
        reverse_ptr_iterator operator ++() {
            index--;
            return reverse_ptr_iterator(origin, index);
        }

        reverse_ptr_iterator operator --() {
            index++;
            return reverse_ptr_iterator(origin, index);
        }

        reverse_ptr_iterator operator +(std::size_t i) {
            return reverse_ptr_iterator <T>(origin, index-i);
        }

        const reverse_ptr_iterator operator +(std::size_t i) const {
            return reverse_ptr_iterator <T>(origin, index-i);
        }

        reverse_ptr_iterator operator -(std::size_t i) {
            return reverse_ptr_iterator <T>(origin, index+i);
        }

        const reverse_ptr_iterator operator -(std::size_t i) const {
            return reverse_ptr_iterator <T>(origin, index+i);
        }

        T & operator *() {
            return origin[index];
        }

        T & operator *() const {
            return origin[index];
        }

        T* operator -> () {
            return &origin[index];
        }

        T* operator -> () const {
            return &origin[index];
        }
        
        const void* get_origin() const override {
            return origin;
        }

        const std::size_t get_index() const override {
            return index;
        }

        const bool operator == (const iterator_base & other) const override {
            return get_origin() == other.get_origin() && get_index() == other.get_index();
        }

        const bool operator != (const iterator_base & other) const override {
            return !(*this == other);
        }
    };
}

#endif