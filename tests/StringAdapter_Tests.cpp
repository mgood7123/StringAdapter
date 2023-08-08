#include <gtest/gtest.h>

#include <string_adapter.h>

using namespace StringAdapter;

#define data_is_null(t)                                                        \
    {                                                                          \
        auto data_ = t.data();                                                 \
        ASSERT_EQ(data_.ptr(), nullptr);                                       \
    }
#define data_is_not_null(t)                                                    \
    {                                                                          \
        auto data_ = t.data();                                                 \
        ASSERT_NE(data_.ptr(), nullptr);                                       \
    }
#define data_is_val(t, i_, value_)                                             \
    {                                                                          \
        auto data_ = t.data();                                                 \
        ASSERT_EQ(data_.ptr()[i_], value_);                                    \
    }
#define data_is_not_val(t, i_, value_)                                         \
    {                                                                          \
        auto data_ = t.data();                                                 \
        ASSERT_NE(data_.ptr()[i_], value_);                                    \
    }

TEST(ResizingVectorAdapter_Core, initialization_data_checking) {
    CharResizingVectorAdapter a;
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(VectorAdapter_Core, initialization_data_checking) {
    CharVectorAdapter a;
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
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
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.vector->capacity(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(VectorAdapter_Core, initialization2_data_checking) {
    CharVectorAdapter a = "hello";
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
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

#define TEST_ITERATOR(A, B, C, D, IT_NAME, IT_BEGIN, IT_END, SLICE_NAME,       \
                      SLICE_FUNC)                                              \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {  \
        puts("TEST_ID: 1");                                                    \
        A a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {          \
        puts("TEST_ID: 2");                                                    \
        B a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {            \
        puts("TEST_ID: 3");                                                    \
        C a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {     \
        puts("TEST_ID: 4");                                                    \
        D a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_BEGIN) {                   \
        puts("TEST_ID: 5");                                                    \
        A a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '1');                                         \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_BEGIN) {                           \
        puts("TEST_ID: 6");                                                    \
        B a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '1');                                         \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_BEGIN) {                             \
        puts("TEST_ID: 7");                                                    \
        C a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '1');                                         \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_BEGIN) {                      \
        puts("TEST_ID: 8");                                                    \
        D a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '5');                                         \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END) {                     \
        puts("TEST_ID: 9");                                                    \
        A a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '5');                                     \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_END) {                             \
        puts("TEST_ID: 10");                                                   \
        B a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '5');                                     \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_END) {                               \
        puts("TEST_ID: 11");                                                   \
        C a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '5');                                     \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END) {                        \
        puts("TEST_ID: 12");                                                   \
        D a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '1');                                     \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END##_2) {                 \
        puts("TEST_ID: 13");                                                   \
        A a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_END##_2) {                         \
        puts("TEST_ID: 14");                                                   \
        B a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_END##_2) {                           \
        puts("TEST_ID: 15");                                                   \
        C a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END##_2) {                    \
        puts("TEST_ID: 16");                                                   \
        D a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {              \
        puts("TEST_ID: 17");                                                   \
        A a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                      \
        puts("TEST_ID: 18");                                                   \
        B a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                        \
        puts("TEST_ID: 19");                                                   \
        C a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                 \
        puts("TEST_ID: 20");                                                   \
        D a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '4');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '2');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core,                                           \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 21");                                                   \
        CharResizingVectorAdapter a = "12345";                                 \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '2');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core,                                                   \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 22");                                                   \
        CharVectorAdapter a = "12345";                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '2');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core,                                                     \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 23");                                                   \
        CharListAdapter a = "12345";                                           \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '2');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 24");                                                   \
        CharForwardListAdapter a = "12345";                                    \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '4');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core,                                           \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 25");                                                   \
        A a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '3');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '4');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core,                                                   \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 26");                                                   \
        B a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '3');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '4');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) { \
        puts("TEST_ID: 27");                                                   \
        C a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '3');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '4');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 28");                                                   \
        D a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '3');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '2');                                    \
        delete sp2;                                                            \
    }

