#include <gtest/gtest.h>


#include <string_adapter.h>

using namespace StringAdapter;

#define data_is_null(t) { auto * data_ = t.data(); ASSERT_EQ(data_, nullptr); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_not_null(t) { auto * data_ = t.data(); ASSERT_NE(data_, nullptr); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_val(t, i_, value_) { auto * data_ = t.data(); ASSERT_EQ(data_[i_], value_); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_not_val(t, i_, value_) { auto * data_ = t.data(); ASSERT_NE(data_[i_], value_); if (t.data_is_allocated()) { delete[] data_; } }

TEST(ResizingVectorAdapter_Core, initialization_data_checking) {
    CharResizingVectorAdapter a;
    ASSERT_EQ(a.vector.size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector[0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(VectorAdapter_Core, initialization_data_checking) {
    CharVectorAdapter a;
    ASSERT_EQ(a.vector.size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector[0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ListAdapter_Core, initialization_data_checking) {
    CharForwardListAdapter a;
    ASSERT_EQ(a.len, 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ForwardListAdapter_Core, initialization_data_checking) {
    CharListAdapter a;
    ASSERT_EQ(a.len, 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ResizingVectorAdapter_Core, initialization2_data_checking) {
    CharResizingVectorAdapter a = "hello";
    ASSERT_EQ(a.vector.size(), 6);
    ASSERT_EQ(a.vector.capacity(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector[5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(VectorAdapter_Core, initialization2_data_checking) {
    CharVectorAdapter a = "hello";
    ASSERT_EQ(a.vector.size(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector[5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(ListAdapter_Core, initialization2_data_checking) {
    CharListAdapter a = "hello";
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(ForwardListAdapter_Core, initialization2_data_checking) {
    CharForwardListAdapter a = "hello";
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[5], 'h');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, 'h');
}

TEST(ResizingVectorAdapter_Core, iterator_begin) {
    CharResizingVectorAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(VectorAdapter_Core, iterator_begin) {
    CharVectorAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ListAdapter_Core, iterator_begin) {
    CharForwardListAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ForwardListAdapter_Core, iterator_begin) {
    CharListAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ResizingVectorAdapter_Core, iterator_end) {
    CharResizingVectorAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(VectorAdapter_Core, iterator_end) {
    CharVectorAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(ListAdapter_Core, iterator_end) {
    CharListAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(ForwardListAdapter_Core, iterator_end) {
    CharForwardListAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), 'h');
}

TEST(ResizingVectorAdapter_Core, slice) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(VectorAdapter_Core, slice) {
    CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ListAdapter_Core, slice) {
    CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, slice) {
    CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'o');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, slice_iterator_begin) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(VectorAdapter_Core, slice_iterator_begin) {
    CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ListAdapter_Core, slice_iterator_begin) {
    CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ForwardListAdapter_Core, slice_iterator_begin) {
    CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'o');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, slice_iterator_end) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(VectorAdapter_Core, slice_iterator_end) {
    CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ListAdapter_Core, slice_iterator_end) {
    CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, slice_iterator_end) {
    CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, cslice) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(VectorAdapter_Core, cslice) {
    CharVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ListAdapter_Core, cslice) {
    CharListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, cslice) {
    CharForwardListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'o');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, cslice_iterator_begin) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(VectorAdapter_Core, cslice_iterator_begin) {
    CharVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ListAdapter_Core, cslice_iterator_begin) {
    CharListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ForwardListAdapter_Core, cslice_iterator_begin) {
    CharForwardListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'o');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, cslice_iterator_end) {
    CharResizingVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(VectorAdapter_Core, cslice_iterator_end) {
    CharVectorAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ListAdapter_Core, cslice_iterator_end) {
    CharListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, cslice_iterator_end) {
    CharForwardListAdapter a = "hello";
    auto sp = a.cslice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, const_iterator_begin) {
    const CharResizingVectorAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(VectorAdapter_Core, const_iterator_begin) {
    const CharVectorAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ListAdapter_Core, const_iterator_begin) {
    const CharForwardListAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ForwardListAdapter_Core, const_iterator_begin) {
    const CharListAdapter a;
    ASSERT_EQ(*a.begin(), '\0');
}

TEST(ResizingVectorAdapter_Core, const_iterator_end) {
    const CharResizingVectorAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(VectorAdapter_Core, const_iterator_end) {
    const CharVectorAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(ListAdapter_Core, const_iterator_end) {
    const CharListAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), '\0');
}

TEST(ForwardListAdapter_Core, const_iterator_end) {
    const CharForwardListAdapter a = "hello";
    ASSERT_EQ(*(a.end()-1), 'h');
}

TEST(ResizingVectorAdapter_Core, const_slice) {
    const CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(VectorAdapter_Core, const_slice) {
    const CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ListAdapter_Core, const_slice) {
    const CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'e');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, const_slice) {
    const CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(s[0], 'o');
    ASSERT_EQ(s[1], 'l');
    ASSERT_EQ(s[2], 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, const_slice_iterator_begin) {
    const CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(VectorAdapter_Core, const_slice_iterator_begin) {
    const CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ListAdapter_Core, const_slice_iterator_begin) {
    const CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'e');
    delete sp;
}

TEST(ForwardListAdapter_Core, const_slice_iterator_begin) {
    const CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*s.begin(), 'o');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, const_slice_iterator_end) {
    const CharResizingVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(VectorAdapter_Core, const_slice_iterator_end) {
    const CharVectorAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ListAdapter_Core, const_slice_iterator_end) {
    const CharListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ForwardListAdapter_Core, const_slice_iterator_end) {
    const CharForwardListAdapter a = "hello";
    auto sp = a.slice(1, 3);
    auto & s = *sp;
    ASSERT_EQ(*(s.end()-1), 'l');
    delete sp;
}

TEST(ResizingVectorAdapter_Core, append1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a.vector.size(), 6);
    ASSERT_EQ(a.vector.capacity(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector[5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(VectorAdapter_Core, append1) {
    CharVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a.vector.size(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector[5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(ListAdapter_Core, append1) {
    CharListAdapter a;
    a.append("hello");
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(ForwardListAdapter_Core, append1) {
    CharForwardListAdapter a;
    a.append("hello");
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[5], 'h');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, 'h');
}

TEST(ResizingVectorAdapter_Core, erase1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.vector.size(), 3);
    ASSERT_EQ(a.vector.capacity(), 3);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a.vector[1], 'o');
    ASSERT_EQ(a.vector[2], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, '\0');
}

TEST(VectorAdapter_Core, erase1) {
    CharVectorAdapter a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.vector.size(), 3);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a.vector[1], 'o');
    ASSERT_EQ(a.vector[2], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, '\0');
}

TEST(ListAdapter_Core, erase1) {
    CharListAdapter a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.len, 2);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, '\0');
}

TEST(ForwardListAdapter_Core, erase1) {
    CharForwardListAdapter a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.len, 2);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], 'h');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, 'h');
}

TEST(ResizingVectorAdapter_Core, replace1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.vector.size(), 8);
    ASSERT_EQ(a.vector.capacity(), 8);
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a.vector[1], 'A');
    ASSERT_EQ(a.vector[5], 'E');
    ASSERT_EQ(a.vector[6], 'o');
    ASSERT_EQ(a.vector[7], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'A');
    ASSERT_EQ(a[5], 'E');
    ASSERT_EQ(a[6], 'o');
    ASSERT_EQ(a[7], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'A');
    data_is_val(a, 5, 'E');
    data_is_val(a, 6, 'o');
    data_is_val(a, 7, '\0');
    ASSERT_EQ(a, "hAPPLEo");
}

TEST(VectorAdapter_Core, replace1) {
    CharVectorAdapter a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.vector.size(), 8);
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a.vector[0], 'h');
    ASSERT_EQ(a.vector[1], 'A');
    ASSERT_EQ(a.vector[5], 'E');
    ASSERT_EQ(a.vector[6], 'o');
    ASSERT_EQ(a.vector[7], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'A');
    ASSERT_EQ(a[5], 'E');
    ASSERT_EQ(a[6], 'o');
    ASSERT_EQ(a[7], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'A');
    data_is_val(a, 5, 'E');
    data_is_val(a, 6, 'o');
    data_is_val(a, 7, '\0');
    ASSERT_EQ(a, "hAPPLEo");
}

TEST(ListAdapter_Core, replace1) {
    CharListAdapter a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'A');
    ASSERT_EQ(a[5], 'E');
    ASSERT_EQ(a[6], 'o');
    ASSERT_EQ(a[7], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'A');
    data_is_val(a, 5, 'E');
    data_is_val(a, 6, 'o');
    data_is_val(a, 7, '\0');
    ASSERT_EQ(a, "hAPPLEo");
}

TEST(ForwardListAdapter_Core, replace1) {
    CharForwardListAdapter a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a[7], 'h');
    ASSERT_EQ(a[6], 'A');
    ASSERT_EQ(a[5], 'P');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 7, 'h');
    data_is_val(a, 6, 'A');
    data_is_val(a, 5, 'P');
    data_is_val(a, 1, 'o');
    data_is_val(a, 0, '\0');
}

TEST(ResizingVectorAdapter_Core, equals1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a, "hello");
}

TEST(VectorAdapter_Core, equals1) {
    CharVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a, "hello");
}

TEST(ListAdapter_Core, equals1) {
    CharListAdapter a;
    a.append("hello");
    ASSERT_EQ(a, "hello");
}

TEST(ForwardListAdapter_Core, equals1) {
    CharForwardListAdapter a;
    a.append("hello");
    ASSERT_EQ(a, "olleh");
}

TEST(ResizingVectorAdapter_Core, equals2) {
    CharResizingVectorAdapter a;
    a.append("hello");
    CharResizingVectorAdapter b;
    b.append("hello");
    ASSERT_EQ(a, b);
}

TEST(VectorAdapter_Core, equals2) {
    CharVectorAdapter a;
    a.append("hello");
    CharVectorAdapter b;
    b.append("hello");
    ASSERT_EQ(a, b);
}

TEST(ListAdapter_Core, equals2) {
    CharListAdapter a;
    a.append("hello");
    CharListAdapter b;
    b.append("hello");
    ASSERT_EQ(a, b);
}

TEST(ForwardListAdapter_Core, equals2) {
    CharForwardListAdapter a;
    a.append("hello");
    CharForwardListAdapter b;
    b.append("hello");
    ASSERT_EQ(a, b);
}
