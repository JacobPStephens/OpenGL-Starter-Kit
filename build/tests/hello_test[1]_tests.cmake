add_test([=[HelloTest.BasicAssertions]=]  /home/jacob/proppa/source/lxpaint/build/tests/hello_test [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[HelloTest.BasicAssertions]=]  PROPERTIES DEF_SOURCE_LINE /home/jacob/proppa/source/lxpaint/tests/test1.cpp:4 WORKING_DIRECTORY /home/jacob/proppa/source/lxpaint/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  hello_test_TESTS HelloTest.BasicAssertions)
