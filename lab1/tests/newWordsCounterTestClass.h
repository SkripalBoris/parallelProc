#ifndef NEWWORDSCOUNTERTESTCLASS_H
#define NEWWORDSCOUNTERTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newWordsCounterTestClass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newWordsCounterTestClass);

    CPPUNIT_TEST(testWordsCounter);

    CPPUNIT_TEST_SUITE_END();

public:
    newWordsCounterTestClass();
    virtual ~newWordsCounterTestClass();
    void setUp();
    void tearDown();

private:
    void testWordsCounter();

};

#endif /* NEWWORDSCOUNTERTESTCLASS_H */

