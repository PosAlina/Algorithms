/**
 * Tests for class Array
 * that implement dynamic array for elements of an arbitrary type.
 * test_suit_names: DynamicArrayTest, DynamicArrayIteratorTest
 * test_name: meaning + function
 *
 * For all tests warning:
 * Clang-Tidy:
 * Initialization of 'test_info_' with static storage duration
 * may throw an exception that cannot be caught
 */

#include "gtest/gtest.h"

#include "constants.hpp"
#include "dynamic_array/dynamic_array.hpp"

#define TEST_CONSTRUCTOR_MESSAGE "RunTestConstructor\n"
#define TEST_DESTRUCTOR_MESSAGE "RunTestDestructor\n"

//
// AUXILIARY FUNCTIONS
//

//Test class for checking calling constructors and destructors.
class TestClass {
public:
    int data;
    explicit TestClass(int data = 1)
    :data(data) {std::cout << TEST_CONSTRUCTOR_MESSAGE;}
    ~TestClass() {std::cout << TEST_DESTRUCTOR_MESSAGE;}
};

std::string getTestSomeMessage(const std::string& message, int times) {
    std::string result;
    for (auto i = 0; i < times; i++) result.append(message);
    return result;
}

//
// TESTS
//

/*
 * Tests on processing dynamic array
 */

// The test checks whether the default constructor without parameter
// for type without destructors: Array()
TEST(DynamicArrayTest, SimpleType_ConstructorWithoutParameter) {
    Array<int> array;

    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.capacity(),const_array::array_capacity);
}

// The test checks whether the default constructor without parameter
// for type with destructors: Array()
TEST(DynamicArrayTest, ComplexType_ConstructorWithoutParameter) {
    {
        ::testing::internal::CaptureStdout();

        Array<TestClass> array;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(array.size(), 0);
        EXPECT_EQ(array.capacity(), const_array::array_capacity);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks whether the default constructor with parameter
// for type without destructors: Array(int)
TEST(DynamicArrayTest, SimpleType_ConstructorWithParameter) {
    auto size = 4;
    Array<int> array(size);

    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.capacity(), size);
}

// The test checks whether the default constructor with parameter
// with error (0 or negative _capacity -> default)
// for type without destructors: Array(int)
TEST(DynamicArrayTest,
     SimpleType_ConstructorWithParameter_NotPositiveCapacity) {
    auto size = -4;
    Array<int> array(size);

    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.capacity(), const_array::array_capacity);
}

