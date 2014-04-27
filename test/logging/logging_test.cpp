#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "logging/logging.hpp"

using ::testing::_;

class MockOutput : public logging::Output {
public:
    MockOutput() {}
    ~MockOutput() {}

    MOCK_METHOD6(writeLine, void(logging::LogLevel,
                                 std::time_t,
                                 const std::string &,
                                 int,
                                 const std::string &,
                                 const std::string &));
};

class LoggingTest : public testing::Test {
    logging::LogLevel level;

protected:
    MockOutput *mockOutput;
    unsigned id;

    void SetUp() {
        mockOutput = new MockOutput;
        id = logging::addOutput(mockOutput);

        level = logging::getLevel();
    }

    void TearDown() {
        logging::removeOutput(id);
        logging::setLevel(level);
    }
};

TEST_F(LoggingTest, ShouldNotLogWithTooLowLevel) {
    // given
    logging::setLevel(logging::WARNING);
    EXPECT_CALL(*mockOutput, writeLine(_, _, _, _, _, _)).Times(0);

    // when
    LOG(INFO) << "message";
}

TEST_F(LoggingTest, ShouldLogMessage) {
    // given
    logging::setLevel(logging::INFO);
    EXPECT_CALL(*mockOutput, writeLine(logging::INFO, _, _, _, _, "alamakota")).Times(1);

    // when
    LOG(INFO) << "alamakota";
}

TEST_F(LoggingTest, ShouldNotLogWithCheckFailed) {
    // given
    logging::setLevel(logging::INFO);
    EXPECT_CALL(*mockOutput, writeLine(_, _, _, _, _, _)).Times(0);

    // when
    LOG_CHECK(INFO, true && false) << "message";
}

TEST_F(LoggingTest, ShouldLogMessageWithCheckPassed) {
    // given
    logging::setLevel(logging::INFO);
    EXPECT_CALL(*mockOutput, writeLine(logging::INFO, _, _, _, _, "alamakota")).Times(1);

    // when
    LOG_CHECK(INFO, true || false) << "alamakota";
}

