/**
 * Tests for class List
 * that implement linked list for elements of an arbitrary type.
 * test_suit_names: LinkedListTest, LinkedListIteratorTest
 * test_name: meaning + function
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
#include "linked_list/linked_list.hpp"

#define TEST_CONSTRUCTOR_MESSAGE "RunTestConstructor\n"
#define TEST_DESTRUCTOR_MESSAGE "RunTestDestructor\n"

std::mt19937 mersenne(static_cast<int>(time(0)));

//
// AUXILIARY FUNCTIONS
//

//Test class for checking calling constructors and destructors.
class TestClass {
public:
    int data;

    TestClass(int data = 1)
    :data(data) {std::cout << TEST_CONSTRUCTOR_MESSAGE;}
    ~TestClass() {std::cout << TEST_DESTRUCTOR_MESSAGE;}

    bool operator==(TestClass& testClass) const {return data == testClass.data;}
    bool operator==(const TestClass& testClass) const {return data == testClass.data;}
    bool operator!=(TestClass& testClass) const {return data != testClass.data;}
    bool operator!=(const TestClass& testClass) const {return data != testClass.data;}
};

std::string getTestSomeMessage(const std::string& message, int times) {
    std::string result;
    for (auto i = 0; i < times; i++) result.append(message);
    return result;
}

template<typename T>
bool isEqualComplexLists(List<T> &list1, List<T> &list2) {
    if (list1.size() != list2.size()) return false;
    auto it1 = list1.iterator();
    auto it2 = list2.iterator();
    while (it1.hasNext()) {
        if (it1.get() != it2.get()) return false;
        it2.next();
        it1.next();
    }
    return it1.get() == it2.get();
}

template<typename T>
bool isEqualListAndArray(List<T> &list, int size, T *array) {
    if (list.size() != size) return false;
    auto i = 0;
    auto it = list.iterator();
    while (it.hasNext()) {
        if (it.get() != array[i]) return false;
        it.next();
        i++;
    }
    return it.get() == array[i];
}

//
// TESTS
//


/*
 * Tests on processing dynamic array
 */

// CONSTRUCTORS

// The test checks whether the default constructor without parameter
// for type without destructors: List()
TEST(LinkedListTest, SimpleType_ConstructorWithoutParameter) {
    List<int> list;

    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
    EXPECT_THROW(list.tail(), std::out_of_range);
}

