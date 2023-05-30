#ifndef STRING_ADAPTER_H
#define STRING_ADAPTER_H

#include <cstring>
#include <forward_list>
#include <list>
#include <string>
#include <vector>
#include <stdexcept>
#include <memory>

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

    template <typename T>
    struct BasicStringAdapter {
        
        typedef T TYPE;

        mutable std::shared_ptr<const T> new_line;
        mutable std::shared_ptr<const T> eof;

        const T & get_new_line() const {
            return *new_line;
        }
        
        const T & get_end_of_file() const {
            return *eof;
        }

        virtual BasicStringAdapter<T>* clone() = 0;
        
        virtual const T & get_item_at_index(const size_t index) const = 0;
        
        const T & operator[] (const size_t index) const {
            return get_item_at_index(index);
        };
        
        // must never be null
        virtual const T* data() const = 0;
        virtual const bool data_is_allocated() const = 0;
        
        virtual const std::vector<T>* data_as_vector() const = 0;
        
        virtual const size_t length() const = 0;
        
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

        virtual const void deleteSelf() const = 0;
        virtual const BasicStringAdapter<T>* newAdapter() const = 0;
        virtual const BasicStringAdapter<T>* newAdapter(const T* ptr) const = 0;
        virtual const BasicStringAdapter<T>* newAdapter(const T* ptr, const size_t length) const = 0;

        const std::vector<std::unique_ptr<BasicStringAdapter<T>>> split(const T & splitter) const {
            std::vector<BasicStringAdapter<T>*> vec;
            BasicStringAdapter<T>* c = newAdapter();
            for (const T & t : *this) {
                if (t == splitter) {
                    vec.push_back(make_unique(c));
                    c = newAdapter();
                } else {
                    T s[2] = { t, get_end_of_file() };
                    BasicStringAdapter<T>* tmp = newAdapter(s, 1);
                    c->append_(*tmp);
                    tmp->deleteSelf();
                }
            }
            vec.push_back(c);
            return vec;
        }

        const std::vector<std::unique_ptr<BasicStringAdapter<T>>> lines() const {
            const size_t len = length();
            if (len == 0) {
                return {};
            }

            return split(get_new_line());
        }

        const size_t line_count() const {
            return lines().size();
        }

        const bool operator == (const T * other) const {
            auto * data_ = data();
            bool r = mem_eq(data_, other, length(), strlen(other)) == 0;
            if (data_is_allocated()) delete[] data_;
            return r;
        }

        const bool operator != (const T * other) const {
            return !(*this == other);
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
        using BASE::resize; \
        using BASE::size; \
        using BASE::c_str; \
        using BASE::c_str_is_allocated; \
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
        using BASE::mem_eq; \
        using BASE::mem_cpy; \
        using BASE::operator==; \
        using BASE::operator!=; \
        using BASE::new_line; \
        using BASE::eof; \

#define BASIC_STRING_ADAPTER_USING(BASE) \
        BASIC_STRING_ADAPTER_USING_BASE(BASE) \
        using BASE::operator=; \
        using BASE::init; \
        using BASE::begin; \
        using BASE::end; \

#define INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(ITEM) std::shared_ptr<T>(new T(ITEM))

#define BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(NAME, T) \
        NAME(const T & new_line, const T & eof) { \
            init(nullptr, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const T * ptr, const T & new_line, const T & eof) { \
            init(ptr, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const T * ptr, const size_t length, const T & new_line, const T & eof) { \
            init(ptr, length, INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(new_line), INTERNAL______BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTOR__TO_SHARED(eof)); \
        } \
\
        NAME(const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(nullptr, new_line, eof); \
        } \
\
        NAME(const T * ptr, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(ptr, new_line, eof); \
        } \
\
        NAME(const T * ptr, const size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
            init(ptr, length, new_line, eof); \
        } \
\
        void init(const T * ptr, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) { \
\
            if (ptr == nullptr) { \
                init(nullptr, 0, new_line, eof); \
                return; \
            } \
\
            size_t length = 0; \
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

    // a reverse string
    template<typename T>
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

        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(ListAdapter, T)
        
        ListAdapter(const ListAdapter & other) {
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ListAdapter & operator=(const ListAdapter & other) const {
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        ListAdapter(ListAdapter && other) {
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ListAdapter & operator=(ListAdapter && other) const {
            list = other.list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        void init(const T * ptr, const size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
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
                for(size_t s = 0; s < length; s++) {
                    this->list.emplace_back(ptr[s]);
                }
                this->list.emplace_back(get_end_of_file());
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
        
        const void deleteSelf() const override {
            delete (ListAdapter<T>*) this;
        }

        ~ListAdapter() override {
        }
    };

    // a reverse string
    template<typename T>
    struct ForwardListAdapter : public BasicStringAdapter<T> {
        mutable std::forward_list<T> forward_list;
        mutable size_t len;
        mutable std::function<const std::pair<T, T>()> new_line__eof__func;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

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

        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(ForwardListAdapter, T)

        ForwardListAdapter(const ForwardListAdapter & other) {
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ForwardListAdapter & operator=(const ForwardListAdapter & other) const {
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        ForwardListAdapter(ForwardListAdapter && other) {
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const ForwardListAdapter & operator=(ForwardListAdapter && other) const {
            forward_list = other.forward_list;
            len = other.len;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }

        void init(const T * ptr, const size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
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
                for(size_t s = 0; s < length; s++) {
                    this->forward_list.emplace_front(ptr[s]);
                }
                this->forward_list.emplace_front(get_end_of_file());
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

        const void deleteSelf() const override {
            delete (ForwardListAdapter<T>*) this;
        }

        ~ForwardListAdapter() override {
        }
    };

    template<typename T>
    struct VectorAdapter : public BasicStringAdapter<T> {
        mutable std::vector<T> vector;
        mutable std::function<const std::pair<T, T>()> new_line__eof__func;
        
        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

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

        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(VectorAdapter, T)

        VectorAdapter(const VectorAdapter & other) {
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const VectorAdapter & operator=(const VectorAdapter & other) const {
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }
        
        VectorAdapter(VectorAdapter && other) {
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
        }
        
        const VectorAdapter & operator=(VectorAdapter && other) const {
            vector = other.vector;
            new_line = other.new_line;
            eof = other.eof;
            return *this;
        }

        void init(const T * ptr, const size_t length, const std::shared_ptr<const T> & new_line, const std::shared_ptr<const T> & eof) {
            this->new_line = new_line;
            this->eof = eof;

            if (ptr == nullptr || length == 0) {
                this->vector = std::move(std::vector<T>(1));
                this->vector[0] = get_end_of_file();
                return;
            }
            
            vector = std::move(std::vector<T>(length+1));
            if (length != 0) {
                for(size_t s = 0; s < length; s++) {
                    vector[s] = ptr[s];
                }
                vector[length] = get_end_of_file();
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
    
    template <typename T>
    struct ResizingVectorAdapter : public VectorAdapter<T> {
        
        using BASE = VectorAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);
        using BASE::vector;
        using BASE::new_line__eof__func;
        
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
    
#define CHAR_ADAPTER_IMPL(ADAPTER) \
template <typename T, typename UNUSED = void> \
struct ADAPTER##_impl : ADAPTER<T> { \
\
    using BASE = ADAPTER<T>; \
    BASIC_STRING_ADAPTER_USING(BASE); \
\
    ADAPTER##_impl() : BASE('\n', '\0') {} \
\
    ADAPTER##_impl(const T * ptr) : BASE(ptr, '\n', '\0') {} \
\
    ADAPTER##_impl(const T * ptr, const size_t length) : BASE(ptr, length, '\n', '\0') {} \
\
    ADAPTER##_impl(const std::string & str) : ADAPTER##_impl(str.data(), str.size()) {} \
\
    void print(const char * str) \
    { \
        const size_t len = size(); \
        std::cout << str << ". (size (excluding null): " << len << ") "; \
        const T * data_ = data(); \
        for (size_t i = 0; i <= len; i++) { \
            if (data_[i] == get_end_of_file()) { \
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
    void append(const T * s) { \
        append_(ADAPTER##_impl<T, UNUSED>(s, strlen(s))); \
    } \
\
    void insert(const size_t pos, const size_t len, const std::string & s) { \
        insert_(ADAPTER##_impl<T, UNUSED>(s.data(), s.length()), pos, len); \
    } \
\
    void insert(const size_t pos, const size_t len, const char * s) { \
        insert_(ADAPTER##_impl<T, UNUSED>(s, strlen(s)), pos, len); \
    } \
\
    void replace(const size_t pos, const size_t len, const std::string & s) { \
        replace_(ADAPTER##_impl<T, UNUSED>(s.data(), s.length()), pos, len); \
    } \
\
    void replace(const size_t pos, const size_t len, const char * s) { \
        replace_(ADAPTER##_impl<T, UNUSED>(s, strlen(s)), pos, len); \
    } \
\
    void erase(const size_t pos, const size_t len) { \
        erase_(pos, len); \
    } \
\
    const T * c_str() const override { \
        return data(); \
    } \
\
    const bool c_str_is_allocated() const override { \
        return data_is_allocated(); \
    } \
\
    const void deleteSelf() const override { \
        delete (ADAPTER##_impl<T, UNUSED>*) this; \
    } \
\
    ADAPTER##_impl<T, UNUSED>* clone() override { \
        return new ADAPTER##_impl<T, UNUSED>(*this); \
    } \
\
    const ADAPTER##_impl<T, UNUSED>* newAdapter() const override { \
        return new ADAPTER##_impl<T, UNUSED>(); \
    } \
\
    const ADAPTER##_impl<T, UNUSED>* newAdapter(const T* ptr) const override { \
        return new ADAPTER##_impl<T, UNUSED>(ptr); \
    } \
\
    const ADAPTER##_impl<T, UNUSED>* newAdapter(const T* ptr, const size_t length) const override { \
        return new ADAPTER##_impl<T, UNUSED>(ptr, length); \
    } \
}

    CHAR_ADAPTER_IMPL(VectorAdapter);
    CHAR_ADAPTER_IMPL(ResizingVectorAdapter);
    CHAR_ADAPTER_IMPL(ListAdapter);
    CHAR_ADAPTER_IMPL(ForwardListAdapter);

    using CharVectorAdapter = VectorAdapter_impl<char, void>;
    using CharResizingVectorAdapter = ResizingVectorAdapter_impl<char, void>;
    using CharListAdapter = ListAdapter_impl<char, void>;
    using CharForwardListAdapter = ForwardListAdapter_impl<char, void>;
    
    using CharAdapter = CharVectorAdapter;
}

#endif