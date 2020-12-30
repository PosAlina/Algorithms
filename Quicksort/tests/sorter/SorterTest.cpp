/**
 * Tests for class Sorter
 * that sorts an array with elements of an arbitrary type.
 * test_suit_names: SorterTest, PrintTest, ExceptionsTest, TimeMeterTest
 * test_name: meaning + FUNCTION_NAME
 *
 *
 * For all tests warning:
 * Clang-Tidy:
 * Initialization of 'test_info_' with static storage duration
 * may throw an exception that cannot be caught
 */

#include "gtest/gtest.h"
#include <random>
#include <ctime>

#include "constants.hpp"
#include <sorter/sorter.hpp>
#include <sorter/time_meter.hpp>

#define TEST_CONSTRUCTOR_MESSAGE "RunTestConstructor\n"
#define TEST_DESTRUCTOR_MESSAGE "RunTestDestructor\n"

Sorter sorter;
std::mt19937 mersenne(static_cast<int>(time(0)));

class TestClass {
public:
    int data;
    explicit TestClass(int data = 1)
            :data(data) {std::cout << TEST_CONSTRUCTOR_MESSAGE;}
    ~TestClass() {std::cout << TEST_DESTRUCTOR_MESSAGE;}
};

//
// AUXILIARY FUNCTIONS
//

template<typename T, typename Compare>
::testing::AssertionResult isSortedArray(T *first, T *last, Compare comp) {
    for (auto i = first; i < last - 2; i++)
        if (!comp(*i, *(i + 1))) return ::testing::AssertionFailure();
    return ::testing::AssertionSuccess();
}

template<typename T>
::testing::AssertionResult isEqualArrays(
        T *first1, T *last1, T *first2, T *last2) {
    if ((last1 - first1) != (last2 - first2))
        return ::testing::AssertionFailure();
    if (memcmp(first1, first2, (last1 - first1) * sizeof(T)) == 0)
        return ::testing::AssertionSuccess();
    return ::testing::AssertionFailure();
}

std::string getTestSomeMessage(const std::string& message, int times) {
    std::string result;
    for (auto i = 0; i < times; i++) result.append(message);
    return result;
}

//
// TESTS
//

/*
 * Tests on how the algorithm of sortings works
 */


// The test checks the processing of an empty array
TEST(SorterTest, EmptyArray_IS_CORRECT) {
    const auto size = 0;
    int a[] {};

    ::testing::internal::CaptureStdout();
    sorter.sort(a, a + size, LESS(int));
    std::string output_message = ::testing::internal::GetCapturedStdout();

    EXPECT_EQ(output_message, EMPTY_ARRAY_MESSAGE);
}

// The test checks that the array from a single element
// is sorted correctly (not changed).
TEST(SorterTest, SortOneElement_SORT) {
    const auto size = 1;
    const int element = mersenne();
    int a[] {element};

    sorter.sort(a, a + size, LESS(int));

    EXPECT_TRUE(isSortedArray(a, a + size, LESS_OR_EQUAL(int)));
    EXPECT_TRUE(a[0] == element);
}

// The test checks that the array with a small number of arguments
// is sorted correctly by inserts in ascending order.
TEST(SorterTest, SortSmallArrayLess_INSERTION_SORT) {
    const auto size = 4;
    int a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.sort(a, a + size, LESS(int));

    EXPECT_TRUE(isSortedArray(a, a + size, LESS_OR_EQUAL(int)));
}

