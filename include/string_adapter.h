#ifndef STRING_ADAPTER_H
#define STRING_ADAPTER_H

#include <cstring>
#include <forward_list>
#include <list>
#include <string>
#include <vector>

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
    typename char_t,
    typename adapter_t,
    typename AdapterMustExtendBasicStringAdapter = std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
>

// ...

// _T is expected for the default char* implementation of classes accepting StringAdapter
//
// this is optional but highly recommended as ( char* , std::string ) based data is extremely common
//
//   also QT variants ( QChar , QString ) are very common in QT based libs/apps (eg KDE, QTCreator, ect )
//     so maybe provide a _QT version as well:
//
//         using YourClass_QT = YourClass<QChar, StringAdapter::CharAdapter_impl<QChar, void>>;
//

using YourClass_T = YourClass<char, StringAdapter::CharAdapter>;

// TYPEDEF

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    // WILL NOT COMPILE
    typedef A<char_t, adapter_t> Deps;

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    // WILL COMPILE
    using Deps = A<char_t, adapter_t>;


// EXAMPLE

    template <
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    typedef Tuple2<Set<const Named<Patch*, char_t, adapter_t>>, Set<const Named<Patch*, char_t, adapter_t>>> Deps;

    using Deps_T = Deps<char, StringAdapter::CharAdapter>;


// TEMPLATE SPECIALIZATION

    template <
        typename T,
        typename char_t,
        typename adapter_t,
        typename AdapterMustExtendBasicStringAdapter = typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type
    >
    struct Named {
        Named() {
            puts("T");
        }
    };

    template <typename T>
    using Named_T = Named<T, char, StringAdapter::CharAdapter>;

    template <typename T, typename char_t, typename adapter_t>
    struct Named<T*, char_t, adapter_t, typename std::enable_if<std::is_base_of<StringAdapter::BasicStringAdapter<char_t>, adapter_t>::value>::type> {
        Named() {
            puts("T*");
        }
    };

    // no need to specify Named_T, the above specialization will handle this for us

    Named_T<int> foo;
    Named_T<int*> foop;

    int main() {}

