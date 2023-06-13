#ifndef INDEXED_ITERATOR_H
#define INDEXED_ITERATOR_H

// allows begin/end sementics for [] operator
namespace IndexedIterator {
    template <typename T, typename reference>
    struct iterator {
        
        T* origin = nullptr;
        mutable std::size_t index = 0;

        iterator(T* origin, std::size_t index) : origin(origin), index(index) {}
        iterator(const iterator<T, reference> & o) : origin(o.origin), index(o.index) {}

        iterator& operator ++(int) {
            index++;
            return *this;
        }

        iterator& operator --(int) {
            index--;
            return *this;
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
}

#endif