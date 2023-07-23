#ifndef INDEXED_ITERATOR_H
#define INDEXED_ITERATOR_H

#include <cstddef>
#include <iostream>
#include <iterator>

#include <stdexcept>
#include <utility>

#include <libobj.h>

#define INDEXED_ITERATOR_EMBED_COMMAS(...) __VA_ARGS__

#define INDEXED_ITERATOR_OPS(iterator, op_plus, op_minus, index_operator)      \
    iterator operator++(int) {                                                 \
        iterator old(origin, index);                                           \
        index op_plus##op_plus;                                                \
        return old;                                                            \
    }                                                                          \
                                                                               \
    iterator operator++(int) const {                                           \
        iterator old(origin, index);                                           \
        index op_plus##op_plus;                                                \
        return old;                                                            \
    }                                                                          \
                                                                               \
    iterator operator--(int) {                                                 \
        iterator old(origin, index);                                           \
        index op_minus##op_minus;                                              \
        return old;                                                            \
    }                                                                          \
                                                                               \
    iterator operator--(int) const {                                           \
        iterator old(origin, index);                                           \
        index op_minus##op_minus;                                              \
        return old;                                                            \
    }                                                                          \
                                                                               \
    iterator operator++() {                                                    \
        index op_plus##op_plus;                                                \
        return iterator(origin, index);                                        \
    }                                                                          \
                                                                               \
    iterator operator++() const {                                              \
        index op_plus##op_plus;                                                \
        return iterator(origin, index);                                        \
    }                                                                          \
                                                                               \
    iterator operator--() {                                                    \
        index op_minus##op_minus;                                              \
        return iterator(origin, index);                                        \
    }                                                                          \
                                                                               \
    iterator operator--() const {                                              \
        index op_minus##op_minus;                                              \
        return iterator(origin, index);                                        \
    }                                                                          \
                                                                               \
    iterator operator+(difference_type i) {                                    \
        return iterator(origin, index op_plus i);                              \
    }                                                                          \
                                                                               \
    const iterator operator+(difference_type i) const {                        \
        return iterator(origin, index op_plus i);                              \
    }                                                                          \
                                                                               \
    iterator operator-(difference_type i) {                                    \
        return iterator(origin, index op_minus i);                             \
    }                                                                          \
                                                                               \
    const iterator operator-(difference_type i) const {                        \
        return iterator(origin, index op_minus i);                             \
    }                                                                          \
                                                                               \
    iterator & operator+=(difference_type i) {                                 \
        index op_plus## = i;                                                   \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    const iterator & operator+=(difference_type i) const {                     \
        index op_plus## = i;                                                   \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    iterator & operator-=(difference_type i) {                                 \
        index op_minus## = i;                                                  \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    const iterator & operator-=(difference_type i) const {                     \
        index op_minus## = i;                                                  \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    iterator & operator+=(const iterator_base_obj & other) {                   \
        index op_plus## = other.get_index();                                   \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    const iterator & operator+=(const iterator_base_obj & other) const {       \
        index op_plus## = other.get_index();                                   \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    iterator & operator-=(const iterator_base_obj & other) {                   \
        index op_minus## = other.get_index();                                  \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    const iterator & operator-=(const iterator_base_obj & other) const {       \
        index op_minus## = other.get_index();                                  \
        return *this;                                                          \
    }                                                                          \
                                                                               \
    reference operator*() {                                                    \
        return origin index_operator;                                          \
    }                                                                          \
                                                                               \
    reference operator*() const {                                              \
        return origin index_operator;                                          \
    }                                                                          \
                                                                               \
    pointer operator->() {                                                     \
        return &origin index_operator;                                         \
    }                                                                          \
                                                                               \
    pointer operator->() const {                                               \
        return &origin index_operator;                                         \
    }                                                                          \
                                                                               \
    const void * get_origin() const override {                                 \
        return origin;                                                         \
    }                                                                          \
                                                                               \
    const difference_type get_index() const override {                         \
        return index;                                                          \
    }                                                                          \
                                                                               \
    difference_type op_plus_(const difference_type & other) const override {   \
        return index op_plus other;                                            \
    }                                                                          \
                                                                               \
    difference_type op_minus_(const difference_type & other) const override {  \
        return index op_minus other;                                           \
    }

#define INDEXED_ITERATOR_DEFINE(template_definition, template_usage,           \
                                ORIGIN_TYPE, VALUE_TYPE, iterator, iterator_base_obj,       \
                                op_plus, op_minus, index_operator)             \
    template template_definition struct iterator : public iterator_base_obj {  \
                                                                               \
            using iterator_category = std::random_access_iterator_tag;         \
            using value_type = VALUE_TYPE;                                     \
            using pointer = VALUE_TYPE *;                                      \
            using reference = VALUE_TYPE &;                                    \
            using O_TYPE = ORIGIN_TYPE;                                        \
                                                                               \
            mutable ORIGIN_TYPE * origin = nullptr;                            \
            mutable difference_type index = 0;                                 \
                                                                               \
            std::ostream & toStream(std::ostream & os) const override {                                          \
                return os << "{ " << &iterator_base_obj::toStream              \
                          << ", index = " << std::to_string(index)             \
                          << ", value = " << origin index_operator << " }";    \
            }                                                                  \
                                                                               \
            bool is_const() const override {                                   \
                return std::is_const<ORIGIN_TYPE>::value;                      \
            }                                                                  \
                                                                               \
            iterator() = default;                                              \
                                                                               \
            iterator(ORIGIN_TYPE * origin, difference_type index) :            \
                origin(origin), index(index) {}                                \
                                                                               \
            iterator(const ORIGIN_TYPE & origin, difference_type index) :      \
                origin(&origin), index(index) {}                               \
                                                                               \
            INDEXED_ITERATOR_OPS(                                              \
                INDEXED_ITERATOR_EMBED_COMMAS(iterator<template_usage>),       \
                op_plus, op_minus, index_operator)                             \
    };