#define TEST_REVERSE_ITERATOR(A, B, C, D, IT_NAME, IT_BEGIN, IT_END,           \
                              SLICE_NAME, SLICE_FUNC)                          \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {  \
        puts("TEST_ID: 29");                                                   \
        A a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {          \
        puts("TEST_ID: 30");                                                   \
        B a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {            \
        puts("TEST_ID: 31");                                                   \
        C a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_empty_##IT_BEGIN##_##IT_END) {     \
        puts("TEST_ID: 32");                                                   \
        D a;                                                                   \
        ASSERT_EQ(a.IT_BEGIN(), a.IT_END());                                   \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_BEGIN) {                   \
        puts("TEST_ID: 33");                                                   \
        A a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '5');                                         \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_BEGIN) {                           \
        puts("TEST_ID: 34");                                                   \
        B a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '5');                                         \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_BEGIN) {                             \
        puts("TEST_ID: 35");                                                   \
        C a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '5');                                         \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_BEGIN) {                      \
        puts("TEST_ID: 36");                                                   \
        D a = "12345";                                                         \
        ASSERT_EQ(*a.IT_BEGIN(), '1');                                         \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END) {                     \
        puts("TEST_ID: 37");                                                   \
        A a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '1');                                     \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_END) {                             \
        puts("TEST_ID: 38");                                                   \
        B a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '1');                                     \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_END) {                               \
        puts("TEST_ID: 39");                                                   \
        C a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '1');                                     \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END) {                        \
        puts("TEST_ID: 40");                                                   \
        D a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1), '5');                                     \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, IT_NAME##_##IT_END##_2) {                 \
        puts("TEST_ID: 41");                                                   \
        A a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, IT_NAME##_##IT_END##_2) {                         \
        puts("TEST_ID: 42");                                                   \
        B a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, IT_NAME##_##IT_END##_2) {                           \
        puts("TEST_ID: 43");                                                   \
        C a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, IT_NAME##_##IT_END##_2) {                    \
        puts("TEST_ID: 44");                                                   \
        D a = "12345";                                                         \
        ASSERT_EQ(*(a.IT_END() - 1 - 2), '3');                                 \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {              \
        puts("TEST_ID: 45");                                                   \
        A a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                      \
        puts("TEST_ID: 46");                                                   \
        B a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                        \
        puts("TEST_ID: 47");                                                   \
        C a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '2');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '4');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core, SLICE_NAME##_##SLICE_FUNC) {                 \
        puts("TEST_ID: 48");                                                   \
        D a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(s[0], '4');                                                  \
        ASSERT_EQ(s[1], '3');                                                  \
        ASSERT_EQ(s[2], '2');                                                  \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core,                                           \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 49");                                                   \
        CharResizingVectorAdapter a = "12345";                                 \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '4');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core,                                                   \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 50");                                                   \
        CharVectorAdapter a = "12345";                                         \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '4');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core,                                                     \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 51");                                                   \
        CharListAdapter a = "12345";                                           \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '4');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_BEGIN) {                 \
        puts("TEST_ID: 52");                                                   \
        CharForwardListAdapter a = "12345";                                    \
        auto sp = a.SLICE_FUNC(1, 3);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*s.IT_BEGIN(), '2');                                         \
        delete sp;                                                             \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core,                                           \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 53");                                                   \
        A a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '2');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '2');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core,                                                   \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 54");                                                   \
        B a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '2');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '2');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) { \
        puts("TEST_ID: 55");                                                   \
        C a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '2');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '2');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         SLICE_NAME##_##SLICE_FUNC##_##IT_NAME##_##IT_END) {                   \
        puts("TEST_ID: 56");                                                   \
        D a = "12345";                                                         \
        auto sp = a.SLICE_FUNC(1, 2);                                          \
        auto & s = *sp;                                                        \
        ASSERT_EQ(*(s.IT_END() - 1), '4');                                     \
        delete sp;                                                             \
        auto sp2 = a.SLICE_FUNC(1, 3);                                         \
        auto & s2 = *sp2;                                                      \
        ASSERT_EQ(*(s2.IT_END() - 1), '4');                                    \
        delete sp2;                                                            \
    }                                                                          \
                                                                               \
    TEST(ResizingVectorAdapter_Core,                                           \
         PRINT__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {                     \
        puts("TEST_ID: 57");                                                   \
        A a = "12345";                                                         \
        std::cout << "a = " << a << "\n";                                      \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, PRINT__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) { \
        puts("TEST_ID: 58");                                                   \
        B b = "12345";                                                         \
        std::cout << "b = " << b << "\n";                                      \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, PRINT__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {   \
        puts("TEST_ID: 59");                                                   \
        C c = "12345";                                                         \
        std::cout << "c = " << c << "\n";                                      \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         PRINT__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {                     \
        puts("TEST_ID: 60");                                                   \
        D d = "12345";                                                         \
        std::cout << "d = " << d << "\n";                                      \
    }                                                                          \
    TEST(ResizingVectorAdapter_Core,                                           \
         HASH__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {                      \
        puts("TEST_ID: 61");                                                   \
        A a = "12345";                                                         \
        std::cout << "hash a = " << std::hash<A>()(a) << "\n";                 \
    }                                                                          \
                                                                               \
    TEST(VectorAdapter_Core, HASH__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {  \
        puts("TEST_ID: 62");                                                   \
        B b = "12345";                                                         \
        std::cout << "hash b = " << std::hash<B>()(b) << "\n";                 \
    }                                                                          \
                                                                               \
    TEST(ListAdapter_Core, HASH__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {    \
        puts("TEST_ID: 63");                                                   \
        C c = "12345";                                                         \
        std::cout << "hash c = " << std::hash<C>()(c) << "\n";                 \
    }                                                                          \
                                                                               \
    TEST(ForwardListAdapter_Core,                                              \
         HASH__##IT_NAME##_empty_##IT_BEGIN##_##IT_END) {                      \
        puts("TEST_ID: 64");                                                   \
        D d = "12345";                                                         \
        std::cout << "hash d = " << std::hash<D>()(d) << "\n";                 \
    }

