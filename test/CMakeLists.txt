find_package(Threads REQUIRED)

# adds a macro TEST_BASEDIR that points to the test/ directory
# can be used to load files in tests
add_definitions(-DTEST_BASEDIR="${TESTDIR}/")

add_executable(run-tests runner.cpp
    logging/logging_test.cpp

    preconditions_test.cpp
    )

# Linking libs
target_link_libraries(run-tests gtest gtest_main)
target_link_libraries(run-tests gmock gmock_main)
target_link_libraries(run-tests logging)
target_link_libraries(run-tests ${CMAKE_THREAD_LIBS_INIT})
