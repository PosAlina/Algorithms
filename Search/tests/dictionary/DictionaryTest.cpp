/**
 * Tests for class Dictionary
 * that implement dynamic dictionary for elements of an arbitrary type.
 * test_suit_names: DictionaryTest, DynamicDictionaryIteratorTest
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
#include "dictionary/dictionary.hpp"

#define TEST_CONSTRUCTOR_MESSAGE "RunTestConstructor\n"
#define TEST_DESTRUCTOR_MESSAGE "RunTestDestructor\n"
#define TEST_KEY_CONSTRUCTOR_MESSAGE "RunTestKeyConstructor\n"
#define TEST_KEY_DESTRUCTOR_MESSAGE "RunTestKeyDestructor\n"

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
    friend std::ostream& operator<< (std::ostream &out, const TestClass &testClass) {
        out << testClass.data;
        return out;
    }
};

//Test class for checking calling constructors and destructors.
class TestKeyClass {
public:
    int data;

    TestKeyClass(int data = 1)
            :data(data) {std::cout << TEST_KEY_CONSTRUCTOR_MESSAGE;}
    ~TestKeyClass() {std::cout << TEST_KEY_DESTRUCTOR_MESSAGE;}

    bool operator==(TestKeyClass& testClass) const {return data == testClass.data;}
    bool operator==(const TestKeyClass& testClass) const {return data == testClass.data;}
    bool operator<(TestKeyClass& testClass) {return data < testClass.data;}
    bool operator<(const TestKeyClass& testClass) const {return data < testClass.data;}
    friend std::ostream& operator<< (std::ostream &out, const TestKeyClass &testClass) {
        out << testClass.data;
        return out;
    }
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
 * Tests on processing dictionary
 */


// The test checks whether the default constructor without parameter
// for type value without destructors and key without destructors: Dictionary()
TEST(DictionaryTest, SimpleTypeValue_SimpleTypeKey_ConstructorWithoutParameter) {
    Dictionary<int, int> dictionary;

    EXPECT_EQ(dictionary.size(), 0);
}

