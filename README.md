# cloning

```sh
# as a non-submodule
git clone --recursive https://github.com/mgood7123/StringAdapter

## as a submodule
git submodule add https://github.com/mgood7123/StringAdapter path/to/StringAdapter
git submodule update --init --recursive

# building and testing
make test_debug
```

# StringAdapter

StringAdapter is a C++11 compatible library, exposing string manipulation capabilities for any given type

ALL PROVIDED DATA STRUCTURES ARE `HASHABLE` AND `COMPARABLE`

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
Slice* slice(std::size_t start, std::size_t end)
const CSlice* cslice(std::size_t start, std::size_t end)
const CSlice* slice(std::size_t start, std::size_t end) const
```
provides a `Slice` of an `adapter`

for reasons of `virtual` functions, `slice` and `cslice` return an `allocated Slice` and `allocated CSlice` object, which must be deleted via the `delete` operator

a `Slice` represents a region of an `adapter`

`Slice` provides

```c
iterator begin()
iterator end()
reverse_iterator rbegin()
reverse_iterator rend()
T & operator[] (const std::size_t index)
const std::size_t get_start()
const std::size_t get_end()
virtual BasicStringAdapter<T> * get_origin()
```

`CSlice` is the `const` version of `Slice`

`CSlice` provides

```c
const const_iterator begin()
const const_iterator end()
const const_iterator cbegin()
const const_iterator cend()
const const_reverse_iterator rbegin()
const const_reverse_iterator rend()
const const_reverse_iterator crbegin()
const const_reverse_iterator crend()
const T & operator[] (const std::size_t index)
const std::size_t get_start()
const std::size_t get_end()
virtual const BasicStringAdapter<T> * get_origin()
```

a `subslice` can be created by using `auto subslice = slice->get_origin()->slice(slice->get_start(), slice->get_end());`

```c
iterator begin()
const const_iterator begin() const
const const_iterator cbegin() const

iterator end()
const const_iterator end() const
const const_iterator cend() const

reverse_iterator rbegin()
const const_reverse_iterator rbegin() const
const const_reverse_iterator crbegin() const

reverse_iterator rend()
const const_reverse_iterator rend() const
const const_reverse_iterator crend() const
```
`adapter specific` `indexed-iterators` for `begin`, `end` and their reverse counterparts `rbegin` and `rend`

required for `for loop` (specifically `for( VALUE_HERE : ADAPTER_HERE )`) capabilities

```c
virtual T & get_item_at_index(const std::size_t index)
virtual const T & get_item_at_index(const std::size_t index) const
```
called by `operator[] (const std::size_t index)`

```c
T & operator[] (const std::size_t index)
const T & operator[] (const std::size_t index) const
```
your basic array index operator, `arr[5] = value`

```c
virtual Shared<T> data()
virtual const CShared<T> data() const
```
returns a `contigous array` of data

this must never return `nullptr`, `NULL`, an `address of 0x0`, or an `integer zero ( return 0; )`

this returns a managed `Shared` object, functions include `T * ptr()`, `std::size_t length()`, and `std::size_t lengthInBytes()`

`Shared` is a wrapper for `std::shared_ptr` and contains additional information such as the length of the pointer array

`CShared` is a `const` version of `Shared`

```c
virtual std::vector<T>* data_as_vector()
virtual const std::vector<T>* data_as_vector() const
```
returns the data as a `std::vector`
```c
virtual const std::size_t length() const
```
returns the length of the data, excluding the `EOF` marker (the `get_end_of_file()` function)

```c
virtual const std::size_t line_count() const
```
returns the number of lines contained in the data, using the `get_new_line()` function

```c
virtual void resize(const std::size_t capacity)
```
resizes the adapter to the specified `capacity`

```c
const std::size_t size() const
```
returns `length()`

```c
const bool operator == (const BasicStringAdapter<T> & other) const
const bool operator != (const BasicStringAdapter<T> & other) const
```
compares the contents of one `adapter` with the contents of another `adapter`

```c
virtual Shared<T> c_str_()
virtual const CShared<T> c_str_() const

