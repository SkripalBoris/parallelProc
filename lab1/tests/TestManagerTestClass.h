#ifndef TESTMANAGERTESTCLASS_H
#define TESTMANAGERTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class TestManagerTestClass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(TestManagerTestClass);

    CPPUNIT_TEST(testTextManager);
    CPPUNIT_TEST(testTextManager2);

    CPPUNIT_TEST_SUITE_END();

public:
    TestManagerTestClass();
    virtual ~TestManagerTestClass();
    void setUp();
    void tearDown();

private:
    void testTextManager();
    void testTextManager2();

};

#endif /* TESTMANAGERTESTCLASS_H */