*/

    template <typename T>
    struct BasicStringAdapter {
        
        typedef T TYPE;
        
        BasicStringAdapter() {} // default constructor
        
        BasicStringAdapter(const T*ptr) {
            puts("BasicStringAdapter( T * ptr ) has not been overridden");
            std::terminate();
        };
        
        BasicStringAdapter(const T*ptr, const size_t length) {
            puts("BasicStringAdapter( T * ptr , size_t length ) has not been overridden");
            std::terminate();
        };
        
        virtual const T & get_item_at_index(const size_t index) const = 0;
        
        const T & operator[] (const size_t index) const {
            return get_item_at_index(index);
        };
        
        // must never be null
        virtual const T* data() const = 0;
        virtual const bool data_is_allocated() const = 0;
        
        virtual const std::vector<T>* data_as_vector() const = 0;
        
        virtual const size_t length() const = 0;
        
        virtual const size_t line_count() const = 0;
        
        virtual void resize(const size_t capacity) = 0;
        
        const size_t size() const {
            return length();
        }
        
        const bool operator == (const BasicStringAdapter<T> & other) const {
            auto * data_ = data();
            auto r = mem_eq(data_, other.data(), length(), other.length()) == 0;
            if (data_is_allocated()) delete[] data_;
            return r;
        }

        const bool operator != (const BasicStringAdapter<T> & other) const {
            return !(*this == other);
        }

        virtual const char * c_str() const = 0;
        virtual const bool c_str_is_allocated() const = 0;
        
        template <typename T2 = T, typename std::enable_if<std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        const int mem_eq(const T2* s1, const T2* s2, const size_t len1, const size_t len2) const {
            return len1 == len2 && std::memcmp(s1, s2, sizeof(T2)*len1);
        }
        
        template <typename T2 = T, typename std::enable_if<!std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        const int mem_eq(const T2* s1, const T2* s2, const size_t len1, const size_t len2) const {
            if (len1 == len2) {
                for(size_t s = 0; s < len1; s++) {
                    if (s1[s] != s2[s]) {
                        // we cannot use s1[s] - s2[s] since - operators might not be available nor sensible
                        return 0;
                    }
                }
                return 0;
            }
            return 1;
        }

        template <typename T2 = T, typename std::enable_if<std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        void mem_cpy(T2* dest, const T2* src, const size_t len) const {
            std::memcpy(dest, src, sizeof(T2)*len);
        }
        
        template <typename T2 = T, typename std::enable_if<!std::is_trivially_copyable<T2>::value, size_t>::type = 0>
        void mem_cpy(T2* dest, const T2* src, const size_t len) const {
            for(size_t s = 0; s < len; s++) {
                dest[s] = src[s];
            }
        }
        
        virtual void append_(const BasicStringAdapter<T> & what) {
            insert_(what, -1);
        }
        
        virtual void insert_(const BasicStringAdapter<T> & what, const size_t pos) = 0;
        
        virtual void replace_(const BasicStringAdapter<T> & what, const size_t pos, const size_t length) {
            
            // be naive
            
            const size_t len = size();
            
            const size_t p = clamp_pos(len, pos);
            
            if (p >= len) {
                insert_(what, p);
                return;
            }
            
            // iterator end - iterator start = length
            //
            const size_t l = clamp_length(len, p, length) - p;
            
            if (l != 0) {
                erase_(p, l);
            }
            
            insert_(what, p);
        }
        
        virtual void erase_(const size_t pos, const size_t length) = 0;
        
        virtual const T get_new_line() const = 0;
        
        virtual ~BasicStringAdapter() {};
        
        const size_t clamp_pos(const size_t pos) const {
            return clamp_pos(size(), pos);
        }

        const size_t clamp_pos(const size_t length, const size_t pos) const {
            return pos == -1 ? length : pos >= length ? length : pos;
        }
        
        const size_t clamp_length(const size_t clamped_pos, const size_t len) const {
            return clamp_length(size(), clamped_pos, len);
        }
        
        const size_t clamp_length(const size_t length, const size_t clamped_pos, const size_t len) const {
            return clamped_pos == length ? 0 : len == -1 ? length : clamped_pos + len >= length ? length : clamped_pos + len;
        }
    };
    
#define BASIC_STRING_ADAPTER_USING_BASE(BASE) \
        using BASE::BASE; \
        using BASE::get_item_at_index; \
        using BASE::operator[]; \
        using BASE::data; \
        using BASE::data_is_allocated; \
        using BASE::data_as_vector; \
        using BASE::length; \
        using BASE::line_count; \
        using BASE::resize; \
        using BASE::size; \
        using BASE::c_str; \
        using BASE::c_str_is_allocated; \
        using BASE::mem_eq; \
        using BASE::mem_cpy; \
        using BASE::append_; \
        using BASE::insert_; \
        using BASE::replace_; \
        using BASE::erase_; \
        using BASE::clamp_pos; \
        using BASE::clamp_length; \
        using BASE::get_new_line; \