// The test checks whether the default constructor with parameter
// for type with destructors: Array(int)
TEST(DynamicArrayTest, ComplexType_ConstructorWithParameter) {
    auto size = 4;
    {
        ::testing::internal::CaptureStdout();

        Array<TestClass> array(size);

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(array.size(), 0);
        EXPECT_EQ(array.capacity(), size);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks whether the default constructor with parameter
// with error (0 or negative _capacity -> default)
// for type with destructors: Array(int)
TEST(DynamicArrayTest,
     ComplexType_ConstructorWithParameter_NotPositiveCapacity) {
    auto size = -10;
    {
        Array<TestClass> array(size);

        EXPECT_EQ(array.size(), 0);
        EXPECT_EQ(array.capacity(), const_array::array_capacity);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks working of copy constructor
// for arrays with simple type: Array(const Array&)
TEST(DynamicArrayTest, SimpleType_CopyConstructor) {
    Array<int> array1;
    array1.insert(1);
    array1.insert(2);
    {
        Array<int> array2(array1);

        EXPECT_EQ(array1.size(), array2.size());
        EXPECT_EQ(array1.capacity(), array2.capacity());
        if (array1.size() == array2.size())
            for (auto i = 0; i < array1.size(); i++)
                EXPECT_EQ(array1[i], array2[i]);
    }
    EXPECT_EQ(array1[0], 1);
    EXPECT_EQ(array1[1], 2);
}

// The test checks working of copy constructor
// for arrays with complex type: Array(const Array&)
TEST(DynamicArrayTest, ComplexType_CopyConstructor) {
    TestClass test1(1), test2(2), test3(3);
    int times;
    {
        Array<TestClass> array1;

        array1.insert(test1);
        array1.insert(test2);
        array1.insert(test3);
        {
            Array<TestClass> array2(array1);

            times = array2.size();
            EXPECT_EQ(array1.size(), array2.size());
            EXPECT_EQ(array1.capacity(), array2.capacity());
            if (array1.size() == array2.size())
                for (auto i = 0; i < array1.size(); i++)
                    EXPECT_EQ(array1[i].data, array2[i].data);
            ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                  getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
        EXPECT_EQ(array1[0].data, test1.data);
        EXPECT_EQ(array1[1].data, test2.data);
        EXPECT_EQ(array1[2].data, test3.data);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks whether the i-element of simple type
// was deleted correctly: operator[]
TEST(DynamicArrayTest, SimpleType_OperatorInsert) {
    auto size = 1;
    Array<int> array(size);
    array.insert(2);
    array.insert(3);
    array.insert(4);
    array.insert(7);

    EXPECT_EQ(array[1], 3);
    EXPECT_THROW(array[-1], std::invalid_argument);
    EXPECT_THROW(array[10], std::invalid_argument);
}

// The test checks whether the i-element of simple type
// was deleted correctly: operator[]
TEST(DynamicArrayTest, StringType_OperatorInsert) {
    Array<std::string> array;
    array.insert(std::string("a"));
    array.insert(std::string("b"));
    array.insert(std::string("c"));
    array.insert(std::string("d"));

    EXPECT_EQ(array[1], std::string("b"));
    EXPECT_THROW(array[-1], std::invalid_argument);
    EXPECT_THROW(array[10], std::invalid_argument);
}

// The test checks whether the i-element of complex type
// was deleted correctly: operator[]
TEST(DynamicArrayTest, ComplexType_OperatorInsert) {
    TestClass test1(1), test2(2), test3(3);
    int times;
    {
        Array<TestClass> array1(2);
        array1.insert(test1);
        array1.insert(test2);
        array1.insert(test3);

        times = array1.size();
        EXPECT_EQ(array1[0].data, test1.data);
        EXPECT_EQ(array1[1].data, test2.data);
        EXPECT_EQ(array1[2].data, test3.data);
        EXPECT_EQ(array1.size(), 3);
        EXPECT_EQ(array1.capacity(), 4);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks whether arrays of simple type
// assignments: operator=
TEST(DynamicArrayTest, SimpleType_OperatorAssignment) {
    Array<int> array1;
    array1.insert(1);
    array1.insert(2);
    Array<int> array2;
    array2 = array1;

    EXPECT_EQ(array1.size(), array2.size());
    EXPECT_EQ(array1.capacity(), array2.capacity());
    if (array1.size() == array2.size())
        for (auto i = 0; i < array1.size(); i++)
            EXPECT_EQ(array1[i], array2[i]);
}

// The test checks whether arrays of complex type
// assignments: operator=
TEST(DynamicArrayTest, ComplexType_OperatorAssignment) {
    TestClass test1(1), test2(2);
    int times;
    {
        Array<TestClass> array1;
        array1.insert(test1);
        array1.insert(test2);
        {
            Array<TestClass> array2;
            array2 = array1;
            times = array1.size();
            EXPECT_EQ(array1.size(), array2.size());
            EXPECT_EQ(array1.capacity(), array2.capacity());
            if (array1.size() == array2.size())
                for (auto i = 0; i < array1.size(); i++)
                    EXPECT_EQ(array1[i].data, array2[i].data);
        ::testing::internal::CaptureStdout();
        }
        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message,
                getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
        EXPECT_EQ(array1[0].data, test1.data);
        EXPECT_EQ(array1[1].data, test2.data);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks that the new value is inserted
// into the array of elements with type without destructors
// by the last element and the _capacity is correctly increased: insert(const T&)
TEST(DynamicArrayTest, SimpleType_InsertWithoutIndex) {
    Array<int> array(1);
    array.insert(2);

    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.capacity(), 1);
    EXPECT_EQ(array[0], 2);

    array.insert(3);

    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.capacity(), 2);
    EXPECT_EQ(array[1], 3);

    array.insert(4);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[2], 4);
}

// The test checks that the new value is inserted
// into the array of elements with type with destructors
// by the last element and the _capacity is correctly increased: insert(const T&)
TEST(DynamicArrayTest, ComplexType_InsertWithoutIndex) {
    auto size = 1;
    int times;
    TestClass test1(1), test2(2), test3(3);
    {
        Array<TestClass> array(size);
        array.insert(test1);

        EXPECT_EQ(array.size(), 1);
        EXPECT_EQ(array.capacity(), 1);
        EXPECT_EQ(array[0].data, 1);

        array.insert(test2);

        EXPECT_EQ(array.size(), 2);
        EXPECT_EQ(array.capacity(), 2);
        EXPECT_EQ(array[1].data, 2);

        array.insert(test3);

        EXPECT_EQ(array.size(), 3);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[2].data, 3);
        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks that the new value is inserted
// into the array of elements with type without destructors
// by the i-element and the _capacity is correctly increased: insert(int, const T&)
TEST(DynamicArrayTest, SimpleType_InsertWithIndex) {
    auto size = 1;
    Array<int> array(size);
    array.insert(2);

    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.capacity(), 1);
    EXPECT_EQ(array[0], 2);

    array.insert(0, 3);

    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.capacity(), 2);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 2);

    array.insert(1, 4);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 2);

    array.insert(3, 5);

    EXPECT_EQ(array.size(), 4);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 2);
    EXPECT_EQ(array[3], 5);

    array.insert(6, 7);//Out of range

    EXPECT_EQ(array.size(), 4);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 2);
    EXPECT_EQ(array[3], 5);
}

// The test checks that the new value is inserted
// into the array of elements with type with destructors
// by the i-element and the _capacity is correctly increased: insert(int, const T&)
TEST(DynamicArrayTest, ComplexType_InsertWithndex) {
    auto size = 1;
    int times;
    TestClass test1(1), test2(2), test3(3), test4(4), test5(5);
    {
        Array<TestClass> array(size);
        array.insert(test1);

        EXPECT_EQ(array.size(), 1);
        EXPECT_EQ(array.capacity(), 1);
        EXPECT_EQ(array[0].data, 1);

        array.insert(0, test2);

        EXPECT_EQ(array.size(), 2);
        EXPECT_EQ(array.capacity(), 2);
        EXPECT_EQ(array[0].data, 2);
        EXPECT_EQ(array[1].data, 1);

        array.insert(1, test3);

        EXPECT_EQ(array.size(), 3);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, 2);
        EXPECT_EQ(array[1].data, 3);
        EXPECT_EQ(array[2].data, 1);

        array.insert(3, test4);

        EXPECT_EQ(array.size(), 4);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, 2);
        EXPECT_EQ(array[1].data, 3);
        EXPECT_EQ(array[2].data, 1);
        EXPECT_EQ(array[3].data, 4);

        array.insert(6, test5);//Out of range

        EXPECT_EQ(array.size(), 4);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, 2);
        EXPECT_EQ(array[1].data, 3);
        EXPECT_EQ(array[2].data, 1);
        EXPECT_EQ(array[3].data, 4);
        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks whether the i-element of simple type
// was deleted correctly: remove(int)
TEST(DynamicArrayTest, SimpleType_Remove) {
    auto size = 1;
    Array<int> array(size);
    array.insert(2);
    array.insert(0, 3);
    array.insert(1, 4);
    array.insert(3, 5);

    EXPECT_EQ(array.size(), 4);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 4);
    EXPECT_EQ(array[2], 2);
    EXPECT_EQ(array[3], 5);

    array.remove(1);

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 5);

    array.remove(-2); // Out of range

    EXPECT_EQ(array.size(), 3);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 2);
    EXPECT_EQ(array[2], 5);

    array.remove(2);

    EXPECT_EQ(array.size(), 2);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 3);
    EXPECT_EQ(array[1], 2);

    array.remove(0);

    EXPECT_EQ(array.size(), 1);
    EXPECT_EQ(array.capacity(), 4);
    EXPECT_EQ(array[0], 2);

    array.remove(0);

    EXPECT_EQ(array.size(), 0);
    EXPECT_EQ(array.capacity(), 4);
}

