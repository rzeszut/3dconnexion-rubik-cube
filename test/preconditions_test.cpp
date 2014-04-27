#include "gtest/gtest.h"
#include "util/preconditions.hpp"

#ifndef TEST_BASEDIR
#define TEST_BASEDIR "test/"
#endif

TEST(PreconditionsTest, ShouldThrowExceptionOnFailedPrecondition) {
    ASSERT_THROW(CHECK(1 == 2, "dupajasia"), preconditions::PreconditionFailedException);
}

TEST(PreconditionsTest, ShouldNotThrowExceptionOnPassedPrecondition) {
    ASSERT_NO_THROW(CHECK(1 == 1, "dupajasia"));
}

TEST(PreconditionsTest, ShouldThrowExceptionOnNullObject) {
    ASSERT_THROW(CHECK_NOT_NULL(nullptr, "alamakota"), preconditions::PreconditionFailedException);
}

TEST(PreconditionsTest, ShouldNotThrowExceptionOnNotNullObject) {
    int *p = new int(42);
    ASSERT_NO_THROW(CHECK_NOT_NULL(p, "alamakota"));
    delete p;
}