#define BASIC_STRING_ADAPTER_USING(BASE) \
        BASIC_STRING_ADAPTER_USING_BASE(BASE) \
        using BASE::operator=; \
        using BASE::operator==; \
        using BASE::operator!=; \
        using BASE::init; \
        using BASE::split; \
        using BASE::lines; \
        using BASE::begin; \
        using BASE::end; \

    // a reverse string
    template<typename T, typename DERIVED>
    struct ListAdapter : public BasicStringAdapter<T> {
        mutable std::list<T> list;
        mutable size_t len;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

        typename std::list<T>::iterator begin() {
            return list.begin();
        }

        typename std::list<T>::iterator end() {
            return list.end();
        }

        const typename std::list<T>::const_iterator cbegin() const {
            return list.cbegin();
        }

        const typename std::list<T>::const_iterator cend() const {
            return list.cend();
        }

        const typename std::list<T>::const_iterator begin() const {
            return cbegin();
        }

        const typename std::list<T>::const_iterator end() const {
            return cend();
        }

        ListAdapter() {
            init(nullptr);
        }
        
        ListAdapter(const T * ptr) {
            init(ptr);
        }
        
        ListAdapter(const T * ptr, const size_t length) {
            init(ptr, length);
        }
        
        ListAdapter(const ListAdapter & other) {
            list = other.list;
            len = other.len;
        }
        
        const ListAdapter & operator=(const ListAdapter & other) const {
            list = other.list;
            len = other.len;
            return *this;
        }
        
        ListAdapter(ListAdapter && other) {
            list = other.list;
            len = other.len;
        }
        
        const ListAdapter & operator=(ListAdapter && other) const {
            list = other.list;
            len = other.len;
            return *this;
        }

        void init(const T * ptr) {
            
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr) {
                this->len = 0;
                this->list = {};
                this->list.emplace_back(eof);
                return;
            }
            
            size_t length = 0;
            
            while(true) {
                if (ptr[length] == eof) {
                    break;
                }
                length++;
            }
            
            init(ptr, length);
            
        }
        
        void init(const T * ptr, const size_t length) {
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr || length == 0) {
                this->len = 0;
                this->list = {};
                this->list.emplace_back(eof);
                return;
            }
            
            this->list = {};
            len = length;
            if (length != 0) {
                for(size_t s = 0; s < length; s++) {
                    this->list.emplace_back(ptr[s]);
                }
                this->list.emplace_back(eof);
            }
        }

        const T & get_item_at_index(const size_t index) const override {
            return *std::next(list.begin(), index);
        }

        const size_t length() const override {
            return len;
        }

        const T * data() const override {
            T * d = new T[len+1];
            size_t i = 0;
            for (const T & copy : list) {
                d[i] = copy;
                i++;
            }
            return d;
        }

        const bool data_is_allocated() const override {
            return true;
        }

        const std::vector<T>* data_as_vector() const override {
            return nullptr;
        }

        const std::vector<DERIVED> split(const T & splitter) const {
            std::vector<DERIVED> vec;
            const T eof = static_cast<const DERIVED*>(this)->get_end_of_file();
            DERIVED c;
            for (const T & t : *this) {
                if (t == splitter) {
                    vec.push_back(c);
                    c = DERIVED();
                } else {
                    T s[2] = { t, eof };
                    c.append_(DERIVED(s, 1));
                }
            }
            vec.push_back(c);
            return vec;
        }

        const std::vector<DERIVED> lines() const {
            const size_t len = length();
            if (len == 0) {
                return {};
            }

            return split(get_new_line());
        }

        const size_t line_count() const override {
            return lines().size();
        }

        void resize(const size_t capacity) override {
            const size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    list.resize(capacity);
                }
            }
        }

        void insert_(const BasicStringAdapter<T> & what, const size_t pos) override {
            auto * vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                list.insert(std::next(list.begin(), clamp_pos(pos)), vec->begin(), vec->end()-1);
                len += what.length();
            } else {
                const T * p = what.data();
                const size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(size_t s = 0; s < l; s++) {
                        v[s] = p[s];
                    }
                }
                if (what.data_is_allocated()) delete[] p;
                list.insert(std::next(list.begin(), clamp_pos(pos)), v.begin(), v.end());
                len += l;
            }
        }
        
        virtual void erase_(const size_t pos, const size_t length) override {
            
            // be naive
            
            const size_t len_ = size();
            
            const size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const size_t l = clamp_length(len_, p, length);
            
            if (pos == 0 && l == len_) {
                list = {};
                len = 0;
            } else if (l != 0) {
                list.erase(std::next(list.begin(), p), std::next(list.begin(), l));
                len = len_ - (l - p);
            }
        }
        
        ~ListAdapter() override {
        }
    };

    // a reverse string
    template<typename T, typename DERIVED>
    struct ForwardListAdapter : public BasicStringAdapter<T> {
        mutable std::forward_list<T> forward_list;
        mutable size_t len;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

        ForwardListAdapter() {
            init(nullptr);
        }
        
        ForwardListAdapter(const T * ptr) {
            init(ptr);
        }
        
        ForwardListAdapter(const T * ptr, const size_t length) {
            init(ptr, length);
        }
        
        ForwardListAdapter(const ForwardListAdapter & other) {
            forward_list = other.forward_list;
            len = other.len;
        }
        
        typename std::forward_list<T>::iterator begin() {
            return forward_list.begin();
        }

        typename std::forward_list<T>::iterator end() {
            return forward_list.end();
        }

        const typename std::forward_list<T>::const_iterator cbegin() const {
            return forward_list.cbegin();
        }

        const typename std::forward_list<T>::const_iterator cend() const {
            return forward_list.cend();
        }

        const typename std::forward_list<T>::const_iterator begin() const {
            return cbegin();
        }

        const typename std::forward_list<T>::const_iterator end() const {
            return cend();
        }

        const ForwardListAdapter & operator=(const ForwardListAdapter & other) const {
            forward_list = other.forward_list;
            len = other.len;
            return *this;
        }
        
        ForwardListAdapter(ForwardListAdapter && other) {
            forward_list = other.forward_list;
            len = other.len;
        }
        
        const ForwardListAdapter & operator=(ForwardListAdapter && other) const {
            forward_list = other.forward_list;
            len = other.len;
            return *this;
        }

        void init(const T * ptr) {
            
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr) {
                this->len = 0;
                this->forward_list = {};
                this->forward_list.emplace_front(eof);
                return;
            }
            
            size_t length = 0;
            
            while(true) {
                if (ptr[length] == eof) {
                    break;
                }
                length++;
            }
            
            init(ptr, length);
            
        }
        
        void init(const T * ptr, const size_t length) {
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr || length == 0) {
                this->forward_list = {};
                this->forward_list.emplace_front(eof);
                this->len = 0;
                return;
            }
            
            this->forward_list = {};
            len = length;
            if (length != 0) {
                for(size_t s = 0; s < length; s++) {
                    this->forward_list.emplace_front(ptr[s]);
                }
                this->forward_list.emplace_front(eof);
            }
        }

        const T & get_item_at_index(const size_t index) const override {
            return *std::next(forward_list.begin(), index);
        }

        const size_t length() const override {
            return len;
        }

        const T * data() const override {
            T * d = new T[len+1];
            size_t i = 0;
            for (const T & copy : forward_list) {
                d[i] = copy;
                i++;
            }
            return d;
        }

        const bool data_is_allocated() const override {
            return true;
        }

        const std::vector<T>* data_as_vector() const override {
            return nullptr;
        }

        const std::vector<DERIVED> split(const T & splitter) const {
            std::vector<DERIVED> vec;
            const T eof = static_cast<const DERIVED*>(this)->get_end_of_file();
            DERIVED c;
            for (const T & t : *this) {
                if (t == splitter) {
                    vec.push_back(c);
                    c = DERIVED();
                } else {
                    T s[2] = { t, eof };
                    c.append_(DERIVED(s, 1));
                }
            }
            vec.push_back(c);
            return vec;
        }

        const std::vector<DERIVED> lines() const {
            const size_t len = length();
            if (len == 0) {
                return {};
            }

            return split(get_new_line());
        }

        const size_t line_count() const override {
            return lines().size();
        }

        void resize(const size_t capacity) override {
            const size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    forward_list.resize(capacity);
                }
            }
        }

        void insert_(const BasicStringAdapter<T> & what, const size_t pos) override {
            auto * vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                forward_list.insert_after(std::next(forward_list.begin(), clamp_pos(pos)), vec->begin(), vec->end()-1);
                len += what.length();
            } else {
                const T * p = what.data();
                const size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(size_t s = 0; s < l; s++) {
                        v[s] = p[s+1];
                    }
                }
                if (what.data_is_allocated()) delete[] p;
                forward_list.insert_after(std::next(forward_list.begin(), clamp_pos(pos)), v.begin(), v.end());
                len += l;
            }
        }
        
        virtual void erase_(const size_t pos, const size_t length) override {
            
            // be naive
            
            const size_t len_ = size();
            
            const size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const size_t l = clamp_length(len_, p, length);
            
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
        
        ~ForwardListAdapter() override {
        }
    };

    template<typename T, typename DERIVED>
    struct VectorAdapter : public BasicStringAdapter<T> {
        
        mutable std::vector<T> vector;
        
        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

        VectorAdapter() {
            init(nullptr);
        }
        
        VectorAdapter(const T * ptr) {
            init(ptr);
        }
        
        VectorAdapter(const T * ptr, const size_t length) {
            init(ptr, length);
        }
        
        VectorAdapter(const VectorAdapter & other) {
            vector = other.vector;
        }
        
        typename std::vector<T>::iterator begin() {
            return vector.begin();
        }

        typename std::vector<T>::iterator end() {
            return vector.end();
        }

        const typename std::vector<T>::const_iterator cbegin() const {
            return vector.cbegin();
        }

        const typename std::vector<T>::const_iterator cend() const {
            return vector.cend();
        }

        const typename std::vector<T>::const_iterator begin() const {
            return cbegin();
        }

        const typename std::vector<T>::const_iterator end() const {
            return cend();
        }

        const VectorAdapter & operator=(const VectorAdapter & other) const {
            vector = other.vector;
            return *this;
        }
        
        VectorAdapter(VectorAdapter && other) {
            vector = other.vector;
        }
        
        const VectorAdapter & operator=(VectorAdapter && other) const {
            vector = other.vector;
            return *this;
        }

        void init(const T * ptr) {
            
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr) {
                this->vector = std::move(std::vector<T>(1));
                this->vector[0] = eof;
                return;
            }
            
            size_t length = 0;
            
            while(true) {
                if (ptr[length] == eof) {
                    break;
                }
                length++;
            }
            
            init(ptr, length);
            
        }
        
        void init(const T * ptr, const size_t length) {
            const T eof = static_cast<DERIVED*>(this)->get_end_of_file();
            
            const T empty[1] { eof };
            
            if (ptr == nullptr || length == 0) {
                this->vector = std::move(std::vector<T>(1));
                this->vector[0] = eof;
                return;
            }
            
            vector = std::move(std::vector<T>(length+1));
            if (length != 0) {
                for(size_t s = 0; s < length; s++) {
                    vector[s] = ptr[s];
                }
                vector[length] = eof;
            }
        }
        
        const T & get_item_at_index(const size_t index) const override {
            return vector[index];
        }
        
        const T * data() const override {
            return vector.data();
        }
        
        const bool data_is_allocated() const override {
            return false;
        }

        const std::vector<T>* data_as_vector() const override {
            return &vector;
        }
        
        const size_t length() const override {
            return vector.size()-1;
        }
        
        const std::vector<DERIVED> split(const T & splitter) const {
            std::vector<DERIVED> vec;
            const T eof = static_cast<const DERIVED*>(this)->get_end_of_file();
            DERIVED c;
            for (const T & t : *this) {
                if (t == splitter) {
                    vec.push_back(c);
                    c = DERIVED();
                } else {
                    T s[2] = { t, eof };
                    c.append_(DERIVED(s, 1));
                }
            }
            vec.push_back(c);
            return vec;
        }

        const std::vector<DERIVED> lines() const {
            const size_t len = length();
            if (len == 0) {
                return {};
            }

            return split(get_new_line());
        }
        
        const size_t line_count() const override {
            return lines().size();
        }

        void resize(const size_t capacity) override {
            const size_t len_ = length();
            if (capacity != len_) {
                if (capacity == 0) {
                    erase_(0, len_);
                } else {
                    vector.resize(capacity);
                }
            }
        }
        
        void insert_(const BasicStringAdapter<T> & what, const size_t pos) override {
            auto * vec = what.data_as_vector();
            if (vec != nullptr) {
                // we can take advantage of vector
                vector.insert(vector.begin() + clamp_pos(pos), vec->begin(), vec->end()-1);
            } else {
                const T * p = what.data();
                const size_t l = what.length();
                auto v = std::vector<T>(l);
                if (l != 0) {
                    for(size_t s = 0; s < l; s++) {
                        v[s] = p[s];
                    }
                }
                if (what.data_is_allocated()) delete[] p;
                vector.insert(vector.begin() + clamp_pos(pos), v.begin(), v.end());
            }
        }
        
        virtual void erase_(const size_t pos, const size_t length) override {
            
            // be naive
            
            const size_t len_ = size();
            
            const size_t p = clamp_pos(len_, pos);
            
            if (p >= len_) {
                // nothing to erase
                return;
            }
            
            const size_t l = clamp_length(len_, p, length);
            
            if (pos == 0 && l == len_) {
                vector = {};
            } else if (l != 0) {
                vector.erase(vector.begin() + p, vector.begin() + l);
            }
        }
        
        ~VectorAdapter() override {
        }
    };
    
    template <typename T, typename DERIVED>
    struct ResizingVectorAdapter : public VectorAdapter<T, DERIVED> {
        
        using BASE = VectorAdapter<T, DERIVED>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);
        using BASE::vector;
        
        void insert_(const BasicStringAdapter<T> & what, const size_t pos) override {
            vector.reserve(length() + what.length() + 1);
            BASE::insert_(what, pos);
        }
        
        virtual void erase_(const size_t pos, const size_t length) override {
            BASE::erase_(pos, length);
            vector.shrink_to_fit();
        }
        
        ~ResizingVectorAdapter() override {
        }
    };
    