// The test checks whether the i-element of complex type
// was deleted correctly: remove(int)
TEST(DynamicArrayTest, ComplexType_Remove) {
    auto size = 1;
    int times;
    TestClass test1(1), test2(2), test3(3), test4(4);
    {
        Array<TestClass> array(size);
        array.insert(test1);
        array.insert(0, test2);
        array.insert(1, test3);
        array.insert(3, test4);

        EXPECT_EQ(array.size(), 4);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, test2.data);
        EXPECT_EQ(array[1].data, test3.data);
        EXPECT_EQ(array[2].data, test1.data);
        EXPECT_EQ(array[3].data, test4.data);

        array.remove(1);

        EXPECT_EQ(array.size(), 3);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, test2.data);
        EXPECT_EQ(array[1].data, test1.data);
        EXPECT_EQ(array[2].data, test4.data);

        array.remove(-2); // Out of range

        EXPECT_EQ(array.size(), 3);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, test2.data);
        EXPECT_EQ(array[1].data, test1.data);
        EXPECT_EQ(array[2].data, test4.data);

        array.remove(2);

        EXPECT_EQ(array.size(), 2);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, test2.data);
        EXPECT_EQ(array[1].data, test1.data);

        array.remove(0);

        EXPECT_EQ(array.size(), 1);
        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array[0].data, test1.data);

        array.remove(0);

        EXPECT_EQ(array.size(), 0);
        EXPECT_EQ(array.capacity(), 4);
        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

/*
 * Tests for iterator`s functions
 */

// The test checks iteratoin for the Iterator for
// for array with elements without destructors
TEST(DynamicArrayIteratorTest, SimpleType_Iteract) {
    auto size = 1;
    Array<int> array(size);
    for (auto i = 1; i < 5; i++) array.insert(i * 3);//3 6 9 12
    auto i = 0;
    auto it = array.iterator();
    for (; it.hasNext(); it.next()) {
        i++;
        EXPECT_EQ(it.get(), i * 3);
    }
    i++;
    for (it.prev(); it.hasPrev(); it.prev()) {
        i--;
        EXPECT_EQ(it.get(), i * 3);
    }
}