TEST_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter, CharListAdapter,
              CharForwardListAdapter, iterator, begin, end, slice, slice)
TEST_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter, CharListAdapter,
              CharForwardListAdapter, const_iterator, cbegin, cend, cslice,
              cslice)
TEST_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter,
              const CharListAdapter, const CharForwardListAdapter,
              const_iterator, begin, end, const_slice, slice)
TEST_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter,
              const CharListAdapter, const CharForwardListAdapter,
              const_iterator_2, cbegin, cend, const_cslice, cslice)

TEST_REVERSE_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter,
                      CharListAdapter, CharForwardListAdapter, reverse_iterator,
                      rbegin, rend, reverse_slice, slice)
TEST_REVERSE_ITERATOR(CharResizingVectorAdapter, CharVectorAdapter,
                      CharListAdapter, CharForwardListAdapter,
                      const_reverse_iterator, crbegin, crend, creverse_slice,
                      cslice)
TEST_REVERSE_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter,
                      const CharListAdapter, const CharForwardListAdapter,
                      const_reverse_iterator, rbegin, rend, const_reverse_slice,
                      slice)
TEST_REVERSE_ITERATOR(const CharResizingVectorAdapter, const CharVectorAdapter,
                      const CharListAdapter, const CharForwardListAdapter,
                      const_reverse_iterator_2, crbegin, crend,
                      const_reverse_slice, cslice)

TEST(ResizingVectorAdapter_Core, append1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.vector->capacity(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 5, '\0');
}

TEST(VectorAdapter_Core, append1) {
    CharVectorAdapter a;
    a.append("hello");
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
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
    ASSERT_EQ(a.vector->size(), 3);
    ASSERT_EQ(a.vector->capacity(), 3);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a.vector.get()[0][1], 'o');
    ASSERT_EQ(a.vector.get()[0][2], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, '\0');
    a.erase(0, -1);
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.vector->capacity(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(VectorAdapter_Core, erase1) {
    CharVectorAdapter a;
    a.append("hello");
    a.erase(1, 3);
    ASSERT_EQ(a.vector->size(), 3);
    ASSERT_EQ(a.length(), 2);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a.vector.get()[0][1], 'o');
    ASSERT_EQ(a.vector.get()[0][2], '\0');
    ASSERT_EQ(a[0], 'h');
    ASSERT_EQ(a[1], 'o');
    ASSERT_EQ(a[2], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, 'h');
    data_is_val(a, 1, 'o');
    data_is_val(a, 2, '\0');
    a.erase(0, -1);
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.vector->capacity(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
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
    a.erase(0, -1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
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
    a.erase(0, -1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(ResizingVectorAdapter_Core, replace1) {
    CharResizingVectorAdapter a;
    a.append("hello");
    a.replace(1, 3, "APPLE");
    ASSERT_EQ(a.vector->size(), 8);
    ASSERT_EQ(a.vector->capacity(), 8);
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a.vector.get()[0][1], 'A');
    ASSERT_EQ(a.vector.get()[0][5], 'E');
    ASSERT_EQ(a.vector.get()[0][6], 'o');
    ASSERT_EQ(a.vector.get()[0][7], '\0');
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
    ASSERT_EQ(a.vector->size(), 8);
    ASSERT_EQ(a.length(), 7);
    ASSERT_EQ(a.vector.get()[0][0], 'h');
    ASSERT_EQ(a.vector.get()[0][1], 'A');
    ASSERT_EQ(a.vector.get()[0][5], 'E');
    ASSERT_EQ(a.vector.get()[0][6], 'o');
    ASSERT_EQ(a.vector.get()[0][7], '\0');
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

TEST(resize, ResizingVectorAdapter) {
    std::string str;
    ASSERT_EQ(str.size(), 0);
    ASSERT_EQ(str.length(), 0);
    CharResizingVectorAdapter a;
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.size(), 0);
    str.resize(5);
    ASSERT_EQ(str.size(), 5);
    ASSERT_EQ(str.length(), 5);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[5], '\0');
    a.resize(5);
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.vector->capacity(), 6);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, '\0');

    str.resize(3);
    ASSERT_EQ(str.size(), 3);
    ASSERT_EQ(str.length(), 3);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[3], '\0');
    a.resize(3);
    ASSERT_EQ(a.vector->size(), 4);
    ASSERT_EQ(a.vector->capacity(), 4);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.length(), 3);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a.vector.get()[0][3], '\0');
    ASSERT_EQ(a[3], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 3, '\0');

    str.resize(0);
    ASSERT_EQ(str.size(), 0);
    ASSERT_EQ(str.length(), 0);
    ASSERT_EQ(str[0], '\0');
    a.resize(0);
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.vector->capacity(), 1);
    ASSERT_EQ(a.size(), 0);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
}

