cmake_minimum_required(VERSION 3.14)
project(gpbotlib)

file(GLOB_RECURSE SOURCES src/**.c include/gpbotlib/**.h)

add_library(gpbotlib ${SOURCES})
target_include_directories(gpbotlib PUBLIC include)

## UNCOMMENT THAT IF YOU ADD EXAMPLES
# add_subdirectory(examples/)
add_subdirectory(sandbox/)