// The test checks function removing iterator
// for array with elements without destructors
TEST(DynamicArrayIteratorTest, SimpleType_Remove) {
    auto size = 1;
    Array<int> array(size);
    for (auto i = 1; i < 5; i++) array.insert(i * 3);//3 6 9 12
    auto it = array.iterator();
    it.remove();//6 9 12
    it.toIndex(0);

    EXPECT_EQ(it.get(), 6);

    it.next();

    EXPECT_EQ(it.get(), 9);

    it.next();

    EXPECT_EQ(it.get(), 12);
}

// The test checks function inserting element via iterator
// for array with elements without destructors
TEST(DynamicArrayIteratorTest, SimpleType_Insert) {
    auto size = 1;
    Array<int> array(size);
    for (auto i = 1; i < 5; i++) array.insert(i * 3);//3 6 9 12
    auto it = array.iterator();
    it.insert(5);//5 3 6 9 12
    it.toIndex(0);

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
TEST(DynamicArrayIteratorTest, SimpleType_Set) {
    auto size = 1;
    Array<int> array(size);
    for (auto i = 1; i < 5; i++) array.insert(i * 3);//3 6 9 12
    auto it = array.iterator();
    it.toIndex(2);
    it.set(7);//3 6 7 12
    it.toIndex(0);

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
TEST(DynamicArrayIteratorTest, ComplexType_Iteract) {
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3);
    {
        Array<TestClass> array(2);

        EXPECT_EQ(array.capacity(), 2);
        EXPECT_EQ(array.size(), 0);

        array.insert(test1);

        EXPECT_EQ(array.capacity(), 2);
        EXPECT_EQ(array.size(), 1);

        array.insert(test2);

        EXPECT_EQ(array.capacity(), 2);
        EXPECT_EQ(array.size(), 2);

        array.insert(test3);

        EXPECT_EQ(array.capacity(), 4);
        EXPECT_EQ(array.size(), 3);

        auto i = 0;
        auto it = array.iterator();
        for (;it.hasNext(); it.next()) {
            i++;
            EXPECT_EQ(it.get().data, i * 3);
        }
        i++;
        for (it.prev(); it.hasPrev(); it.prev()) {
            i--;
            EXPECT_EQ(it.get().data, i * 3);
        }

        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function removing iterator
// for array with elements with destructors
TEST(DynamicArrayIteratorTest, ComplexType_Remove) {
    auto size = 2;
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3);
    {
        Array<TestClass> array(size);
        array.insert(test1);
        array.insert(test2);
        array.insert(test3);
        auto it = array.iterator();
        it.remove();//6 9
        it.toIndex(0);

        EXPECT_EQ(it.get().data, 6);

        it.next();

        EXPECT_EQ(it.get().data, 9);

        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function inserting element via iterator
// for array with elements with destructors
TEST(DynamicArrayIteratorTest, ComplexType_Insert) {
    auto size = 2;
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3), test4(5);
    {
        Array<TestClass> array(size);
        array.insert(test1);
        array.insert(test2);
        array.insert(test3);
        auto it = array.iterator();
        it.toIndex(0);

        EXPECT_EQ(it.get().data, 3);//6

        it.next();

        EXPECT_EQ(it.get().data, 6);

        it.next();

        EXPECT_EQ(it.get().data, 9);

        it = array.iterator();
        it.insert(test4);//5 3 6 9
        it.toIndex(0);

        EXPECT_EQ(it.get().data, 5);

        it.next();

        EXPECT_EQ(it.get().data, 3);

        it.next();

        EXPECT_EQ(it.get().data, 6);

        it.next();

        EXPECT_EQ(it.get().data, 9);
        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}

// The test checks function set element via iterator
// for array with elements with destructors
TEST(DynamicArrayIteratorTest, ComplexType_Set) {
    auto size = 2;
    int times;
    TestClass test1(3), test2(2 * 3), test3(3 * 3), test4(7);
    {
        Array<TestClass> array(size);
        array.insert(test1);
        array.insert(test2);
        array.insert(test3);
        auto it = array.iterator();
        it.toIndex(2);
        it.set(test4);//3 6 7
        it.toIndex(0);

        EXPECT_EQ(it.get().data, 3);

        it.next();

        EXPECT_EQ(it.get().data, 6);

        it.next();

        EXPECT_EQ(it.get().data, 7);

        times = array.size();
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message,
              getTestSomeMessage(TEST_DESTRUCTOR_MESSAGE, times));
}