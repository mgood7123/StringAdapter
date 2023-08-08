cmake_minimum_required(VERSION 3.4.1)

#disable example for now, focus on StringAdapter

#testBuilder_add_source(example example.cpp)
#testBuilder_add_library(example gtest_main)
#testBuilder_build(example EXECUTABLES)

testBuilder_add_source(StringAdapter_Tests StringAdapter_Tests.cpp)
testBuilder_add_library(StringAdapter_Tests gtest_main)
testBuilder_add_library(StringAdapter_Tests StringAdapter)
testBuilder_build(StringAdapter_Tests EXECUTABLES)