cmake_minimum_required(VERSION 3.5)

set(EXTRA_COMPONENT_DIRS "../components")

set(TEST_COMPONENTS "testable" CACHE STRING "Components to test")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(wifi_subsys_tests)