// The test checks whether the default constructor without parameter
// for type value with destructors and key without destructors: Dictionary()
TEST(DictionaryTest,  ComplexTypeValue_SimpleTypeKey_ConstructorWithoutParameter) {
    {
        ::testing::internal::CaptureStdout();

        Dictionary<int, TestClass> dictionary;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");

        EXPECT_EQ(dictionary.size(), 0);
        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks whether the default constructor without parameter
// for type value without destructors and key with destructors: Dictionary()
TEST(DictionaryTest,  SimpleTypeValue_ComplexTypeKey_ConstructorWithoutParameter) {
    {
        ::testing::internal::CaptureStdout();

        Dictionary<TestKeyClass, int> dictionary;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(dictionary.size(), 0);

        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks whether the default constructor without parameter
// for type value with destructors and key with destructors: Dictionary()
TEST(DictionaryTest,  ComplexTypeValue_ComplexTypeKey_ConstructorWithoutParameter) {
    {
        ::testing::internal::CaptureStdout();

        Dictionary<TestKeyClass, TestClass> dictionary;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(dictionary.size(), 0);

        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

//Put one element

// The test checks putting the one element
// for type value without destructors and key without destructors: Dictionary()
TEST(DictionaryTest, SimpleTypeValue_SimpleTypeKey_PutOne) {
    Dictionary<double, int> dictionary;
    int value = mersenne();
    double key = 5.9;
    dictionary.put(key, value);

    EXPECT_EQ(dictionary.size(), 1);
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_EQ(dictionary[key], value);
    dictionary.remove(key);
    EXPECT_EQ(dictionary.size(), 0);
}

// The test checks putting the one element
// for type value with destructors and key without destructors: Dictionary()
TEST(DictionaryTest,  ComplexTypeValue_SimpleTypeKey_PutOne) {
    TestClass value(mersenne());
    int key = mersenne();
    {
        ::testing::internal::CaptureStdout();
        Dictionary<int, TestClass> dictionary;

        std::string output_message = ::testing::internal::GetCapturedStdout();
        EXPECT_EQ(output_message, "");
        EXPECT_EQ(dictionary.size(), 0);

        dictionary.put(key, value);

        EXPECT_EQ(dictionary.size(), 1);
        EXPECT_TRUE(dictionary.contains(key));
        EXPECT_EQ(dictionary[key], value);

        dictionary.remove(key);
        EXPECT_EQ(dictionary.size(), 0);

        ::testing::internal::CaptureStdout();
    }
    std::string output_message = ::testing::internal::GetCapturedStdout();
    EXPECT_EQ(output_message, "");
}

// The test checks putting the one element
// for type value without destructors and key with destructors: Dictionary()
TEST(DictionaryTest,  SimpleTypeValue_ComplexTypeKey_PutOne) {
    int value = mersenne();
    TestKeyClass key(mersenne());
    Dictionary<TestKeyClass, int> dictionary;
    dictionary.put(key, value);

    EXPECT_EQ(dictionary.size(), 1);
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_EQ(dictionary[key], value);
    dictionary.remove(key);
    EXPECT_EQ(dictionary.size(), 0);
}

// The test checks putting the one element
// for type value with destructors and key with destructors: put()
TEST(DictionaryTest,  ComplexTypeValue_ComplexTypeKey_PutOne) {
    TestClass value(mersenne());
    TestKeyClass key(mersenne());
    Dictionary<TestKeyClass, TestClass> dictionary;
    dictionary.put(key, value);

    EXPECT_EQ(dictionary.size(), 1);
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_EQ(dictionary[key], value);
    dictionary.remove(key);
    EXPECT_EQ(dictionary.size(), 0);
}

/*
 * Tests for iterator`s functions
 */

// The test checks whether the default constructor without parameter
// for type value without destructors and key without destructors: Dictionary()
TEST(DictionaryTest, SimpleTypeValue_SimpleTypeKey_Iteract) {
    {
        auto size = 10;
        Dictionary<int, int> dictionary;
        std::map<int, int> tests;
        for (auto i = 0; i < size; i++)
            tests.insert(std::make_pair(mersenne(), mersenne()));
        for (auto it = tests.begin(); it != tests.end(); it++)
            dictionary.put(it->first, it->second);
        EXPECT_EQ(dictionary.size(), tests.size());
        for (auto it = tests.begin(); it != tests.end(); it++)
            EXPECT_TRUE(dictionary.contains(it->first));

        for (auto it = dictionary.iterator(); it.hasNext(); it.next()) {
            EXPECT_TRUE(tests.contains(it.key()));
            EXPECT_EQ(it.get(), tests[it.key()]);
        }

        auto it = dictionary.iterator();
        int value = mersenne();
        it.set(value);
        it.next();
        it.prev();
        EXPECT_EQ(it.get(), value);
        EXPECT_FALSE(it.hasPrev());
        dictionary.print();
    }
}

// The test checks whether the default constructor without parameter
// for type value with destructors and key without destructors: Dictionary()
TEST(DictionaryTest, ComplexTypeValue_SimpleTypeKey_Iteract) {
    auto size = 10;
    std::map<int, TestClass> tests;
    for (auto i = 0; i < size; i++)
        tests.insert(std::make_pair(mersenne(), TestClass(mersenne())));
    Dictionary<int, TestClass> dictionary;
    for (auto it = tests.begin(); it != tests.end(); it++)
        dictionary.put(it->first, it->second);

    EXPECT_EQ(dictionary.size(), size);
    dictionary.print();

    EXPECT_EQ(dictionary.size(), tests.size());
    for (auto it = tests.begin(); it != tests.end(); it++)
        EXPECT_TRUE(dictionary.contains(it->first));

    for (auto it = dictionary.iterator(); it.hasNext(); it.next()) {
        EXPECT_TRUE(tests.contains(it.key()));
        EXPECT_EQ(it.get(), tests[it.key()]);
    }

    auto it = dictionary.iterator();
    TestClass value(mersenne());
    it.set(value);
    it.next();
    it.prev();
    EXPECT_EQ(it.get(), value);
    EXPECT_FALSE(it.hasPrev());
}


// The test checks whether the default constructor without parameter
// for type value without destructors and key with destructors: Dictionary()
TEST(DictionaryTest,  SimpleTypeValue_ComplexTypeKey_Iteract) {
    auto size = 10;
    std::map<TestKeyClass, int> tests;
    for (auto i = 0; i < size; i++)
        tests.insert(std::make_pair(TestKeyClass(mersenne()), mersenne()));
    Dictionary<TestKeyClass, int> dictionary;
    for (auto it = tests.begin(); it != tests.end(); it++)
        dictionary.put(it->first, it->second);

    EXPECT_EQ(dictionary.size(), size);
    dictionary.print();

    EXPECT_EQ(dictionary.size(), tests.size());
    for (auto it = tests.begin(); it != tests.end(); it++)
        EXPECT_TRUE(dictionary.contains(it->first));

    for (auto it = dictionary.iterator(); it.hasNext(); it.next()) {
        EXPECT_TRUE(tests.contains(it.key()));
        EXPECT_EQ(it.get(), tests[it.key()]);
    }

    auto it = dictionary.iterator();
    int value(mersenne());
    it.set(value);
    it.next();
    it.prev();
    EXPECT_EQ(it.get(), value);
    EXPECT_FALSE(it.hasPrev());
}

// The test checks whether the default constructor without parameter
// for type value with destructors and key witho destructors: Dictionary()
TEST(DictionaryTest,  ComplexTypeValue_ComplexTypeKey_Iteract) {
    auto size = 10;
    std::map<TestKeyClass, TestClass> tests;
    for (auto i = 0; i < size; i++)
        tests.insert(std::make_pair(TestKeyClass(mersenne()), TestClass(mersenne())));
    Dictionary<TestKeyClass, TestClass> dictionary;
    for (auto it = tests.begin(); it != tests.end(); it++)
        dictionary.put(it->first, it->second);

    EXPECT_EQ(dictionary.size(), size);
    dictionary.print();

    EXPECT_EQ(dictionary.size(), tests.size());
    for (auto it = tests.begin(); it != tests.end(); it++)
        EXPECT_TRUE(dictionary.contains(it->first));

    for (auto it = dictionary.iterator(); it.hasNext(); it.next()) {
        EXPECT_TRUE(tests.contains(it.key()));
        EXPECT_EQ(it.get(), tests[it.key()]);
    }

    auto it = dictionary.iterator();
    TestClass value(mersenne());
    it.set(value);
    it.next();
    it.prev();
    EXPECT_EQ(it.get(), value);
    EXPECT_FALSE(it.hasPrev());
}



/*
// Dictionary(const Dictionary<K, V> &dictionary)
TEST(DictionaryTest,  SimpleTypeValue_SimpleTypeKey_CopyConstructor) {
    int value = mersenne();
    int key = mersenne();
    Dictionary<int, int> dictionary;

    dictionary.put(key, value);
    Dictionary<int, int> dictionary2(dictionary);

    EXPECT_EQ(dictionary.size(), dictionary2.size());
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_TRUE(dictionary2.contains(key));
    EXPECT_EQ(dictionary[key], value);
    EXPECT_EQ(dictionary2[key], value);
}

// Dictionary(const Dictionary<K, V> &dictionary)
TEST(DictionaryTest,  ComplexTypeValue_SimpleTypeKey_CopyConstructor) {
    TestClass value(mersenne());
    int key = mersenne();
    Dictionary<int, TestClass> dictionary;

    dictionary.put(key, value);
    Dictionary<int, TestClass> dictionary2(dictionary);

    EXPECT_EQ(dictionary.size(), dictionary2.size());
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_TRUE(dictionary2.contains(key));
    EXPECT_EQ(dictionary[key], value);
    EXPECT_EQ(dictionary2[key], value);
}

// Dictionary(const Dictionary<K, V> &dictionary)
TEST(DictionaryTest,  SimpleTypeValue_ComplexTypeKey_CopyConstructor) {
    int value = mersenne();
    TestKeyClass key(mersenne());
    Dictionary<TestKeyClass, int> dictionary;
    dictionary.put(key, value);
    Dictionary<TestKeyClass, int> dictionary2(dictionary);

    EXPECT_EQ(dictionary.size(), dictionary2.size());
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_TRUE(dictionary2.contains(key));
    EXPECT_EQ(dictionary[key], value);
    EXPECT_EQ(dictionary2[key], value);
}

// Dictionary(const Dictionary<K, V> &dictionary)
TEST(DictionaryTest,  ComplexTypeValue_CopyConstructor) {
    TestClass value(mersenne());
    TestKeyClass key(mersenne());
    Dictionary<TestKeyClass, TestClass> dictionary;
    dictionary.put(key, value);
    Dictionary<TestKeyClass, TestClass> dictionary2(dictionary);

    EXPECT_EQ(dictionary.size(), dictionary2.size());
    EXPECT_TRUE(dictionary.contains(key));
    EXPECT_TRUE(dictionary2.contains(key));
    EXPECT_EQ(dictionary[key], value);
    EXPECT_EQ(dictionary2[key], value);
}*/
