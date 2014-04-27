#include "gmock/gmock.h"
#include "logging/logging.hpp"

int main(int argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);

    logging::setLevel(logging::DEBUG);
    logging::addOutput(new logging::FileOutput("test.log"));

    return RUN_ALL_TESTS();
}

