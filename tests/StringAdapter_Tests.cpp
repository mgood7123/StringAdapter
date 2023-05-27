#include <gtest/gtest.h>


#include <string_adapter.h>

using namespace StringAdapter;

#define ADAPTER_IMPL(ADAPTER) template <typename char_t, typename UNUSED = void> \
struct ADAPTER##_impl : ADAPTER<char_t, ADAPTER##_impl<char_t, UNUSED>> { \
    using BASE = ADAPTER<char_t, ADAPTER##_impl<char_t, UNUSED>>; \
    BASIC_STRING_ADAPTER_USING(BASE); \
\
    ADAPTER##_impl(const std::string & str) { \
        init(str.data(), str.length()); \
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
    void insert(const size_t pos, const size_t len, const char * s) { \
        insert_(ADAPTER##_impl<char_t, UNUSED>(s, strlen(s)), pos, len); \
    } \
\
    void replace(const size_t pos, const size_t len, const char * s) { \
        replace_(ADAPTER##_impl<char_t, UNUSED>(s, strlen(s)), pos, len); \
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
using SA_V = VectorAdapter_impl<char, void>;

ADAPTER_IMPL(ResizingVectorAdapter);
using SA_RV = ResizingVectorAdapter_impl<char, void>;

ADAPTER_IMPL(ListAdapter);
using SA_LL = ListAdapter_impl<char, void>;

ADAPTER_IMPL(ForwardListAdapter);
using SA_RLL = ForwardListAdapter_impl<char, void>;

#define data_is_null(t) { auto * data_ = t.data(); ASSERT_EQ(data_, nullptr); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_not_null(t) { auto * data_ = t.data(); ASSERT_NE(data_, nullptr); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_val(t, i_, value_) { auto * data_ = t.data(); ASSERT_EQ(data_[i_], value_); if (t.data_is_allocated()) { delete[] data_; } }
#define data_is_not_val(t, i_, value_) { auto * data_ = t.data(); ASSERT_NE(data_[i_], value_); if (t.data_is_allocated()) { delete[] data_; } }

TEST(ResizingVectorAdapter_Core, initialization_data_checking) {
    SA_RV a;
    ASSERT_EQ(a.vector.size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector[0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(VectorAdapter_Core, initialization_data_checking) {
    SA_V a;
    ASSERT_EQ(a.vector.size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector[0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ListAdapter_Core, initialization_data_checking) {
    SA_RLL a;
    ASSERT_EQ(a.len, 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ForwardListAdapter_Core, initialization_data_checking) {
    SA_LL a;
    ASSERT_EQ(a.len, 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ResizingVectorAdapter_Core, initialization2_data_checking) {
    SA_RV a = "hello";
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
    SA_V a = "hello";
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
    SA_LL a = "hello";
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(ForwardListAdapter_Core, initialization2_data_checking) {
    SA_RLL a = "hello";
    ASSERT_EQ(a.len, 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[5], 'h');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, 'h');
}

TEST(ResizingVectorAdapter_Core, append1) {
    SA_RV a;
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
    SA_V a;
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
    SA_LL a;
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
    SA_RLL a;
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
    SA_RV a;
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
    SA_V a;
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
    SA_LL a;
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
    SA_RLL a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.len, 2);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a[2], 'e');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 2, 'e');
    data_is_val(a, 1, 'o');
    data_is_val(a, 0, '\0');
}

TEST(ResizingVectorAdapter_Core, replace1) {
    SA_RV a;
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
}

TEST(VectorAdapter_Core, replace1) {
    SA_V a;
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
}

TEST(ListAdapter_Core, replace1) {
    SA_LL a;
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
}

TEST(ForwardListAdapter_Core, replace1) {
    SA_RLL a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a[7], 'e');
    ASSERT_EQ(a[6], 'A');
    ASSERT_EQ(a[5], 'P');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 7, 'e');
    data_is_val(a, 6, 'A');
    data_is_val(a, 5, 'P');
    data_is_val(a, 1, 'o');
    data_is_val(a, 0, '\0');
}
