#ifndef STRING_ADAPTER_H
#define STRING_ADAPTER_H

#include <cstring>
#include <forward_list>
#include <list>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>
#include "indexed_iterator.h"

template <class T, class E>
using STRING_ADAPTER__FIRST = T;

// a templated std::hash that accepts any class which contains the function hashCode with the signature std::size_t () const

template <typename T>
struct std::hash<STRING_ADAPTER__FIRST<T, typename std::enable_if<
    std::is_member_function_pointer<decltype(static_cast<std::size_t(T::*)() const>(&T::hashCode))>::value
>::type>>
{
    std::size_t operator()(const T & x) const
    {
        return x.hashCode();
    }
};

namespace StringAdapter {
/*

reasoning:

as long as we are able to represent as structure
array, as a minimal string-like wrapper with one
way conversion to string, then it is technically
possible to then attempt to match sequential
structures and eventually specific structure
sequences

we CANNOT use std::basic_string<T> because it
requires TWO-WAY conversion from T to int
and int to T





template decleration for classes wishing to use this:
    
template <
    typename T,
    typename adapter_t,
    typename AdapterMustExtendBasicStringAdapter = std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
>

// ...

// _T is expected for the default char* implementation of classes accepting StringAdapter
//
// this is optional but highly recommended as ( char* / std::string ) based data is extremely common
//
//   also QT variants ( QChar / QString ) are very common in QT based libs/apps (eg KDE, QTCreator, ect )
//     so maybe provide a _QT version as well:
//
//         using YourClass_QT = YourClass<QChar, StringAdapter::CharAdapter_impl<QChar, void>>;
//

using YourClass_T = YourClass<char, StringAdapter::CharAdapter>;

// TYPEDEF

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    // WILL NOT COMPILE
    typedef A<T, adapter_t> Deps;

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    // WILL COMPILE
    using Deps = A<T, adapter_t>;


// EXAMPLE

    template <
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    typedef Tuple2<Set<const Named<Patch*, T, adapter_t>>, Set<const Named<Patch*, T, adapter_t>>> Deps;

    using Deps_T = Deps<char, StringAdapter::CharAdapter>;


// TEMPLATE SPECIALIZATION

    template <
        typename T,
        typename T,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type
    >
    struct Named {
        Named() {
            puts("T");
        }
    };

    template <typename T>
    using Named_T = Named<T, char, StringAdapter::CharAdapter>;

    template <typename T, typename T, typename adapter_t>
    struct Named<T*, T, adapter_t, typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<T>, adapter_t>::value>::type> {
        Named() {
            puts("T*");
        }
    };

    // no need to specify Named_T, the above specialization will handle this for us

    Named_T<int> foo;
    Named_T<int*> foop;

    int main() {}

*/

    template <typename T1, typename T2>
    static int compare_2(const T1 & a, const T2 & b) {
        if (a < b) {
            return 1;
        }
        if (b < a) {
            return -1;
        }
        return 0;
    }

    template <class InputIt1, class InputIt2>
    static int lexicographical_compare_2(InputIt1 first1, InputIt1 last1,
                                InputIt2 first2, InputIt2 last2)
    {
        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
        {
            int r = compare_2(*first1, *first2);
            if (r != 0) {
                return r;
            }
        }

        if (first1 == last1) {
            if (first2 == last2) {
                // first1 is end
                // first2 is end
                return 0; // contents equal
            } else {
                // first1 is end
                // first2 is not end
                return -1;
            }
        } else {
            // first1 is not end
            // first2 is end
            return 1;
        }
    }

    template <class F>
    static void compare_3_func(const F & fa, const F & fb, int & r)
    {
        if (r == 0) {
            r = compare_2(fa, fb);
        }
    }

    template <typename C, class Tuple, std::size_t... Is>
    static int compare_3_tuple(const C & a_, const C & b_, const Tuple& t, std::index_sequence<Is...>)
    {
        int r = 0;
        (compare_3_func(a_.*(std::get<Is>(t)), b_.*(std::get<Is>(t)), r) , ...);
        return r;
    }

    template <typename C, typename ... T>
    static int compare_3(const C & a_, const C & b_, const T & ... args) {
        return compare_3_tuple(a_, b_, std::tie(args...), std::index_sequence_for<T...>{});
    }

    template <class F>
    static void hash_3_func(const F & fa, std::size_t & hash)
    {
        hash = 31 * hash + std::hash<F>()(fa);
    }

    template <typename C, class Tuple, std::size_t... Is>
    static std::size_t hash_3_tuple(const C & a_, const Tuple& t, std::index_sequence<Is...>)
    {
        std::size_t hashCode_ = 1;
        (hash_3_func(a_.*(std::get<Is>(t)), hashCode_) , ...);
        return hashCode_;
    }

    template <typename C, typename ... T>
    static std::size_t hash_3(const C & a_, const T & ... args) {
        return hash_3_tuple(a_, std::tie(args...), std::index_sequence_for<T...>{});
    }

    template <typename C, typename IT_CBEGIN_FUNC, typename IT_CEND_FUNC>
    static int compare_3_iterator(const C & a_, const C & b_, const IT_CBEGIN_FUNC & b1, const IT_CEND_FUNC & e1) {
        return lexicographical_compare_2((a_.*b1)(), (a_.*e1)(), (b_.*b1)(), (b_.*e1)());
    }

    template <typename C, typename T>
    static std::size_t hash_3_iterator(const C & a_) {
        std::size_t hashCode_ = 1;
        for(const T & item : a_) {
            hashCode_ = 31 * hashCode_ + std::hash<T>()(item);
        }
        return hashCode_;
    }

    template <typename C>
    struct Comparable {

        std::function<int(const C & a, const C & b)> c;

        Comparable() = default;

        Comparable(std::function<int(const C & a, const C & b)> f) {
            c = f;
        }