TEST(resize, VectorAdapter) {
    std::string str;
    ASSERT_EQ(str.size(), 0);
    ASSERT_EQ(str.length(), 0);
    CharVectorAdapter a;
    ASSERT_EQ(a.vector->size(), 1);
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.size(), 0);
    str.resize(5);
    ASSERT_EQ(str.size(), 5);
    ASSERT_EQ(str.length(), 5);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[5], '\0');
    a.resize(5);
    ASSERT_EQ(a.vector->size(), 6);
    ASSERT_EQ(a.vector->capacity(), 6);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a.vector.get()[0][5], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, '\0');

    str.resize(3);
    ASSERT_EQ(str.size(), 3);
    ASSERT_EQ(str.length(), 3);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[3], '\0');
    a.resize(3);
    ASSERT_EQ(a.vector->size(), 4);
    ASSERT_EQ(a.vector->capacity(), 4);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.length(), 3);
    ASSERT_EQ(a.vector.get()[0][0], '\0');
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a.vector.get()[0][3], '\0');
    ASSERT_EQ(a[3], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 3, '\0');
}

TEST(resize, ListAdapter) {
    std::string str;
    ASSERT_EQ(str.size(), 0);
    ASSERT_EQ(str.length(), 0);
    CharListAdapter a;
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.size(), 0);
    str.resize(5);
    ASSERT_EQ(str.size(), 5);
    ASSERT_EQ(str.length(), 5);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[5], '\0');
    a.resize(5);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, '\0');

    str.resize(3);
    ASSERT_EQ(str.size(), 3);
    ASSERT_EQ(str.length(), 3);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[3], '\0');
    a.resize(3);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.length(), 3);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[3], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 3, '\0');
}

TEST(resize, ForwardListAdapter) {
    std::string str;
    ASSERT_EQ(str.size(), 0);
    ASSERT_EQ(str.length(), 0);
    CharForwardListAdapter a;
    ASSERT_EQ(a.length(), 0);
    ASSERT_EQ(a.size(), 0);
    str.resize(5);
    ASSERT_EQ(str.size(), 5);
    ASSERT_EQ(str.length(), 5);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[5], '\0');
    a.resize(5);
    ASSERT_EQ(a.size(), 5);
    ASSERT_EQ(a.length(), 5);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[5], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 5, '\0');

    str.resize(3);
    ASSERT_EQ(str.size(), 3);
    ASSERT_EQ(str.length(), 3);
    ASSERT_EQ(str[0], '\0');
    ASSERT_EQ(str[3], '\0');
    a.resize(3);
    ASSERT_EQ(a.size(), 3);
    ASSERT_EQ(a.length(), 3);
    ASSERT_EQ(a[0], '\0');
    ASSERT_EQ(a[3], '\0');
    data_is_not_null(a);
    data_is_val(a, 0, '\0');
    data_is_val(a, 3, '\0');
}