#define ADAPTER_IMPL(ADAPTER) template <typename char_t, typename UNUSED = void> \
struct ADAPTER##_impl : ADAPTER<char_t, ADAPTER##_impl<char_t, UNUSED>> { \
    using BASE = ADAPTER<char_t, ADAPTER##_impl<char_t, UNUSED>>; \
    BASIC_STRING_ADAPTER_USING(BASE); \
\
    ADAPTER##_impl(const std::string & str) { \
        init(str.data(), str.length()); \
    } \
\
    const bool operator == (const char * other) const { \
        auto * data_ = data(); \
        bool r = mem_eq(data_, other, length(), strlen(other)) == 0; \
        if (data_is_allocated()) delete[] data_; \
        return r; \
    } \
\
    const bool operator != (const char * other) const { \
        return !(*this == other); \
    } \
\
    void print(const char * str) \
    { \
        const size_t len = size(); \
        std::cout << str << ". (size (excluding null): " << len << ") "; \
        const char_t * data_ = data(); \
        const char_t eof = get_end_of_file(); \
        for (size_t i = 0; i <= len; i++) { \
            if (data_[i] == eof) { \
                std::cout << "\\0" << ' '; \
            } else { \
                std::cout << data_[i] << ' '; \
            } \
        } \
        std::cout << '\n'; \
        if (data_is_allocated()) { delete[] data_; } \
    } \