        Comparable(const Comparable<C> & other) {
            c = other.c;
        }

        Comparable(Comparable<C> && other) {
            c = other.c;
        }

        Comparable<C> & operator = (const Comparable<C> & other) {
            c = other.c;
            return *this;
        }

        Comparable<C> & operator = (Comparable<C> && other) {
            c = other.c;
            return *this;
        }

        int operator < (const C & x) const {
            return c(static_cast<const C&>(*this), x) < 0;
        }
        int operator <= (const C & x) const {
            return c(static_cast<const C&>(*this), x) <= 0;
        }
        int operator == (const C & x) const {
            return c(static_cast<const C&>(*this), x) == 0;
        }
        int operator != (const C & x) const {
            return c(static_cast<const C&>(*this), x) != 0;
        }
        int operator > (const C & x) const {
            return c(static_cast<const C&>(*this), x) > 0;
        }
        int operator >= (const C & x) const {
            return c(static_cast<const C&>(*this), x) >= 0;
        }
    };

    template <typename C>
    struct Hashable {

        std::function<std::size_t(const C & a)> c;

        Hashable() = default;

        Hashable(std::function<std::size_t(const C & a)> f) {
            c = f;
        }

        Hashable(const Hashable<C> & other) {
            c = other.c;
        }

        Hashable(Hashable<C> && other) {
            c = other.c;
        }

        Hashable<C> & operator = (const Hashable<C> & other) {
            c = other.c;
            return *this;
        }

        Hashable<C> & operator = (Hashable<C> && other) {
            c = other.c;
            return *this;
        }

        std::size_t hashCode() const noexcept {
            // https://docs.oracle.com/javase/8/docs/api/java/util/List.html#hashCode--
            return c(static_cast<const C&>(*this));
        }
    };

#define COMPARABLE_USING_BASE(BASE) \
        using BASE::operator <; \
        using BASE::operator <=; \
        using BASE::operator ==; \
        using BASE::operator !=; \
        using BASE::operator >=; \
        using BASE::operator >; \

#define HASHABLE_USING_BASE(BASE) \
        using BASE::hashCode; \

    template <typename T>
    class Shared :
        public Comparable<Shared<T>>,
        public Hashable<Shared<T>>
    {
        std::shared_ptr<T> ptr_;
        std::size_t len_ = 0;

        public:

        COMPARABLE_USING_BASE(Comparable<Shared<T>>);
        HASHABLE_USING_BASE(Hashable<Shared<T>>);

        Shared() :
            Shared<T>::Comparable([](auto & a, auto & b) { return compare_3(a, b, &Shared<T>::ptr_, &Shared<T>::len_); }),
            Shared<T>::Hashable([](auto & a) { return hash_3(a, &Shared<T>::ptr_, &Shared<T>::len_); })
        {}
        
        Shared(T * ptr, std::size_t len) : Shared(ptr, len, [](auto p) {}) {}

        template <class Deleter>
        Shared(T * ptr, std::size_t len, Deleter deleter) : Shared() {
            if (ptr != nullptr) {
                ptr_ = std::shared_ptr<T>(ptr, deleter);
                len_ = len;
            }
        }

        Shared(const Shared<T> & other) : Comparable<Shared<T>>(other), Hashable<Shared<T>>(other) {
            ptr_ = other.ptr_;
            len_ = other.len_;
        }
        Shared(Shared<T> && other) : Comparable<Shared<T>>(std::move(other)), Hashable<Shared<T>>(std::move(other)) {
            ptr_ = std::move(other.ptr_);
            len_ = other.len_;
            other.len_ = 0;
        }
        Shared<T> & operator =(const Shared<T> & other) {
            Comparable<Shared<T>>::operator =(other);
            Hashable<Shared<T>>::operator =(other);
            ptr_ = other.ptr_;
            len_ = other.len;
            return *this;
        }
        Shared<T> & operator =(Shared<T> && other) {
            Comparable<Shared<T>>::operator =(std::move(other));
            Hashable<Shared<T>>::operator =(std::move(other));
            ptr_ = std::move(other.ptr_);
            len_ = other.len_;
            other.len_ = 0;
            return *this;
        }

        T * ptr() {
            return ptr_.get();
        }

        std::size_t length() {
            return len_;
        }

        std::size_t lengthInBytes() {
            return len_ * sizeof(T);
        }
    };

    template <typename T>
    class CShared :
        public Comparable<CShared<T>>,
        public Hashable<CShared<T>>
    {
        mutable std::shared_ptr<const T> ptr_;
        mutable std::size_t len_ = 0;

        public:

        COMPARABLE_USING_BASE(Comparable<CShared<T>>);
        HASHABLE_USING_BASE(Hashable<CShared<T>>);
        
        CShared() :
            CShared<T>::Comparable([](auto & a, auto & b) { return compare_3(a, b, &CShared<T>::ptr_, &CShared<T>::len_); }),
            CShared<T>::Hashable([](auto & a) { return hash_3(a, &CShared<T>::ptr_, &CShared<T>::len_); })
        {}
        
        CShared(const T * ptr, std::size_t len) : CShared(ptr, len, [](auto p) {}) {}

        template <class Deleter>
        CShared(const T * ptr, std::size_t len, Deleter deleter) : CShared() {
            if (ptr != nullptr) {
                ptr_ = std::shared_ptr<const T>(ptr, deleter);
                len_ = len;
            }
        }
        CShared(const CShared<T> & other) : Comparable<CShared<T>>(other), Hashable<CShared<T>>(other) {
            ptr_ = other.ptr_;
            len_ = other.len_;
        }
        CShared(CShared<T> && other) : Comparable<CShared<T>>(std::move(other)), Hashable<CShared<T>>(std::move(other)) {
            ptr_ = std::move(other.ptr_);
            len_ = other.len_;
            other.len_ = 0;
        }
        CShared<T> & operator =(const CShared<T> & other) {
            Comparable<CShared<T>>::operator =(other);
            Hashable<CShared<T>>::operator =(other);
            ptr_ = other.ptr_;
            len_ = other.len;
            return *this;
        }
        CShared<T> & operator =(CShared<T> && other) {
            Comparable<CShared<T>>::operator =(std::move(other));
            Hashable<CShared<T>>::operator =(std::move(other));
            ptr_ = std::move(other.ptr_);
            len_ = other.len_;
            other.len_ = 0;
            return *this;
        }

        const T * ptr() const {
            return ptr_.get();
        }

        const std::size_t length() const {
            return len_;
        }

        const std::size_t lengthInBytes() const {
            return len_ * sizeof(T);
        }
    };

