if(EXISTS "/Users/aidanwang/repos/cmake-project-template/cmake-build-debug/tests/TestCMakeDemo-b12d07c_tests.cmake")
  include("/Users/aidanwang/repos/cmake-project-template/cmake-build-debug/tests/TestCMakeDemo-b12d07c_tests.cmake")
else()
  add_test(TestCMakeDemo_NOT_BUILT-b12d07c TestCMakeDemo_NOT_BUILT-b12d07c)
endif()
