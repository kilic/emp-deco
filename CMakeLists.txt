cmake_minimum_required (VERSION 3.0)
project (emp-deco)
set(NAME "emp-deco")

find_path(CMAKE_FOLDER NAMES cmake/emp-tool-config.cmake)
include(${CMAKE_FOLDER}/cmake/common.cmake)

find_package(emp-sh2pc REQUIRED)
include_directories(${EMP-SH2PC_INCLUDE_DIRS})
  
add_library(hmac_lib STATIC emp-deco/utils.cpp emp-deco/tls.cpp emp-deco/hmac.cpp emp-deco/sha256.cpp)
target_link_libraries(hmac_lib ${EMP-OT_LIBRARIES} ${OPENSSL_LIBRARIES})

add_definitions(-DCIRCUIT_PATH=${PROJECT_SOURCE_DIR}/emp-deco/circuits/)

# tests
set(TEST_LIBRARIES 
  hmac_lib
  ${EMP-OT_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)
add_executable(hmac_test "test/hmac.cpp")
target_link_libraries(hmac_test ${TEST_LIBRARIES})
set(TEST_LIBRARIES 
  hmac_lib
  ${EMP-OT_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)
add_executable(sha256_test "test/sha256.cpp")
target_link_libraries(sha256_test ${TEST_LIBRARIES})
