#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\Core\Core.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace CoreModuleTest
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(TestMethod1)
        {
            char* input[4] = { "END", "OF", "THE", "WORLD" };
            char* result[4] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_word(input, 4, result, 0, 0, false);
            Assert::AreEqual(len, 2);
        }
        TEST_METHOD(TestMethod2)
        {
            char* input[11] = { "Algebra", "Apple", "Zoo", "Elephant", "Under", "Fox", "Dog", "Moon", "Leaf",
                                    "Trick", "Pseudopseudohypoparathyroidism" };
            char* result[11] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_word(input, 11, result, 0, 0, false);
            Assert::AreEqual(len, 4);
        }
        TEST_METHOD(TestMethod3)
        {
            char* input[11] = { "Algebra", "Apple", "Zoo", "Elephant", "Under", "Fox", "Dog", "Moon", "Leaf",
                                    "Trick", "Pseudopseudohypoparathyroidism" };
            char* result[11] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 11, result, 0, 0, false);
            Assert::AreEqual(len, 2);
        }
        TEST_METHOD(TestMethod4)
        {
            char* input[7] = { "Algebra", "Zoo", "Under", "Dog", "Moon", "Leaf",
                                    "Trick" };
            char* result[7] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_word(input, 7, result, 0, 0, false);
            Assert::AreEqual(len, 1);
        }
        TEST_METHOD(TestMethod5)
        {
            char* input[7] = { "Algebra", "Zoo", "Under", "Dog", "Moon", "Leaf",
                                    "Trick" };
            char* result[7] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 7, result, 0, 0, false);
            Assert::AreEqual(len, 1);
        }
        TEST_METHOD(TestMethod6)
        {
            char* input[5] = { "Algebra", "Applea", "Zooa", "Elephant", "Under" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_word(input, 5, result, 0, 0, true);
            Assert::AreEqual(len, 3);
        }
        TEST_METHOD(TestMethod7)
        {
            char* input[5] = { "Algebra", "Applea", "Zooa", "Elephant", "Under" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 5, result, 0, 0, true);
            Assert::AreEqual(len, 3);
        }
        TEST_METHOD(TestMethod8)
        {
            char* input[5] = { "Algebra", "Applea", "Zooa", "Elephant", "Under" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 5, result, 0, 0, false);
            Assert::AreEqual(len, -1);
        }
        TEST_METHOD(TestMethod9)
        {
            char* input[5] = { "Algebra", "Applea", "Zooa", "Elephant", "Under" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 5, result, 0, 0, false);
            Assert::AreEqual(len, -1);
        }
        TEST_METHOD(TestMethod10)
        {
            char* input[5] = { "aa", "aaa", "aaaa", "aaaaa", "a" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_word(input, 5, result, 0, 0, true);
            Assert::AreEqual(len, 5);
        }
        TEST_METHOD(TestMethod11)
        {
            char* input[5] = { "aa", "aaa", "aaaa", "aaaaa", "a" };
            char* result[5] = { 0 };
            /* 调用Core中封装好的函数 */
            int len = gen_chain_char(input, 5, result, 0, 0, true);
            Assert::AreEqual(len, 5);
        }
    };
}