#define INDEXED_ITERATOR_ASSIGN(a, b, T) a = T(b.is_const() ? const_cast<typename T::O_TYPE*>(b.origin) : const_cast<typename T::O_TYPE*>(b.origin), b.index)

// allows begin/end sementics for [] operator
namespace IndexedIterator {

    struct iterator_base_obj {
            using difference_type = std::size_t;
            virtual std::ostream & toStream(std::ostream & os) const = 0;
            virtual bool is_const() const = 0;
            virtual const void * get_origin() const = 0;
            virtual const difference_type get_index() const = 0;
            virtual difference_type
            op_plus_(const difference_type & other) const = 0;
            virtual difference_type
            op_minus_(const difference_type & other) const = 0;

            difference_type operator+(const iterator_base_obj & other) {
                return op_plus_(other.get_index());
            }
            difference_type operator+(const iterator_base_obj & other) const {
                return op_plus_(other.get_index());
            }
            difference_type operator-(const iterator_base_obj & other) {
                return op_minus_(other.get_index());
            }
            difference_type operator-(const iterator_base_obj & other) const {
                return op_minus_(other.get_index());
            }
            const bool operator<(const iterator_base_obj & other) const {
                return get_origin() == other.get_origin()
                       && get_index() < other.get_index();
            }
            const bool operator<=(const iterator_base_obj & other) const {
                return get_origin() == other.get_origin()
                       && get_index() <= other.get_index();
            }
            const bool operator==(const iterator_base_obj & other) const {
                return get_origin() == other.get_origin()
                       && get_index() == other.get_index();
            }
            const bool operator!=(const iterator_base_obj & other) const {
                return !(*this == other);
            }
            const bool operator>(const iterator_base_obj & other) const {
                return get_origin() == other.get_origin()
                       && get_index() > other.get_index();
            }
            const bool operator>=(const iterator_base_obj & other) const {
                return get_origin() == other.get_origin()
                       && get_index() >= other.get_index();
            }
    };

    struct forward_iterator_base_obj : public iterator_base_obj {};
    struct reverse_iterator_base_obj : public iterator_base_obj {};

    INDEXED_ITERATOR_DEFINE(
        INDEXED_ITERATOR_EMBED_COMMAS(<typename T, typename V>),
        INDEXED_ITERATOR_EMBED_COMMAS(T, V), T, V, iterator,
        forward_iterator_base_obj, +, -, ->operator[](index))

    INDEXED_ITERATOR_DEFINE(
        INDEXED_ITERATOR_EMBED_COMMAS(<typename T, typename V>),
        INDEXED_ITERATOR_EMBED_COMMAS(T, V), T, V, reverse_iterator,
        reverse_iterator_base_obj, -, +, ->operator[](index))

    INDEXED_ITERATOR_DEFINE(INDEXED_ITERATOR_EMBED_COMMAS(<typename V>), V, V,
                            V, ptr_iterator, forward_iterator_base_obj, +,
                            -, [index])

    INDEXED_ITERATOR_DEFINE(INDEXED_ITERATOR_EMBED_COMMAS(<typename V>), V, V,
                            V, reverse_ptr_iterator, reverse_iterator_base_obj,
                            -, +, [index])

    std::ostream & operator<<(std::ostream & os, const iterator_base_obj & obj);

} // namespace IndexedIterator


namespace std {
    template <typename T, typename V>
    struct hash<IndexedIterator::iterator<T, V>> {
        size_t operator()(const IndexedIterator::iterator<T, V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename T, typename V>
    struct hash<const IndexedIterator::iterator<T, V>> {
        size_t operator()(const IndexedIterator::iterator<T, V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename T, typename V>
    struct hash<IndexedIterator::reverse_iterator<T, V>> {
        size_t operator()(const IndexedIterator::reverse_iterator<T, V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename T, typename V>
    struct hash<const IndexedIterator::reverse_iterator<T, V>> {
        size_t operator()(const IndexedIterator::reverse_iterator<T, V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename V>
    struct hash<IndexedIterator::ptr_iterator<V>> {
        size_t operator()(const IndexedIterator::ptr_iterator<V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename V>
    struct hash<const IndexedIterator::ptr_iterator<V>> {
        size_t operator()(const IndexedIterator::ptr_iterator<V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename V>
    struct hash<IndexedIterator::reverse_ptr_iterator<V>> {
        size_t operator()(const IndexedIterator::reverse_ptr_iterator<V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };

    template <typename V>
    struct hash<const IndexedIterator::reverse_ptr_iterator<V>> {
        size_t operator()(const IndexedIterator::reverse_ptr_iterator<V> & obj) const {
            return LibObj::Obj_Base::HashCodeBuilder().add(obj.origin).add(obj.index).hash;
        }
    };
} // namespace std

#endif