add_test([=[HelloTest.BasicAssertions]=]  /home/jacob/proppa/source/opengl_kit_v2/build/tests/test1 [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[HelloTest.BasicAssertions]=]  PROPERTIES DEF_SOURCE_LINE /home/jacob/proppa/source/opengl_kit_v2/tests/test1.cpp:4 WORKING_DIRECTORY /home/jacob/proppa/source/opengl_kit_v2/build/tests SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  test1_TESTS HelloTest.BasicAssertions)
