#include "TestManagerTestClass.h"
#include "TextManager.h"

CPPUNIT_TEST_SUITE_REGISTRATION(TestManagerTestClass);

TestManagerTestClass::TestManagerTestClass() {
}

TestManagerTestClass::~TestManagerTestClass() {
}

void TestManagerTestClass::setUp() {
}

void TestManagerTestClass::tearDown() {
}

void TestManagerTestClass::testTextManager() {
    std::string testString = "Hello, world, Hello!";
    TextManager *wc = new TextManager(testString);
    CPPUNIT_ASSERT_EQUAL(wc->getGlobalMap()->getCounterMap()->at("Hello"),2);
    CPPUNIT_ASSERT_EQUAL(wc->getGlobalMap()->getCounterMap()->at("world"),1);
    delete(wc);
}

void TestManagerTestClass::testTextManager2() {
    FILE *file = fopen("res/test1.txt","r");
    TextManager *textManager = new TextManager(file);
    CPPUNIT_ASSERT_EQUAL(textManager->getGlobalMap()->getCounterMap()->at("лесу"),3);
    CPPUNIT_ASSERT_EQUAL(textManager->getGlobalMap()->getCounterMap()->at("Мороз"),1);
    delete(textManager);
}