\
    void append(const std::string & s) { \
        append(s.data(), s.length()); \
    } \
\
    void append(const char_t * s) { \
        append_(ADAPTER##_impl<char_t, UNUSED>(s, strlen(s))); \
    } \
\
    void insert(const size_t pos, const size_t len, const std::string & s) { \
        insert_(ADAPTER##_impl<char_t, UNUSED>(s.data(), s.length()), pos, len); \
    } \
\
    void insert(const size_t pos, const size_t len, const char * s) { \
        insert_(ADAPTER##_impl<char_t, UNUSED>(s, strlen(s)), pos, len); \
    } \
\
    void replace(const size_t pos, const size_t len, const std::string & s) { \
        replace_(ADAPTER##_impl<char_t, UNUSED>(s.data(), s.length()), pos, len); \
    } \
\
    void replace(const size_t pos, const size_t len, const char * s) { \
        replace_(ADAPTER##_impl<char_t, UNUSED>(s, strlen(s)), pos, len); \
    } \
\
    void erase(const size_t pos, const size_t len) { \
        erase_(pos, len); \
    } \
\
    const char_t * c_str() const override { \
        return data(); \
    } \
\
    const bool c_str_is_allocated() const override { \
        return data_is_allocated(); \
    } \
\
    const char_t get_new_line() const override { \
        return '\n'; \
    } \
\
    const char_t get_end_of_file() const { \
        return '\0'; \
    } \
}

    ADAPTER_IMPL(VectorAdapter);
    ADAPTER_IMPL(ResizingVectorAdapter);
    ADAPTER_IMPL(ListAdapter);
    ADAPTER_IMPL(ForwardListAdapter);

    using CharVectorAdapter = VectorAdapter_impl<char, void>;
    using CharResizingVectorAdapter = ResizingVectorAdapter_impl<char, void>;
    using CharListAdapter = ListAdapter_impl<char, void>;
    using CharForwardListAdapter = ForwardListAdapter_impl<char, void>;
    
    using CharAdapter = CharVectorAdapter;
}

#endif