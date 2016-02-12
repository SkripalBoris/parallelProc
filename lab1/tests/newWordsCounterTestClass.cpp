#include "newWordsCounterTestClass.h"
#include "WordsCounter.h"

CPPUNIT_TEST_SUITE_REGISTRATION(newWordsCounterTestClass);

newWordsCounterTestClass::newWordsCounterTestClass() {
}

newWordsCounterTestClass::~newWordsCounterTestClass() {
}

void newWordsCounterTestClass::setUp() {
}

void newWordsCounterTestClass::tearDown() {
}

void newWordsCounterTestClass::testWordsCounter() {
    std::string testString = "Hello, world, Hello!";
    WordsCounter *wc = new WordsCounter(testString);
    CPPUNIT_ASSERT_EQUAL(wc->getMap()->at("Hello"),2);
    CPPUNIT_ASSERT_EQUAL(wc->getMap()->at("world"),1);
}