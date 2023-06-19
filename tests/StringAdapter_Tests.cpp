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
    CharListAdapter a;
    ASSERT_EQ(a.len, 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ForwardListAdapter_Core, initialization_data_checking) {
    CharForwardListAdapter a;
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

#define TEST_ITERATOR(A, B, C, D, IT_NAME, IT_BEGIN, IT_END, SLICE_NAME, SLICE_FUNC) \
TEST(ResizingVectorAdapter_Core, IT_NAME##_empty##IT_BEGIN##_##IT_END) { \
    A a; \
    ASSERT_EQ(a.IT_BEGIN(), a.IT_END()); \
} \
\
TEST(VectorAdapter_Core, IT_NAME##_empty##IT_BEGIN##_##IT_END) { \
    B a; \
    ASSERT_EQ(a.IT_BEGIN(), a.IT_END()); \
} \
\
TEST(ListAdapter_Core, IT_NAME##_empty##IT_BEGIN##_##IT_END) { \
    C a; \
    ASSERT_EQ(a.IT_BEGIN(), a.IT_END()); \
} \
\
TEST(ForwardListAdapter_Core, IT_NAME##_empty##IT_BEGIN##_##IT_END) { \
    D a; \
    ASSERT_EQ(a.IT_BEGIN(), a.IT_END()); \
} \
\
TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_BEGIN) { \
    A a = "12345"; \
    ASSERT_EQ(*a.IT_BEGIN(), '1'); \
} \
\
TEST(VectorAdapter_Core, IT_NAME##_##IT_BEGIN) { \
    B a = "12345"; \
    ASSERT_EQ(*a.IT_BEGIN(), '1'); \
} \
\
TEST(ListAdapter_Core, IT_NAME##_##IT_BEGIN) { \
    C a = "12345"; \
    ASSERT_EQ(*a.IT_BEGIN(), '1'); \
} \
\
TEST(ForwardListAdapter_Core, IT_NAME##_##IT_BEGIN) { \
    D a = "12345"; \
    ASSERT_EQ(*a.IT_BEGIN(), '5'); \
} \
\
TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END) { \
    A a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1), '5'); \
} \
\
TEST(VectorAdapter_Core, IT_NAME##_##IT_END) { \
    B a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1), '5'); \
} \
\
TEST(ListAdapter_Core, IT_NAME##_##IT_END) { \
    C a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1), '5'); \
} \
\
TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END) { \
    D a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1), '1'); \
} \
\
TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END##_2) { \
    A a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1-2), '3'); \
} \
\
TEST(VectorAdapter_Core, IT_NAME##_##IT_END##_2) { \
    B a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1-2), '3'); \
} \
\
TEST(ListAdapter_Core, IT_NAME##_##IT_END##_2) { \
    C a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1-2), '3'); \
} \
\
TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END##_2) { \
    D a = "12345"; \
    ASSERT_EQ(*(a.IT_END()-1-2), '3'); \
} \
\
TEST(ResizingVectorAdapter_Core, SLICE_NAME) { \
    A a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(s[0], '2'); \
    ASSERT_EQ(s[1], '3'); \
    ASSERT_EQ(s[2], '4'); \
    delete sp; \
} \
\
TEST(VectorAdapter_Core, SLICE_NAME) { \
    B a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(s[0], '2'); \
    ASSERT_EQ(s[1], '3'); \
    ASSERT_EQ(s[2], '4'); \
    delete sp; \
} \
\
TEST(ListAdapter_Core, SLICE_NAME) { \
    C a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(s[0], '2'); \
    ASSERT_EQ(s[1], '3'); \
    ASSERT_EQ(s[2], '4'); \
    delete sp; \
} \
\
TEST(ForwardListAdapter_Core, SLICE_NAME) { \
    D a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(s[0], '4'); \
    ASSERT_EQ(s[1], '3'); \
    ASSERT_EQ(s[2], '2'); \
    delete sp; \
} \
\
TEST(ResizingVectorAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_BEGIN) { \
    CharResizingVectorAdapter a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(*s.IT_BEGIN(), '2'); \
    delete sp; \
} \
\
TEST(VectorAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_BEGIN) { \
    CharVectorAdapter a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(*s.IT_BEGIN(), '2'); \
    delete sp; \
} \
\
TEST(ListAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_BEGIN) { \
    CharListAdapter a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(*s.IT_BEGIN(), '2'); \
    delete sp; \
} \
\
TEST(ForwardListAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_BEGIN) { \
    CharForwardListAdapter a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 3); \
    auto & s = *sp; \
    ASSERT_EQ(*s.IT_BEGIN(), '4'); \
    delete sp; \
} \
\
TEST(ResizingVectorAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_END) { \
    A a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 2); \
    auto & s = *sp; \
    ASSERT_EQ(*(s.IT_END()-1), '3'); \
    delete sp; \
    auto sp2 = a.SLICE_FUNC(1, 3); \
    auto & s2 = *sp2; \
    ASSERT_EQ(*(s2.IT_END()-1), '4'); \
    delete sp2; \
} \
\
TEST(VectorAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_END) { \
    B a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 2); \
    auto & s = *sp; \
    ASSERT_EQ(*(s.IT_END()-1), '3'); \
    delete sp; \
    auto sp2 = a.SLICE_FUNC(1, 3); \
    auto & s2 = *sp2; \
    ASSERT_EQ(*(s2.IT_END()-1), '4'); \
    delete sp2; \
} \
\
TEST(ListAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_END) { \
    C a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 2); \
    auto & s = *sp; \
    ASSERT_EQ(*(s.IT_END()-1), '3'); \
    delete sp; \
    auto sp2 = a.SLICE_FUNC(1, 3); \
    auto & s2 = *sp2; \
    ASSERT_EQ(*(s2.IT_END()-1), '4'); \
    delete sp2; \
} \
\
TEST(ForwardListAdapter_Core, SLICE_NAME##_##IT_NAME##_##IT_END) { \
    D a = "12345"; \
    auto sp = a.SLICE_FUNC(1, 2); \
    auto & s = *sp; \
    ASSERT_EQ(*(s.IT_END()-1), '3'); \
    delete sp; \
    auto sp2 = a.SLICE_FUNC(1, 3); \
    auto & s2 = *sp2; \
    ASSERT_EQ(*(s2.IT_END()-1), '2'); \
    delete sp2; \
} \

TEST_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter, CharListAdapter, CharForwardListAdapter, iterator, begin, end, slice, slice)
TEST_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter, CharListAdapter, CharForwardListAdapter, citerator, cbegin, cend, cslice, cslice)
TEST_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter, const CharListAdapter, const CharForwardListAdapter, const_iterator, begin, end, const_slice, slice)
TEST_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter, const CharListAdapter, const CharForwardListAdapter, const_citerator, cbegin, cend, const_cslice, cslice)

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
