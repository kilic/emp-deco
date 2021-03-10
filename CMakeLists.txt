cmake_minimum_required (VERSION 3.0)
project (emp-deco)
set(NAME "emp-deco")

find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${CMAKE_FOLDER}/cmake/common.cmake)

find_package(emp-sh2pc REQUIRED)
include_directories(${EMP-SH2PC_INCLUDE_DIRS})
  
add_library(hmac_lib STATIC emp-deco/tls.cpp emp-deco/hmac.cpp emp-deco/sha256.cpp emp-deco/debug.cpp)
target_link_libraries(hmac_lib ${EMP-OT_LIBRARIES} ${OPENSSL_LIBRARIES})

add_definitions(-DCIRCUIT_PATH=${PROJECT_SOURCE_DIR}/emp-deco/circuits/)

# tests
set(TEST_LIBRARIES 
  hmac_lib
  ${EMP-OT_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)
add_executable(hmac_test "test/hmac_test.cpp")
target_link_libraries(hmac_test ${TEST_LIBRARIES})
add_executable(sha256_test "test/sha256_test.cpp")
target_link_libraries(sha256_test ${TEST_LIBRARIES})
add_executable(tls_test "test/tls_test.cpp")
target_link_libraries(tls_test ${TEST_LIBRARIES})