// The test checks that the array with a small number of arguments
// with constructors is sorted correctly by inserts in ascending order.
TEST(SorterTest, ComplexType_SortSmallArrayLess_INSERTION_SORT) {
    const auto size = 4;
    int times = 4;
    TestClass test1(1), test2(2), test3(3), test4(4);
    {
        ::testing::internal::CaptureStdout();

        TestClass a[size];

        std::string constructor_message =
                ::testing::internal::GetCapturedStdout();
        a[0] = test3;
        a[1] = test4;
        a[2] = test1;
        a[3] = test2;

        sorter.sort(a, a + size, [](const TestClass& a, const TestClass& b) {return a.data < b.data;});

        EXPECT_EQ(a[0].data, test1.data);
        EXPECT_EQ(a[1].data, test2.data);
        EXPECT_EQ(a[2].data, test3.data);
        EXPECT_EQ(a[3].data, test4.data);

        EXPECT_EQ(constructor_message,
                  getTestSomeMessage(TEST_CONSTRUCTOR_MESSAGE, times));
        ::testing::internal::CaptureStdout();
    }
    std::string destructor_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(destructor_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks that the array with a small number of arguments
// is sorted correctly by inserts in descending order.
TEST(SorterTest, SortSmallArrayGreater_INSERTION_SORT) {
    const auto size = 4;
    unsigned a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.sort(a, a + size, GREATER(unsigned));

    EXPECT_TRUE(isSortedArray(a, a + size, GREATER_OR_EQUAL(unsigned)));
}


// The test checks that the array with a small number of arguments
// with constructors is sorted correctly by inserts in descending order.
TEST(SorterTest, ComplexType_SortSmallArrayGreater_INSERTION_SORT) {
    const auto size = 4;
    TestClass test1(1), test2(2), test3(3), test4(4);
    {
        ::testing::internal::CaptureStdout();

        TestClass a[size];

        std::string constructor_message =
                ::testing::internal::GetCapturedStdout();
        a[0] = test3;
        a[1] = test4;
        a[2] = test1;
        a[3] = test2;

        sorter.sort(a, a + size, [](const TestClass& a, const TestClass& b) {return a.data > b.data;});

        EXPECT_EQ(a[0].data, test4.data);
        EXPECT_EQ(a[1].data, test3.data);
        EXPECT_EQ(a[2].data, test2.data);
        EXPECT_EQ(a[3].data, test1.data);

        EXPECT_EQ(constructor_message,
                  getTestSomeMessage(TEST_CONSTRUCTOR_MESSAGE, size));
        ::testing::internal::CaptureStdout();
    }
    std::string destructor_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(destructor_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// The test checks that the large array from multiply elements
// is sorted correctly by quicksort in ascending order.
TEST(SorterTest, SortArrayLess_QUICKSORT) {
    const auto size = const_sort::insert_len + 10;
    int a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.sort(a, a + size, LESS(int));

    EXPECT_TRUE(isSortedArray(a, a + size, LESS_OR_EQUAL(int)));
}

// The test checks that the array with a small number of arguments
// with constructors is sorted correctly by inserts in descending order.
TEST(SorterTest, ComplexType_SortSmallArrayLess_QUICKSORT) {
    const auto size = const_sort::insert_len + 2;
    {
        ::testing::internal::CaptureStdout();
        TestClass a[size];
        std::string constructor_message =
                ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(constructor_message,
                  getTestSomeMessage(TEST_CONSTRUCTOR_MESSAGE, size));
        for (auto &elem : a) elem = TestClass(mersenne());

        sorter.sort(a, a + size, [](const TestClass& a, const TestClass& b) {return a.data < b.data;});

        {
            auto pred_elem = a[0];
            for (auto &elem : a) {
                EXPECT_TRUE(pred_elem.data <= elem.data);
                pred_elem = elem;
            }
        }
        ::testing::internal::CaptureStdout();
    }
    std::string destructor_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(destructor_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// The test checks that the large array from multiply elements
// is sorted correctly by quicksort in descending order.
TEST(SorterTest, SortArrayGreater_QUICKSORT) {
    const auto size = const_sort::insert_len + 2;
    long a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.sort(a, a + size, GREATER(long));

    EXPECT_TRUE(isSortedArray(a, a + size, GREATER_OR_EQUAL(long)));
}

// The test checks that the array from multiply elements
// is sorted correctly by quicksort in ascending order
// with unstandart compare predicat.
TEST(SorterTest, SortArrayWithComp_SORT) {
    const auto size = const_sort::insert_len + 2;
    char a[size];
    for (auto &elem : a) elem = (char)((int)'a' + (mersenne() % 25));
    a[2] = 'a';
    a[3] = 'c';

    sorter.sort(a, a + size,
                [](char a, char b) {return (a > b) || (a == 'a');});
    EXPECT_TRUE(
            isSortedArray(a, a + size,
                          [](char a, char b) {return (a >= b) || (a == 'a');}));
    EXPECT_FALSE(
            isSortedArray(a, a + size, [](char a, char b) {return a >= b;}));
}

TEST(SorterTest, SortArrayGreater_SIMPLEQUICKSORT) {
    const auto size = const_sort::insert_len + 2;
    long a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.print(a, a + size);
    sorter.simple_quicksort(a, a + size - 1, GREATER(long));
    sorter.print(a, a + size);

    EXPECT_TRUE(isSortedArray(a, a + size, GREATER_OR_EQUAL(long)));
}

TEST(SorterTest, SortArrayGreater_SIMPLEINSRTIONSORT) {
    const auto size = const_sort::insert_len + 2;
    long a[size];
    for (auto &elem : a) elem = mersenne();

    sorter.print(a, a + size);
    sorter.simple_insertion_sort(a, a + size - 1, GREATER(long));
    sorter.print(a, a + size);

    EXPECT_TRUE(isSortedArray(a, a + size, GREATER_OR_EQUAL(long)));
}