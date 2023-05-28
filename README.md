# StringAdapter

StringAdapter is a C++11 compatible library, exposing string manipulation capabilities for any given type

we provide 4 types of adapters: `ListAdapter`, `ForwardListAdapter`, `VectorAdapter`, and `ResizingListAdapter`

for demonstration we provide a `char` implementation of each adapter

`CharVectorAdapter` using `VectorAdapter`

`CharResizingVectorAdapter` using `ResizingVectorAdapter`

`CharListAdapter` using `ListAdapter`

`CharForwardListAdapter` using `ForwardListAdapter`

## VectorAdapter

the most basic and common adapter, utilises `std::vector` to hold its data

equivilant to a `PointerAdapter`

`PointerAdapter` was going to be included however pointer arithmatic proved to be too difficult and `std::vector` implements most of what we needed

## ResizingVectorAdapter

a less common adapter, utilises `std::vector` to hold its data, automatically grows and shrinks to fit its data, useful for very large data

equivilant to a `PointerAdapter`

`PointerAdapter` was going to be included however pointer arithmatic proved to be too difficult and `std::vector` implements most of what we needed

## ListAdapter

a common adapter, utilises `std::list` to hold its data, this is equivilant to a `linked list`

## ForwardListAdapter

a common adapter, utilises `std::forward_list` to hold its data, this is equivilant to a `reversed linked list`

all data entered is reversed, `123\0` -> `\0321`

`index 0` is the item after the `\0`

## base functions

the base adapter `BasicStringAdapter<T>` provide the following functions

```c
virtual const T & get_item_at_index(const size_t index) const
```
called by `operator[]`

```c
const T & operator[] (const size_t index) const
```
your basic array index operator, `arr[5] = value`

```c
virtual const T* data() const
```
returns a `contigous array` of data

this must never return `nullptr`, `NULL`, an `address of 0x0`, or an `integer zero ( return 0; )`

```c
virtual const bool data_is_allocated() const
```
this must always be checked when using `data()`

the returned pointer must be `freed` via the `delete[]` operator

```c
virtual const std::vector<T>* data_as_vector() const
```
returns the data as a `std::vector`

it `may return nullptr`

the returned pointer must be `freed` via the `delete` operator

```c
virtual const size_t length() const
```
returns the length of the data, excluding the `EOF` marker (the `get_end_of_file()` function)

```c
virtual const size_t line_count() const
```
returns the number of lines contained in the data, using the `get_new_line()` function

```c
virtual void resize(const size_t capacity)
```
resizes the adapter to the specified `capacity`

```c
const size_t size() const
```
returns `length()`

```c
const bool operator == (const BasicStringAdapter<T> & other) const
const bool operator != (const BasicStringAdapter<T> & other) const
```
compares the contents of one `adapter` with the contents of another `adapter`

```c
virtual const char * c_str() const
virtual const bool c_str_is_allocated() const
```
same usage as `data()` and `data_is_allocated()`

returns the contents as a `char *` string

this should be a one way `T -> char*` conversion for each element `T`

`IMPORTANT:` there is no `char* -> T` conversion as `std::string` requires since `reversing a conversion may be impossible`

```c
const int mem_eq(const T2* s1, const T2* s2, const size_t len1, const size_t len2) const
void mem_cpy(T2* dest, const T2* src, const size_t len) const
```
compares content

for `is_trivially_copyable` `std::memcmp`/`std::memcpy` is used

for others a `for loop` is used

```c
virtual void append_(const BasicStringAdapter<T> & what)
```
appends the contents of the specified adapter `what` to the end of `this` adapter

this calls `insert_` with a `pos` of `-1`

```c
virtual void insert_(const BasicStringAdapter<T> & what, const size_t pos)
```
inserts the contents of the specified adapter `what` to the specified `pos` of `this` adapter

`pos` is bound to the start and end of the adapter

```c
virtual void replace_(const BasicStringAdapter<T> & what, const size_t pos, const size_t length)
```
replaces the contents of `this` adapter, at the specified position `pos`, with the specified length `length`, with the contents of the specified adapter `what`

`pos` is bound to the start and end of the adapter
`length` is bound to the end of the adapter in accordance to the `pos`

```c
virtual void erase_(const size_t pos, const size_t length)
```
erases the contents of `this` adapter, at the specified position `pos`, with the specified length `length`

`pos` is bound to the start and end of the adapter
`length` is bound to the end of the adapter in accordance to the `pos`

```c
const size_t clamp_pos(const size_t pos) const
const size_t clamp_pos(const size_t length, const size_t pos) const
const size_t clamp_length(const size_t clamped_pos, const size_t len) const
const size_t clamp_length(const size_t length, const size_t clamped_pos, const size_t len) const
```
clamps the `pos` and `len` based on the specified `length`

`length` is typically the result of `size()` or `length()`

```c
virtual const T get_new_line() const
```
obtains the `new line` element, for `char` this is typically `\n`

## additional functions

the 4 adapters provide everything that `BasicStringAdapter<T>` provides above (`basic functions`)

with the following additional functions

```c
IMPL::iterator begin()
const IMPL::const_iterator begin() const
const IMPL::const_iterator cbegin() const

IMPL::iterator end()
const IMPL::const_iterator end() const
const IMPL::const_iterator cend() const
```
`adapter specific` `iterators` for `begin` and `end`

required for `for loop` (specifically `for(const T & value : adapter)`) capabilities

```c
void init(const T * ptr)
void init(const T * ptr, size_t length)
```
```c
    // intended to be used in constructors:
        ListAdapter() {
            init(nullptr);
        }
        
        ListAdapter(const T * ptr) {
            init(ptr);
        }
        
        ListAdapter(const T * ptr, const size_t length) {
            init(ptr, length);
        }
```

```c
std::vector<DERIVED> split(const T & splitter) const
```
splits an `adapter` into the same `adapter` delimited by the specified `splitter`

`abcd<SPLITTER>efg` -> `[ "abcd", "efg" ]`

```c
std::vector<DERIVED> lines() const
```
splits an `adapter` into the same `adapter` delimited by the `new line` item, returned by `get_new_line()`

`abcd\nefg` -> `[ "abcd", "efg" ]`

```c
std::vector<DERIVED> lines() const
```
splits an `adapter` into the same `adapter` delimited by the `new line` item, returned by `get_new_line()`

`abcd\nefg` -> `[ "abcd", "efg" ]`

## Char Adapter functions

all `Char Adapters` provide everything `the 4 adapters` provides above (`basic functions` + `additional functions`)

with the following additional functions

```c
const char_t get_end_of_file() const
```
returns the `EOF` marker, for `char` this is the `null terminator` which is `\0`

```c
void append(const std::string & s)
void append(const char_t * s)
```
calls `append_`

```c
void insert(const size_t pos, const size_t len, const std::string & s)
void insert(const size_t pos, const size_t len, const char * s)
```
calls `insert_`

```c
void replace(const size_t pos, const size_t len, const std::string & s)
void replace(const size_t pos, const size_t len, const char * s)
```
calls `replace_`

```c
void erase(const size_t pos, const size_t len)
```
calls `erase_`


# cloning

## as a non-submodule

```shell script
git clone --recursive https://github.com/mgood7123/StringAdapter
```

## as a submodule

```shell script
git submodule add https://github.com/mgood7123/StringAdapter path/to/StringAdapter
git submodule update --init --recursive
```

# building and testing

```shell script
make test_debug
```