    template <typename T>
    struct BasicStringAdapter :
        public Comparable<BasicStringAdapter<T>>,
        public Hashable<BasicStringAdapter<T>>
    {
        COMPARABLE_USING_BASE(Comparable<BasicStringAdapter<T>>);
        HASHABLE_USING_BASE(Hashable<BasicStringAdapter<T>>);
        
#define BASIC_STRING_ADAPTER_SLICE_USING_BASE(BASE) \
        using BASE::BASE; \
        using BASE::begin; \
        using BASE::end; \
        using BASE::rbegin; \
        using BASE::rend; \
        using BASE::operator[]; \
        COMPARABLE_USING_BASE(BASE); \
        HASHABLE_USING_BASE(BASE); \
        using BASE::get_origin; \
        using BASE::get_origin_; \

#define BASIC_STRING_ADAPTER_CSLICE_USING_BASE(BASE) \
        using BASE::BASE; \
        using BASE::begin; \
        using BASE::end; \
        using BASE::cbegin; \
        using BASE::cend; \
        using BASE::rbegin; \
        using BASE::rend; \
        using BASE::crbegin; \
        using BASE::crend; \
        using BASE::operator[]; \
        COMPARABLE_USING_BASE(BASE); \
        HASHABLE_USING_BASE(BASE); \
        using BASE::get_origin; \
        using BASE::get_origin_; \
    
        class Slice :
            public Comparable<Slice>,
            public Hashable<Slice>
        {
            BasicStringAdapter<T> * origin = nullptr;
            std::size_t start_ = 0;
            std::size_t end_ = 0;

            public:

            COMPARABLE_USING_BASE(Comparable<Slice>);
            HASHABLE_USING_BASE(Hashable<Slice>);

            Slice(BasicStringAdapter<T>* origin, std::size_t start, std::size_t end) : origin(origin), start_(start), end_(start < end ? end : start),
                Slice::Comparable([](auto & a, auto & b) { return compare_3_iterator(a, b, &Slice::cbegin, &Slice::cend); }),
                Slice::Hashable([](auto & a) { return hash_3_iterator<Slice, T>(a); })
            {}

            using iterator = IndexedIterator::iterator<BasicStringAdapter<T>, T>;
            using reverse_iterator = IndexedIterator::reverse_iterator<BasicStringAdapter<T>, T>;

            using const_iterator = IndexedIterator::iterator<const BasicStringAdapter<T>, const T>;
            using reverse_const_iterator = IndexedIterator::reverse_iterator<const BasicStringAdapter<T>, const T>;

            iterator begin() {
                return iterator(origin, start_);
            }

            iterator end() {
                return iterator(origin, end_+1);
            }

            reverse_iterator rbegin() {
                return reverse_iterator(origin, end_);
            }

            reverse_iterator rend() {
                return reverse_iterator(origin, start_-1);
            }

            const_iterator begin() const {
                return const_iterator(origin, start_);
            }

            const_iterator end() const {
                return const_iterator(origin, end_+1);
            }

            const_iterator cbegin() const {
                return const_iterator(origin, start_);
            }

            const_iterator cend() const {
                return const_iterator(origin, end_+1);
            }

            reverse_const_iterator rbegin() const {
                return reverse_const_iterator(origin, end_);
            }

            reverse_const_iterator rend() const {
                return reverse_const_iterator(origin, start_-1);
            }

            reverse_const_iterator rcbegin() const {
                return reverse_const_iterator(origin, end_);
            }

            reverse_const_iterator rcend() const {
                return reverse_const_iterator(origin, start_-1);
            }

            T & operator[] (const std::size_t index) {
                return origin->operator[](start_ + index);
            }

            const T & operator[] (const std::size_t index) const {
                return origin->operator[](start_ + index);
            }

            const std::size_t get_start() {
                return start_;
            };

            const std::size_t get_end() {
                return end_;
            };

            virtual BasicStringAdapter<T>* get_origin() = 0;

            protected:
            BasicStringAdapter<T>* get_origin_() {
                return origin;
            };
        };
        
        class CSlice :
            public Comparable<CSlice>,
            public Hashable<CSlice>
        {
            const BasicStringAdapter<T> * origin = nullptr;
            std::size_t start_ = 0;
            std::size_t end_ = 0;

            public:

            COMPARABLE_USING_BASE(Comparable<CSlice>);
            HASHABLE_USING_BASE(Hashable<CSlice>);

            CSlice(const BasicStringAdapter<T>* origin, const std::size_t start, const std::size_t end) : origin(origin), start_(start), end_(start < end ? end : start),
                CSlice::Comparable([](auto & a, auto & b) { return compare_3_iterator(a, b, &CSlice::cbegin, &CSlice::cend); }),
                CSlice::Hashable([](auto & a) { return hash_3_iterator<CSlice, T>(a); })
            {}

            using const_iterator = IndexedIterator::iterator<const BasicStringAdapter<T>, const T>;
            using reverse_const_iterator = IndexedIterator::reverse_iterator<const BasicStringAdapter<T>, const T>;

            const const_iterator cbegin() const {
                return const_iterator(origin, start_);
            }

            const const_iterator cend() const {
                return const_iterator(origin, end_+1);
            }

            const const_iterator begin() const {
                return cbegin();
            }

            const const_iterator end() const {
                return cend();
            }

            const reverse_const_iterator crbegin() const {
                return reverse_const_iterator(origin, end_);
            }

            const reverse_const_iterator crend() const {
                return reverse_const_iterator(origin, start_-1);
            }

            const reverse_const_iterator rbegin() const {
                return crbegin();
            }

            const reverse_const_iterator rend() const {
                return crend();
            }

            const std::size_t get_start() {
                return start_;
            };

            const std::size_t get_end() {
                return end_;
            };

            const T & operator[] (const std::size_t index) const {
                return origin->operator[](start_ + index);
            }

            virtual const BasicStringAdapter<T>* get_origin() = 0;

            protected:
            const BasicStringAdapter<T>* get_origin_() {
                return origin;
            };
        };

        typedef T TYPE;

        BasicStringAdapter<T>() :
                BasicStringAdapter<T>::Comparable([](auto & a, auto & b) { return compare_3_iterator(a, b, &BasicStringAdapter<T>::cbegin, &BasicStringAdapter<T>::cend); }),
                BasicStringAdapter<T>::Hashable([](auto & a) { return hash_3_iterator<BasicStringAdapter<T>, T>(a); })
        {}

        void to_string() const {
            std::cout << *this;
        }

        mutable std::shared_ptr<const T> new_line;
        mutable std::shared_ptr<const T> eof;

        const T & get_new_line() const {
            return *new_line;
        }
        
        const T & get_end_of_file() const {
            return *eof;
        }

        using iterator = IndexedIterator::iterator<BasicStringAdapter<T>, T>;
        using const_iterator = IndexedIterator::iterator<const BasicStringAdapter<T>, const T>;

        virtual iterator begin() {
            return iterator(this, 0);
        }

        virtual iterator end() {
            return iterator(this, length());
        }

        virtual const const_iterator cbegin() const {
            return const_iterator(this, 0);
        }

        virtual const const_iterator cend() const {
            return const_iterator(this, length());
        }

        virtual const const_iterator begin() const {
            return cbegin();
        }

        virtual const const_iterator end() const {
            return cend();
        }

        using reverse_iterator = IndexedIterator::reverse_iterator<BasicStringAdapter<T>, T>;
        using reverse_const_iterator = IndexedIterator::reverse_iterator<const BasicStringAdapter<T>, const T>;

        virtual reverse_iterator rbegin() {
            return reverse_iterator(this, length()-1);
        }

        virtual reverse_iterator rend() {
            return reverse_iterator(this, -1);
        }

        virtual const reverse_const_iterator crbegin() const {
            return reverse_const_iterator(this, length()-1);
        }

        virtual const reverse_const_iterator crend() const {
            return reverse_const_iterator(this, -1);
        }

        virtual const reverse_const_iterator rbegin() const {
            return crbegin();
        }

        virtual const reverse_const_iterator rend() const {
            return crend();
        }

        virtual Slice* slice(std::size_t start, std::size_t end) = 0;
        virtual const CSlice* cslice(std::size_t start, std::size_t end) const = 0;
        virtual const CSlice* slice(std::size_t start, std::size_t end) const = 0;

        virtual BasicStringAdapter<T>* clone() = 0;
        virtual const BasicStringAdapter<T>* clone() const = 0;
        
        virtual T & get_item_at_index(const std::size_t index) = 0;
        virtual const T & get_item_at_index(const std::size_t index) const = 0;
        
        T & operator[] (const std::size_t index) {
            return get_item_at_index(index);
        };

        const T & operator[] (const std::size_t index) const {
            return get_item_at_index(index);
        };
        
        // must never be null
        virtual Shared<T> data() = 0;
        virtual const CShared<T> data() const = 0;

        virtual Shared<char> c_str_() {
            return data();
        }
        virtual const CShared<char> c_str_() const {
            return data();
        }
        
        virtual std::vector<T>* data_as_vector() = 0;
        virtual const std::vector<T>* data_as_vector() const = 0;
        
        virtual const std::size_t length() const = 0;
        
        virtual void resize(const std::size_t capacity) = 0;
        
        const std::size_t size() const {
            return length();
        }

        virtual Shared<char> c_str() = 0;
        virtual const CShared<char> c_str() const = 0;

        const bool operator == (const char * other) const {
            return strcmp(other) == 0;
        }

        const bool operator != (const char* other) const {
            return strcmp(other) != 0;
        }

        const int strcmp(const BasicStringAdapter<T> & other) const {
            return lexicographical_compare_2(begin(), end(), other.begin(), other.end());
        }

        const int strcmp(const char * other) const {
            return strcmp(other, strlen(other));
        }

        const int strcmp(const T * other, const std::size_t len) const {
            return lexicographical_compare_2(begin(), end(), IndexedIterator::ptr_iterator<const T>(other, 0), IndexedIterator::ptr_iterator<const T>(other, len));
        }

        virtual void append_(const BasicStringAdapter<T> & what) {
            insert_(what, -1);
        }
        
        virtual void insert_(const BasicStringAdapter<T> & what, const std::size_t pos) = 0;
        
        virtual void replace_(const BasicStringAdapter<T> & what, const std::size_t pos, const std::size_t length) {
            
            // be naive
            
            const std::size_t len = size();
            
            const std::size_t p = clamp_pos(len, pos);
            
            if (p >= len) {
                insert_(what, p);
                return;
            }
            
            // iterator end - iterator start = length
            //
            const std::size_t l = clamp_length(len, p, length) - p;
            
            if (l != 0) {
                erase_(p, l);
            }
            
            insert_(what, p);
        }
        
        virtual void erase_(const std::size_t pos, const std::size_t length) = 0;
        
        virtual ~BasicStringAdapter() {};
        
        const std::size_t clamp_pos(const std::size_t pos) const {
            return clamp_pos(size(), pos);
        }

        const std::size_t clamp_pos(const std::size_t length, const std::size_t pos) const {
            return pos == -1 ? length : pos >= length ? length : pos;
        }
        
        const std::size_t clamp_length(const std::size_t clamped_pos, const std::size_t len) const {
            return clamp_length(size(), clamped_pos, len);
        }
        
        const std::size_t clamp_length(const std::size_t length, const std::size_t clamped_pos, const std::size_t len) const {
            return clamped_pos == length ? 0 : len == -1 ? length : clamped_pos + len >= length ? length : clamped_pos + len;
        }

        virtual const void deleteSelf() const = 0;
        virtual BasicStringAdapter<T>* newAdapter() = 0;
        virtual BasicStringAdapter<T>* newAdapter(const T* ptr) = 0;
        virtual BasicStringAdapter<T>* newAdapter(const T* ptr, const std::size_t length) = 0;
        virtual BasicStringAdapter<T>* newAdapter() const = 0;
        virtual BasicStringAdapter<T>* newAdapter(const T* ptr) const = 0;
        virtual BasicStringAdapter<T>* newAdapter(const T* ptr, const std::size_t length) const = 0;

        std::vector<std::shared_ptr<BasicStringAdapter<T>>> split(const T & splitter) const {
            std::vector<std::shared_ptr<BasicStringAdapter<T>>> vec;
            BasicStringAdapter<T>* c = newAdapter();
            for (const T & t : *this) {
                if (t == splitter) {
                    vec.push_back(std::shared_ptr<BasicStringAdapter<T>>(c));
                    c = newAdapter();
                } else {
                    T s[2] = { t, get_end_of_file() };
                    BasicStringAdapter<T>* tmp = newAdapter(s, 1);
                    c->append_(*tmp);
                    tmp->deleteSelf();
                }
            }
            vec.push_back(std::shared_ptr<BasicStringAdapter<T>>(c));
            return vec;
        }

        std::vector<std::shared_ptr<BasicStringAdapter<T>>> lines() const {
            const std::size_t len = length();
            if (len == 0) {
                return {std::shared_ptr<BasicStringAdapter<T>>(newAdapter())};
            }

            return split(get_new_line());
        }

        const std::size_t line_count() const {
            return lines().size();
        }
    };
    
#define BASIC_STRING_ADAPTER_USING_BASE(BASE) \
        using BASE::BASE; \
        using BASE::get_item_at_index; \
        using BASE::operator[]; \
        COMPARABLE_USING_BASE(BASE); \
        HASHABLE_USING_BASE(BASE); \
        using BASE::data; \
        using BASE::data_as_vector; \
        using BASE::c_str; \
        using BASE::c_str_; \
        using BASE::size; \
        using BASE::length; \
        using BASE::resize; \
        using BASE::append_; \
        using BASE::insert_; \
        using BASE::replace_; \
        using BASE::erase_; \
        using BASE::clamp_pos; \
        using BASE::clamp_length; \
        using BASE::get_new_line; \
        using BASE::get_end_of_file; \
        using BASE::clone; \
        using BASE::lines; \
        using BASE::split; \
        using BASE::line_count; \
        using BASE::strcmp; \
        using BASE::new_line; \
        using BASE::eof; \
        using BASE::begin; \
        using BASE::end; \
        using BASE::cbegin; \
        using BASE::cend; \
        using BASE::rbegin; \
        using BASE::rend; \
        using BASE::crbegin; \
        using BASE::crend; \
        using BASE::slice; \
        using BASE::cslice; \
        using typename BASE::iterator; \
        using typename BASE::const_iterator; \
        using typename BASE::reverse_iterator; \
        using typename BASE::reverse_const_iterator; \

#define BASIC_STRING_ADAPTER_USING(BASE) \
        BASIC_STRING_ADAPTER_USING_BASE(BASE) \
        using BASE::operator =; \
        using BASE::init; \

#define INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(ITEM) std::shared_ptr<T>(new T(ITEM))

#define BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(NAME, T) \
        NAME(const T & new_line, const T & eof) { \
            init(nullptr, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const T* ptr, const T & new_line, const T & eof) { \
            init(ptr, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const T* ptr, const std::size_t length, const T & new_line, const T & eof) { \
            init(ptr, length, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(nullptr, new_line, eof); \
        } \
\
        NAME(const T* ptr, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(ptr, new_line, eof); \
        } \
\
        NAME(const T* ptr, const std::size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(ptr, length, new_line, eof); \
        } \
\
        void init(const T* ptr, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
\
            if (ptr == nullptr) { \
                init(nullptr, 0, new_line, eof); \
                return; \
            } \
\
            std::size_t length = 0; \
\
            while(true) { \
                if (ptr[length] == *eof) { \
                    break; \
                } \
                length++; \
            } \
\
            init(ptr, length, new_line, eof); \
        }

    template <typename T>
    struct ListAdapter : public BasicStringAdapter<T> {
        mutable std::list<T> list;
        mutable std::size_t len;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(ListAdapter, T)
        
        ListAdapter(const ListAdapter & other) : BasicStringAdapter<T>(other) {
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ListAdapter & operator =(const ListAdapter & other) {
            BasicStringAdapter<T>::operator =(other);
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        ListAdapter(ListAdapter && other) : BasicStringAdapter<T>(std::move(other)) {
            list = std::move(other.list);
            len = std::move(other.len);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
        }
        
        const ListAdapter & operator =(ListAdapter && other) {
            BasicStringAdapter<T>::operator =(std::move(other));
            list = std::move(other.list);
            len = std::move(other.len);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
            return *this;
        }
        
        void init(const T* ptr, const std::size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
            this->new_line = new_line;
            this->eof = eof;
            
            if (ptr == nullptr || length == 0) {
                this->len = 0;
                this->list = {};
                this->list.emplace_back(get_end_of_file());
                return;
            }
            
            this->list = {};
            len = length;
            if (length != 0) {
                for(std::size_t s = 0; s < length; s++) {
                    this->list.emplace_back(ptr[s]);
                }
                this->list.emplace_back(get_end_of_file());
            }
        }

        T & get_item_at_index(const std::size_t index) override {
            return *std::next(list.begin(), index);
        }

        const T & get_item_at_index(const std::size_t index) const override {
            return *std::next(list.begin(), index);
        }

        const std::size_t length() const override {
            return len;
        }

        Shared<T> data() override {
            T* d = new T[len+1];
            std::size_t i = 0;
            for (const T & copy : list) {
                d[i] = copy;
                i++;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        const CShared<T> data() const override {
            T* d = new T[len+1];
            std::size_t i = 0;
            for (const T & copy : list) {
                d[i] = copy;
                i++;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        std::vector<T>* data_as_vector() override {
            return nullptr;
        }

        const std::vector<T>* data_as_vector() const override {
            return nullptr;
        }

        void resize(const std::size_t capacity) override {
            const std::size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    list.resize(capacity);
                }
            }
        }

        void insert_(const BasicStringAdapter<T> & what, const std::size_t pos) override {
            auto* vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                list.insert(std::next(list.begin(), clamp_pos(pos)), vec->begin(), vec->end()-1);
                len += what.length();
            } else {
                const CShared<T> p = what.data();
                const std::size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(std::size_t s = 0; s < l; s++) {
                        v[s] = p.ptr()[s];
                    }
                }
                list.insert(std::next(list.begin(), clamp_pos(pos)), v.begin(), v.end());
                len += l;
            }
        }
        
        virtual void erase_(const std::size_t pos, const std::size_t length) override {
            
            // be naive
            
            const std::size_t len_ = size();
            
            const std::size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const std::size_t l = clamp_length(len_, p, length);
            
            if (pos == 0 && l == len_) {
                list = {};
                len = 0;
            } else if (l != 0) {
                list.erase(std::next(list.begin(), p), std::next(list.begin(), l));
                len = len_ - (l - p);
            }
        }
        
        const void deleteSelf() const override {
            delete (ListAdapter<T>*) this;
        }

        ~ListAdapter() override {
        }
    };

    // a reverse string
    template <typename T>
    struct ForwardListAdapter : public BasicStringAdapter<T> {
        mutable std::forward_list<T> forward_list;
        mutable std::size_t len;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);
        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(ForwardListAdapter, T)

        virtual iterator begin() override {
            auto l = length();
            l = l == 0 ? 0 : 1;
            return iterator(this, l);
        }

        virtual const const_iterator cbegin() const override {
            auto l = length();
            l = l == 0 ? 0 : 1;
            return const_iterator(this, l);
        }

        virtual iterator end() override {
            auto l = length();
            l = l == 0 ? 0 : l+1;
            return iterator(this, l);
        }

        virtual const const_iterator cend() const override {
            auto l = length();
            l = l == 0 ? 0 : l+1;
            return const_iterator(this, l);
        }

        virtual reverse_iterator rbegin() override {
            return reverse_iterator(this, length());
        }

        virtual const reverse_const_iterator crbegin() const override {
            return reverse_const_iterator(this, length());
        }

        virtual reverse_iterator rend() override {
            return reverse_iterator(this, 0);
        }

        virtual const reverse_const_iterator crend() const override {
            return reverse_const_iterator(this, 0);
        }
        
        ForwardListAdapter(const ForwardListAdapter & other) : BasicStringAdapter<T>(other) {
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ForwardListAdapter & operator =(const ForwardListAdapter & other) {
            BasicStringAdapter<T>::operator =(other);
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        ForwardListAdapter(ForwardListAdapter && other) : BasicStringAdapter<T>(std::move(other)) {
            forward_list = std::move(other.forward_list);
            len = std::move(other.len);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
        }
        
        const ForwardListAdapter & operator =(ForwardListAdapter && other) {
            BasicStringAdapter<T>::operator =(std::move(other));
            forward_list = std::move(other.forward_list);
            len = std::move(other.len);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
            return *this;
        }

        void init(const T* ptr, const std::size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
            this->new_line = new_line;
            this->eof = eof;
            
            if (ptr == nullptr || length == 0) {
                this->forward_list = {};
                this->forward_list.emplace_front(get_end_of_file());
                this->len = 0;
                return;
            }

            this->forward_list = {};
            len = length;
            if (length != 0) {
                for(std::size_t s = 0; s < length; s++) {
                    this->forward_list.emplace_front(ptr[s]);
                }
                this->forward_list.emplace_front(get_end_of_file());
            }
        }

        T & get_item_at_index(const std::size_t index) override {
            return *std::next(forward_list.begin(), index);
        }

        const T & get_item_at_index(const std::size_t index) const override {
            return *std::next(forward_list.begin(), index);
        }

        const std::size_t length() const override {
            return len;
        }

        Shared<T> data() override {
            T* d = new T[len+1];
            std::size_t i = 0;
            for (const T & copy : forward_list) {
                d[i] = copy;
                i++;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        const CShared<T> data() const override {
            T* d = new T[len+1];
            std::size_t i = 0;
            for (const T & copy : forward_list) {
                d[i] = copy;
                i++;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        Shared<T> c_str_() override {
            T* d = new T[len+1];
            std::size_t i = len;
            for (const T & copy : forward_list) {
                d[i] = copy;
                i--;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        const CShared<T> c_str_() const override {
            T* d = new T[len+1];
            std::size_t i = len;
            for (const T & copy : forward_list) {
                d[i] = copy;
                i--;
            }
            return {d, len, [](auto p) { delete[] p; }};
        }

        std::vector<T>* data_as_vector() override {
            return nullptr;
        }

        const std::vector<T>* data_as_vector() const override {
            return nullptr;
        }

        void resize(const std::size_t capacity) override {
            const std::size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    forward_list.resize(capacity);
                }
            }
        }

        void insert_(const BasicStringAdapter<T> & what, const std::size_t pos) override {
            auto* vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                forward_list.insert_after(std::next(forward_list.begin(), clamp_pos(pos)), vec->begin(), vec->end()-1);
                len += what.length();
            } else {
                const CShared<T> p = what.data();
                const std::size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(std::size_t s = 0; s < l; s++) {
                        v[s] = p.ptr()[s+1];
                    }
                }
                forward_list.insert_after(std::next(forward_list.begin(), clamp_pos(pos)), v.begin(), v.end());
                len += l;
            }
        }
        
        virtual void erase_(const std::size_t pos, const std::size_t length) override {
            
            // be naive
            
            const std::size_t len_ = size();
            
            const std::size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const std::size_t l = clamp_length(len_, p, length);
            
            if (pos == 0 && l == len_) {
                forward_list = {};
                len = 0;
            } else if (l != 0) {
                auto begin = std::next(forward_list.begin(), p);
                auto end = std::next(begin, l);
                forward_list.erase_after(begin, end);
                len = len_ - (l - p);
            }
        }

        const void deleteSelf() const override {
            delete (ForwardListAdapter<T>*) this;
        }

        ~ForwardListAdapter() override {
        }
    };

    template <typename T>
    struct VectorAdapter : public BasicStringAdapter<T> {
        mutable std::vector<T> vector;
        
        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);
        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(VectorAdapter, T)

        VectorAdapter(const VectorAdapter & other) : BasicStringAdapter<T>(other) {
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const VectorAdapter & operator =(const VectorAdapter & other) {
            BasicStringAdapter<T>::operator =(other);
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        VectorAdapter(VectorAdapter && other) : BasicStringAdapter<T>(std::move(other)) {
            vector = std::move(other.vector);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
        }
        
        const VectorAdapter & operator =(VectorAdapter && other) {
            BasicStringAdapter<T>::operator =(std::move(other));
            vector = std::move(other.vector);
            new_line = std::move(other.new_line);
            eof = std::move(other.eof);
            return *this;
        }

        void init(const T* ptr, const std::size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
            this->new_line = new_line;
            this->eof = eof;

            if (ptr == nullptr || length == 0) {
                this->vector = std::move(std::vector<T>(1));
                this->vector[0] = get_end_of_file();
                return;
            }
            
            vector = std::move(std::vector<T>(length+1));
            if (length != 0) {
                for(std::size_t s = 0; s < length; s++) {
                    vector[s] = ptr[s];
                }
                vector[length] = get_end_of_file();
            }
        }
        
        T & get_item_at_index(const std::size_t index) override {
            return vector[index];
        }

        const T & get_item_at_index(const std::size_t index) const override {
            return vector[index];
        }
        
        Shared<T> data() override {
            return {vector.data(), length()};
        }

        const CShared<T> data() const override {
            return {vector.data(), length()};
        }
        
        std::vector<T>* data_as_vector() override {
            return &vector;
        }

        const std::vector<T>* data_as_vector() const override {
            return &vector;
        }
        
        const std::size_t length() const override {
            return vector.size()-1;
        }
        
        void resize(const std::size_t capacity) override {
            const std::size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    vector.resize(capacity);
                }
            }
        }
        
        void insert_(const BasicStringAdapter<T> & what, const std::size_t pos) override {
            auto* vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                vector.insert(vector.begin() + clamp_pos(pos), vec->begin(), vec->end()-1);
            } else {
                const CShared<T> p = what.data();
                const std::size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(std::size_t s = 0; s < l; s++) {
                        v[s] = p.ptr()[s];
                    }
                }
                vector.insert(vector.begin() + clamp_pos(pos), v.begin(), v.end());
            }
        }
        
        virtual void erase_(const std::size_t pos, const std::size_t length) override {
            
            // be naive
            
            const std::size_t len_ = size();
            
            const std::size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const std::size_t l = clamp_length(len_, p, length);
            
            if (pos == 0 && l == len_) {
                vector = {};
            } else if (l != 0) {
                vector.erase(vector.begin() + p, vector.begin() + l);
            }
        }
        
        ~VectorAdapter() override {
        }
    };
    
    template <typename T>
    struct ResizingVectorAdapter : public VectorAdapter<T> {
        
        using BASE = VectorAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);
        using BASE::vector;

        void insert_(const BasicStringAdapter<T> & what, const std::size_t pos) override {
            vector.reserve(length() + what.length() + 1);
            BASE::insert_(what, pos);
        }
        
        virtual void erase_(const std::size_t pos, const std::size_t length) override {
            BASE::erase_(pos, length);
            vector.shrink_to_fit();
        }
        
        ~ResizingVectorAdapter() override {
        }
    };

#define CHAR_ADAPTER_IMPL(ADAPTER) \
template <typename T> \
struct ADAPTER##_impl : ADAPTER<T> { \
\
    using BASE = ADAPTER<T>; \
    BASIC_STRING_ADAPTER_USING(BASE); \
\
    ADAPTER##_impl() : BASE('\n', '\0') {} \
\
    ADAPTER##_impl(const T* ptr) : BASE(ptr, '\n', '\0') {} \
\
    ADAPTER##_impl(const T* ptr, const std::size_t length) : BASE(ptr, length, '\n', '\0') {} \
\
    ADAPTER##_impl(const std::string & str) : ADAPTER##_impl(str.data(), str.size()) {} \
\
    void print(const char* str) { \
        const std::size_t len = size(); \
        std::cout << str << ". (size (excluding null): " << len << ") "; \
        const CShared<T> data_ = data(); \
        for (std::size_t i = 0; i <= len; i++) { \
            if (data_.ptr()[i] == get_end_of_file()) { \
                std::cout << "\\0" << ' '; \
            } else { \
                std::cout << data_.ptr()[i] << ' '; \
            } \
        } \
        std::cout << '\n'; \
    } \
\
    void append(const std::string & s) { \
        append(s.data(), s.length()); \
    } \
\
    void append(const T* s) { \
        append_(ADAPTER##_impl<T>(s, strlen(s))); \
    } \
\
    void append(const T* s, std::size_t len) { \
        append_(ADAPTER##_impl<T>(s, len)); \
    } \
\
    void append(const T & c) { \
        T s[2] = { c, get_end_of_file() }; \
        append_(ADAPTER##_impl<T>(s, 1)); \
    } \
\
    void insert(const std::size_t pos, const std::size_t len, const std::string & s) { \
        insert_(ADAPTER##_impl<T>(s.data(), s.length()), pos, len); \
    } \
\
    void insert(const std::size_t pos, const std::size_t len, const char* s) { \
        insert_(ADAPTER##_impl<T>(s, strlen(s)), pos, len); \
    } \
\
    void insert(const std::size_t pos, const std::size_t len, const char* s, const std::size_t len2) { \
        insert_(ADAPTER##_impl<T>(s, len2), pos, len); \
    } \
\
    void insert(const std::size_t pos, const std::size_t len, const T & c) { \
        T s[2] = { c, get_end_of_file() }; \
        insert_(ADAPTER##_impl<T>(s, 1), pos, len); \
    } \
\
    void replace(const std::size_t pos, const std::size_t len, const std::string & s) { \
        replace_(ADAPTER##_impl<T>(s.data(), s.length()), pos, len); \
    } \
\
    void replace(const std::size_t pos, const std::size_t len, const char* s) { \
        replace_(ADAPTER##_impl<T>(s, strlen(s)), pos, len); \
    } \
\
    void replace(const std::size_t pos, const std::size_t len, const char* s, const std::size_t len2) { \
        replace_(ADAPTER##_impl<T>(s, len2), pos, len); \
    } \
\
    void replace(const std::size_t pos, const std::size_t len, const char & c) { \
        T s[2] = { c, get_end_of_file() }; \
        replace_(ADAPTER##_impl<T>(s, 1), pos, len); \
    } \
\
    void erase(const std::size_t pos, const std::size_t len) { \
        erase_(pos, len); \
    } \
\
    Shared<char> c_str() override { \
        return c_str_(); \
    } \
\
    const CShared<char> c_str() const override { \
        return c_str_(); \
    } \
\
    const void deleteSelf() const override { \
        delete (ADAPTER##_impl<T>*) this; \
    } \
\
    struct Slice : public BASE::Slice { \
        using BASE_S = typename BASE::Slice; \
        BASIC_STRING_ADAPTER_SLICE_USING_BASE(BASE_S) \
        ADAPTER##_impl<T>* get_origin() override { \
            return reinterpret_cast<ADAPTER##_impl<T>*>(get_origin_()); \
        }; \
    }; \
\
    struct CSlice : public BASE::CSlice { \
        using BASE_S = typename BASE::CSlice; \
        BASIC_STRING_ADAPTER_SLICE_USING_BASE(BASE_S) \
        const ADAPTER##_impl<T>* get_origin() override { \
            return reinterpret_cast<const ADAPTER##_impl<T>*>(get_origin_()); \
        }; \
    }; \
\
    Slice* slice(std::size_t start, std::size_t end) override { \
        return new Slice(this, (begin()+start).index, (begin()+end).index); \
    }; \
\
    const CSlice* cslice(std::size_t start, std::size_t end) const override { \
        return new CSlice(this, (begin()+start).index, (begin()+end).index); \
    }; \
\
    const CSlice* slice(std::size_t start, std::size_t end) const override { \
        return new CSlice(this, (begin()+start).index, (begin()+end).index); \
    }; \
\
    ADAPTER##_impl<T>* clone() override { \
        return new ADAPTER##_impl<T>(*this); \
    } \
\
    const ADAPTER##_impl<T>* clone() const override { \
        return new ADAPTER##_impl<T>(*this); \
    } \
\
    ADAPTER##_impl<T>* newAdapter() override { \
        return new ADAPTER##_impl<T>(); \
    } \
\
    ADAPTER##_impl<T>* newAdapter(const T* ptr) override { \
        return new ADAPTER##_impl<T>(ptr); \
    } \
\
    ADAPTER##_impl<T>* newAdapter(const T* ptr, const std::size_t length)override { \
        return new ADAPTER##_impl<T>(ptr, length); \
    } \
\
    ADAPTER##_impl<T>* newAdapter() const override { \
        return new ADAPTER##_impl<T>(); \
    } \
\
    ADAPTER##_impl<T>* newAdapter(const T* ptr) const override { \
        return new ADAPTER##_impl<T>(ptr); \
    } \
\
    ADAPTER##_impl<T>* newAdapter(const T* ptr, const std::size_t length) const override { \
        return new ADAPTER##_impl<T>(ptr, length); \
    } \
}

    CHAR_ADAPTER_IMPL(VectorAdapter);
    CHAR_ADAPTER_IMPL(ResizingVectorAdapter);
    CHAR_ADAPTER_IMPL(ListAdapter);
    CHAR_ADAPTER_IMPL(ForwardListAdapter);

    using CharVectorAdapter = VectorAdapter_impl<char>;
    using CharResizingVectorAdapter = ResizingVectorAdapter_impl<char>;
    using CharListAdapter = ListAdapter_impl<char>;
    using CharForwardListAdapter = ForwardListAdapter_impl<char>;
    
    using CharAdapter = CharVectorAdapter;
}

template <typename T>
::std::ostream& operator <<(::std::ostream& os, const StringAdapter::BasicStringAdapter<T> & this_) {
    os << "\"";
    auto s = this_.c_str();
    os << s.ptr();
    os << "\"";
    return os;
}

#endif