virtual Shared<char> c_str()
virtual const Shared<char> c_str() const
```
same usage as `data()` and `data_is_allocated()`

returns the contents as a `Shared<char>` which can be accessed as a `char *` string via `ptr()`

this should be a one way `T -> char*` conversion for each element `T`

thus should be a one way `Shared<T> -> Shared<char>` conversion for each element `Shared<T>`

`IMPORTANT:` there is no `char* -> T` conversion as `std::string` requires since `reversing a conversion may be impossible`

`IMPLEMENTATION:` it is recommended to implement `c_str_()` and friends, and return a suitible `Shared<T>`, subclasses should implement `c_str()` calling `c_str_()` and if needed, converting the result to `Shared<char>`

this allows intermediate conversion of the returned `Shared<T>` required for a string which `data()` might not consider, in the case of `ForwardListAdapter` it flips the string so the EOF is at the end instead of the start, `\0ih` -> `hi\0`

```c
void to_string() const
```
prints this object to `std::cout`, intended for `<< base << derived << more_derived` and so on

```c
const int strcmp(const BasicStringAdapter<T> & other) const
const int strcmp(const char * other) const
const int strcmp(const T * other, const std::size_t len) const
```
compares content

for a `ranged for loop` is used for direct comparison of elements

`strcmp(const char * other)` should be reserved for when `T` is `char`

```c
virtual void append_(const BasicStringAdapter<T> & what)
```
appends the contents of the specified adapter `what` to the end of `this` adapter

this calls `insert_` with a `pos` of `-1`

```c
virtual void insert_(const BasicStringAdapter<T> & what, const std::size_t pos)
```
inserts the contents of the specified adapter `what` to the specified `pos` of `this` adapter

`pos` is bound to the start and end of the adapter

```c
virtual void replace_(const BasicStringAdapter<T> & what, const std::size_t pos, const std::size_t length)
```
replaces the contents of `this` adapter, at the specified position `pos`, with the specified length `length`, with the contents of the specified adapter `what`

`pos` is bound to the start and end of the adapter
`length` is bound to the end of the adapter in accordance to the `pos`

```c
virtual void erase_(const std::size_t pos, const std::size_t length)
```
erases the contents of `this` adapter, at the specified position `pos`, with the specified length `length`

`pos` is bound to the start and end of the adapter
`length` is bound to the end of the adapter in accordance to the `pos`

```c
const std::size_t clamp_pos(const std::size_t pos) const
const std::size_t clamp_pos(const std::size_t length, const std::size_t pos) const
const std::size_t clamp_length(const std::size_t clamped_pos, const std::size_t len) const
const std::size_t clamp_length(const std::size_t length, const std::size_t clamped_pos, const std::size_t len) const
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
void init(const T * ptr)
void init(const T * ptr, std::size_t length)
```
```c
    template <typename T>
    struct ListAdapter : public BasicStringAdapter<T> {
        mutable std::list<T> list;
        mutable std::size_t len;

        using BASE = BasicStringAdapter<T>;
        BASIC_STRING_ADAPTER_USING_BASE(BASE);

        // this uses `init` in constructors
        BASIC_STRING_ADAPTER_USING_BASE_CONSTRUCTORS(ListAdapter, T)
        
        // ...
    };
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
void append(const char_t * s, std::size_t len)
void append(const char_t & c)
```
calls `append_`

```c
void insert(const std::size_t pos, const std::size_t len, const std::string & s)
void insert(const std::size_t pos, const std::size_t len, const char * s)
void insert(const std::size_t pos, const std::size_t len, const char * s, const std::size_t len2)
void insert(const std::size_t pos, const std::size_t len, const char & s)
```
calls `insert_`

```c
void replace(const std::size_t pos, const std::size_t len, const std::string & s)
void replace(const std::size_t pos, const std::size_t len, const char * s)
void replace(const std::size_t pos, const std::size_t len, const char * s, const std::size_t len2)
void replace(const std::size_t pos, const std::size_t len, const char & s)
```
calls `replace_`

```c
void erase(const std::size_t pos, const std::size_t len)
```
calls `erase_`