// The test checks whether the default constructor without parameter
// for type with destructors: List()
TEST(LinkedListTest, ComplexType_ConstructorWithoutParameter) {
    {
        ::testing::internal::CaptureStdout();

        List<TestClass> list;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(list.size(), 0);
        EXPECT_THROW(list.head(), std::out_of_range);
        EXPECT_THROW(list.tail(), std::out_of_range);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}


// The test checks working of copy constructor
// for arrays with simple type: List(const List&)
TEST(LinkedListTest, SimpleType_CopyConstructor) {
    List<int> list1;
    auto size = 4;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
    {
        List<int> list2(list1);

        EXPECT_TRUE(isEqualComplexLists(list1, list2));
    }
    EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
}


// The test checks working of copy constructor
// for arrays with simple type: List(const List&)
TEST(LinkedListTest, SimpleType_CopyConstructor_Large) {
    List<int> list1;
    auto size = const_list::chunk_capacity<int> * 3 + 2;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
    {
        List<int> list2(list1);

        EXPECT_TRUE(isEqualComplexLists(list1, list2));
    }
    EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
}

// The test checks working of copy constructor
// for arrays with complex type: List(const List&)
TEST(LinkedListTest, ComplexType_CopyConstructor) {
    auto size = 4;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list1;
        for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
        {
            List<TestClass> list2(list1);

            EXPECT_TRUE(isEqualComplexLists(list1, list2));
            ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                  getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
        EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// The test checks working of copy constructor
// for arrays with complex type: List(const List&)
TEST(LinkedListTest, ComplexType_CopyConstructor_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list1;
        for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
        {
            List<TestClass> list2(list1);

            EXPECT_TRUE(isEqualComplexLists(list1, list2));
            ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                  getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
        EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// OPERATORS

// The test checks whether list of simple type
// assignments: operator=
TEST(LinkedListTest, SimpleType_OperatorAssignment) {
    List<int> list1;
    auto size = 4;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
    {
        List<int> list2;
        list2 = list1;

        EXPECT_TRUE(isEqualComplexLists(list1, list2));
    }
    EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
}

// The test checks whether list of simple type
// assignments: operator=
TEST(LinkedListTest, SimpleType_OperatorAssignment_Large) {
    List<int> list1;
    auto size = const_list::chunk_capacity<int> * 3 + 2;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
    {
        List<int> list2;
        list2 = list1;

        EXPECT_TRUE(isEqualComplexLists(list1, list2));
    }
    EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
}

// The test checks whether list of complex type
// assignments: operator=
TEST(LinkedListTest, ComplexType_OperatorAssignment) {
    auto size = 4;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list1;
        for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
        {
            List<TestClass> list2;
            list2 = list1;

            EXPECT_TRUE(isEqualComplexLists(list1, list2));
            ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                  getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
        EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// The test checks whether list of complex type
// assignments: operator=
TEST(LinkedListTest, ComplexType_OperatorAssignment_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list1;
        for (auto i = 0; i < size; i++) list1.insertTail(tests[i]);
        {
            List<TestClass> list2;
            list2 = list1;

            EXPECT_TRUE(isEqualComplexLists(list1, list2));
            ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                  getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
        EXPECT_TRUE(isEqualListAndArray(list1, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// INSERT HEAD

// The test checks that the new value is inserted
// into the head of the small list of elements with type without destructors
// and the _capacity is correctly increased: insertHead(const T&)
TEST(LinkedListTest, SimpleType_InsertHead) {
    List<int> list;
    auto size = 4, new_size = 0;
    int tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = mersenne();
        check_tests[size - i - 1] = tests[i];
    }
    for (auto i = 0; i < size; i++) {
        list.insertHead(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[0]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
}

// The test checks that the new value is inserted
// into the head of the large list of elements with type without destructors
// and the _capacity is correctly increased: insertHead(const T&)
TEST(LinkedListTest, SimpleType_InsertHead_Large) {
    List<int> list;
    auto size = const_list::chunk_capacity<int> * 3 + 2, new_size = 0;
    int tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = mersenne();
        check_tests[size - i - 1] = tests[i];
    }
    for (auto i = 0; i < size; i++) {
        list.insertHead(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[0]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
}

// The test checks that the new value is inserted
// into the head of the small list of elements with type with destructors
// and the _capacity is correctly increased: insertHead(const T&)
TEST(LinkedListTest, ComplexType_InsertHead) {
    auto size = 4, new_size = 0;
    TestClass tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = TestClass(mersenne());
        check_tests[size - i - 1] = tests[i];
    }
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertHead(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[0]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// The test checks that the new value is inserted
// into the head of the large list of elements with type with destructors
// and the _capacity is correctly increased: insertHead(const T&)
TEST(LinkedListTest, ComplexType_InsertHead_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2, new_size = 0;
    TestClass tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = TestClass(mersenne());
        check_tests[size - i - 1] = tests[i];
    }
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertHead(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[0]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// INSERT TAIL

// The test checks that the new value is inserted
// into the tail of the small list of elements with type without destructors
// and the _capacity is correctly increased: insertTail(const T&)
TEST(LinkedListTest, SimpleType_InsertTail) {
    List<int> list;
    auto size = 4, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
}

// The test checks that the new value is inserted
// into the tail of the large list of elements with type without destructors
// and the _capacity is correctly increased: insertTail(const T&)
TEST(LinkedListTest, SimpleType_InsertTail_Large) {
    List<int> list;
    auto size = const_list::chunk_capacity<int> * 3 + 2, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
}

// The test checks that the new value is inserted
// into the tail of the small list of elements with type with destructors
// and the _capacity is correctly increased: insertTail(const T&)
TEST(LinkedListTest, ComplexType_InsertTail) {
    auto size = 4, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// The test checks that the new value is inserted
// into the tail of the large list of elements with type with destructors
// and the _capacity is correctly increased: insertTail(const T&)
TEST(LinkedListTest, ComplexType_InsertTail_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// REMOVE HEAD

// The test checks whether the element without destructors
// was deleted correctly from the small list: removeHead()
TEST(LinkedListTest, SimpleType_RemoveHead) {
    List<int> list;
    auto size = 4, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
    for (auto i = 1; i < size; i++) {
        list.removeHead();
        new_size--;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[size - 1]);
    }
    list.removeHead();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
    EXPECT_THROW(list.tail(), std::out_of_range);
}

// The test checks whether the element without destructors
// was deleted correctly from the large list: removeHead()
TEST(LinkedListTest, SimpleType_RemoveHead_Large) {
    List<int> list;
    auto size = const_list::chunk_capacity<int> * 3 + 2, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
    for (auto i = 1; i < size; i++) {
        list.removeHead();
        new_size--;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[size - 1]);
    }
    list.removeHead();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
    EXPECT_THROW(list.tail(), std::out_of_range);
}

// The test checks whether the element with destructors
// was deleted correctly from the small list: removeHead()
TEST(LinkedListTest, ComplexType_RemoveHead) {
    auto size = 4, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        for (auto i = 1; i < size; i++) {
            list.removeHead();
            new_size--;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[size - 1]);
        }
        list.removeHead();
        EXPECT_EQ(list.size(), 0);
        EXPECT_THROW(list.head(), std::out_of_range);
        EXPECT_THROW(list.tail(), std::out_of_range);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}


// The test checks whether the element with destructors
// was deleted correctly from the large list: removeHead()
TEST(LinkedListTest, ComplexType_RemoveHead_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        for (auto i = 1; i < size; i++) {
            list.removeHead();
            new_size--;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[size - 1]);
        }
        list.removeHead();
        EXPECT_EQ(list.size(), 0);
        EXPECT_THROW(list.head(), std::out_of_range);
        EXPECT_THROW(list.tail(), std::out_of_range);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// REMOVE TAIL

// The test checks whether the element without destructors
// was deleted correctly from the small list: removeTail()
TEST(LinkedListTest, SimpleType_RemoveTail) {
    List<int> list;
    auto size = 4, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
    for (auto i = size - 2; i >= 0; i--) {
        list.removeTail();
        new_size--;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    list.removeTail();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
    EXPECT_THROW(list.tail(), std::out_of_range);
}

// The test checks whether the element without destructors
// was deleted correctly from the large list: removeTail()
TEST(LinkedListTest, SimpleType_RemoveHead_Tail) {
    List<int> list;
    auto size = const_list::chunk_capacity<int> * 3 + 2, new_size = 0;
    int tests[size];
    for (auto i = 0; i < size; i++) tests[i] = mersenne();
    for (auto i = 0; i < size; i++) {
        list.insertTail(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, tests));
    for (auto i = size - 2; i >= 0; i--) {
        list.removeTail();
        new_size--;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[0]);
        EXPECT_EQ(list.tail(), tests[i]);
    }
    list.removeTail();
    EXPECT_EQ(list.size(), 0);
    EXPECT_THROW(list.head(), std::out_of_range);
    EXPECT_THROW(list.tail(), std::out_of_range);
}

// The test checks whether the element with destructors
// was deleted correctly from the small list: removeTail()
TEST(LinkedListTest, ComplexType_RemoveTail) {
    auto size = 4, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        for (auto i = size - 2; i >= 0; i--) {
            list.removeTail();
            new_size--;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        list.removeTail();
        EXPECT_EQ(list.size(), 0);
        EXPECT_THROW(list.head(), std::out_of_range);
        EXPECT_THROW(list.tail(), std::out_of_range);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}


// The test checks whether the element with destructors
// was deleted correctly from the large list: removeTail()
TEST(LinkedListTest, ComplexType_RemoveTail_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2, new_size = 0;
    TestClass tests[size];
    for (auto i = 0; i < size; i++) tests[i] = TestClass(mersenne());
    {
        List<TestClass> list;
        for (auto i = 0; i < size; i++) {
            list.insertTail(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, tests));
        for (auto i = size - 2; i >= 0; i--) {
            list.removeTail();
            new_size--;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[0]);
            EXPECT_EQ(list.tail(), tests[i]);
        }
        list.removeTail();
        EXPECT_EQ(list.size(), 0);
        EXPECT_THROW(list.head(), std::out_of_range);
        EXPECT_THROW(list.tail(), std::out_of_range);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

/*
 * Tests for iterator`s functions
 */

// The test checks that the new value is inserted
// into the head of the small list of elements with type without destructors
// and the _capacity is correctly increased via iterator
TEST(LinkedListIteratorTest, SimpleType_InsertHead) {
    List<int> list;
    auto it = list.iterator();
    auto size = 4, new_size = 0;
    int tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = mersenne();
        check_tests[size - i - 1] = tests[i];
    }
    for (auto i = 0; i < size; i++) {
        it.insert(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[0]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
}

// The test checks that the new value is inserted
// into the head of the large list of elements with type without destructors
// and the _capacity is correctly increased via iterator
TEST(LinkedListIteratorTest, SimpleType_InsertHead_Large) {
    List<int> list;
    auto it = list.iterator();
    auto size = const_list::chunk_capacity<int> * 3 + 2, new_size = 0;
    int tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = mersenne();
        check_tests[size - i - 1] = tests[i];
    }
    for (auto i = 0; i < size; i++) {
        it.insert(tests[i]);
        new_size++;
        EXPECT_EQ(list.size(), new_size);
        EXPECT_EQ(list.head(), tests[i]);
        EXPECT_EQ(list.tail(), tests[0]);
    }
    EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
}

// The test checks that the new value is inserted
// into the head of the small list of elements with type with destructors
// and the _capacity is correctly increased via iterator
TEST(LinkedListIteratorTest, ComplexType_InsertHead) {
    auto size = 4, new_size = 0;
    TestClass tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = TestClass(mersenne());
        check_tests[size - i - 1] = tests[i];
    }
    {
        List<TestClass> list;
        auto it = list.iterator();
        for (auto i = 0; i < size; i++) {
            it.insert(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[0]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}


// The test checks that the new value is inserted
// into the head of the large list of elements with type with destructors
// and the _capacity is correctly increased via iterator
TEST(LinkedListIteratorTest, ComplexType_InsertHead_Large) {
    auto size = const_list::chunk_capacity<TestClass> * 3 + 2, new_size = 0;
    TestClass tests[size], check_tests[size];
    for (auto i = 0; i < size; i++) {
        tests[i] = TestClass(mersenne());
        check_tests[size - i - 1] = tests[i];
    }
    {
        List<TestClass> list;
        auto it = list.iterator();
        for (auto i = 0; i < size; i++) {
            it.insert(tests[i]);
            new_size++;
            EXPECT_EQ(list.size(), new_size);
            EXPECT_EQ(list.head(), tests[i]);
            EXPECT_EQ(list.tail(), tests[0]);
        }
        EXPECT_TRUE(isEqualListAndArray(list, size, check_tests));
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, size));
}

// The test checks function inserting element via iterator
// for array with elements without destructors
TEST(LinkedListIteratorTest, SimpleType_Insert) {
    List<int> list;
    for (auto i = 1; i < 5; i++) list.insertTail(i * 3);//3 6 9 12
    auto it = list.iterator();
    it.insert(5);//5 3 6 9 12

    EXPECT_EQ(it.get(), 5);
    it.next();
    EXPECT_EQ(it.get(), 3);
    it.next();
    EXPECT_EQ(it.get(), 6);
    it.next();
    EXPECT_EQ(it.get(), 9);
    it.next();
    EXPECT_EQ(it.get(), 12);
}

// The test checks function set element via iterator
// for array with elements without destructors
TEST(LinkedListIteratorTest, SimpleType_Set) {
    List<int> list;
    for (auto i = 1; i < 5; i++) list.insertTail(i * 3);//3 6 9 12
    auto it = list.iterator();
    it.next();
    it.next();
    it.set(7);//3 6 7 12

    it.prev();
    it.prev();
    EXPECT_EQ(it.get(), 3);
    it.next();
    EXPECT_EQ(it.get(), 6);
    it.next();
    EXPECT_EQ(it.get(), 7);
    it.next();
    EXPECT_EQ(it.get(), 12);
}

// The test checks iteratoin for the Iterator for
// array of elements with type with destructor
TEST(LinkedListIteratorTest, ComplexType_Iteract) {
    auto size = 2;
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3);
    {
        List<TestClass> list;
        list.insertHead(test3);
        list.insertHead(test2);
        list.insertHead(test1);

        auto i = 0;
        auto it = list.iterator();
        while (true) {
            i++;
            EXPECT_EQ(it.get().data, i * 3);
            if (!it.hasNext()) break;
            it.next();
        }
        i++;
        while (true) {
            i--;
            EXPECT_EQ(it.get().data, i * 3);
            if (!it.hasPrev()) break;
            it.prev();
        }
        times = list.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function removing iterator
// for array with elements with destructors
TEST(LinkedListIteratorTest, ComplexType_Remove) {
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3);
    {
        List<TestClass> list;
        list.insertTail(test1);
        list.insertTail(test2);
        list.insertTail(test3);
        auto it = list.iterator();
        it.remove();//6 9

        EXPECT_EQ(it.get().data, 6);
        it.next();
        EXPECT_EQ(it.get().data, 9);
        times = list.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function inserting element via iterator
// for array with elements with destructors
TEST(LinkedListIteratorTest, ComplexType_Insert) {
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3), test4(5);
    {
        List<TestClass> list;
        list.insertTail(test1);
        list.insertTail(test2);
        list.insertTail(test3);
        auto it = list.iterator();

        EXPECT_EQ(it.get().data, 3);
        it.next();
        EXPECT_EQ(it.get().data, 6);
        it.next();
        EXPECT_EQ(it.get().data, 9);

        it = list.iterator();
        it.insert(test4);//5 3 6 9

        EXPECT_EQ(it.get().data, 5);
        it.next();
        EXPECT_EQ(it.get().data, 3);
        it.next();
        EXPECT_EQ(it.get().data, 6);
        it.next();
        EXPECT_EQ(it.get().data, 9);
        times = list.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function set element via iterator
// for array with elements with destructors
TEST(LinkedListIteratorTest, ComplexType_Set) {
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3), test4(7);
    {
        List<TestClass> list;
        list.insertHead(test3);
        list.insertHead(test2);
        list.insertHead(test1);
        auto it = list.iterator();
        it.next();
        it.next();
        it.set(test4);//3 6 7

        it.prev();
        it.prev();
        EXPECT_EQ(it.get().data, 3);
        it.next();
        EXPECT_EQ(it.get().data, 6);
        it.next();
        EXPECT_EQ(it.get().data, 7);
        times = list.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}