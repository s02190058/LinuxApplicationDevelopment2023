cmake_minimum_required(VERSION 3.22)
project(rhasher C)
set(CMAKE_C_STANDARD 17)

# all options for program
option(READLINE_ENABLED "Set readline mode" false)

# libraries configuration
set(LIBS "")
find_library(LIB_RHASH rhash REQUIRED)
if (NOT LIB_RHASH)
    message(FATAL_ERROR "LIB_RHASH was not found")
endif ()
list(APPEND LIBS ${LIB_RHASH})

# compile options configuration
set(COMPILE_OPTIONS "")

if (READLINE_ENABLED)
    find_library(LIB_READLINE readline REQUIRED)
    if (NOT LIB_READLINE)
        message(FATAL_ERROR "LIB_READLINE was not found")
    else ()
        list(APPEND LIBS ${LIB_READLINE})
        list(APPEND COMPILE_OPTIONS "READLINE_ENABLED=1")
    endif ()
endif ()

add_executable(rhasher rhasher.c)
target_link_libraries(rhasher ${LIBS})
target_compile_definitions(rhasher PRIVATE ${COMPILE_OPTIONS})

add_custom_target(distclean  COMMAND git clean -fd)
add_custom_target(test_md5 DEPENDS rhasher test_md5_file test_md5_string)
add_custom_target(test_sha1 DEPENDS rhasher test_sha1_file test_sha1_string)
add_custom_target(test_tth DEPENDS rhasher test_tth_file test_tth_string)
add_custom_target(run_test DEPENDS rhasher test_md5 test_sha1 test_tth)

find_program(MD5SUM md5sum)
if (MD5SUM)
    add_custom_target(test_md5_file DEPENDS rhasher
            COMMAND echo test md5 file
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n data > ${CMAKE_SOURCE_DIR}/test_tmp/data.txt
            COMMAND echo MD5 ${CMAKE_CURRENT_BINARY_DIR}/test_tmp/data.txt | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE [0-9a-f]{32} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND echo -n data | md5sum | grep -oE [0-9a-f]{32} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND cmp ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test md5 file passed
            )
    add_custom_target(test_md5_string DEPENDS rhasher
            COMMAND echo test md5 string
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n 'MD5 \"data' | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE [0-9a-f]{32} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND echo -n data | md5sum | grep -oE [0-9a-f]{32} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND cmp ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test md5 string passed
            )
else()
    message(WARNING "you have not installed sha1sum utility for testing md5 - try sudo apt install md5sum")
    add_custom_target(test_md5_file DEPENDS rhasher)
    add_custom_target(test_md5_string DEPENDS rhasher)
endif()

find_program(SHA1SUM sha1sum)
if (SHA1SUM)
    add_custom_target(test_sha1_file DEPENDS rhasher
            COMMAND echo test sha1 file
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n data > ${CMAKE_SOURCE_DIR}/test_tmp/data.txt
            COMMAND echo -n SHA1 ${CMAKE_SOURCE_DIR}/test_tmp/data.txt | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE [0-9a-f]{40} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND echo -n data | sha1sum | grep -oE [0-9a-f]{40} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND cmp ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test sha1 file passed
            )
    add_custom_target(test_sha1_string DEPENDS rhasher
            COMMAND echo test sha1 string
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n 'SHA1 \"data' | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE [0-9a-f]{40} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND echo -n data | sha1sum | grep -oE [0-9a-f]{40} > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND cmp ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test sha1 string passed
            )
else()
    message(WARNING "you have not installed sha1sum utility for testing sha1 - try sudo apt install sha1sum")
    add_custom_target(test_sha1_file DEPENDS rhasher)
    add_custom_target(test_sha1_string DEPENDS rhasher)
endif()
find_program(RHASH rhash)
if (RHASH)
    add_custom_target(test_tth_file DEPENDS rhasher
            COMMAND echo test tth file
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n data > ${CMAKE_SOURCE_DIR}/test_tmp/input.txt
            COMMAND echo -n 'tth ${CMAKE_SOURCE_DIR}/test_tmp/input.txt' | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE '[A-Za-z0-9+\/=]{32}' > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND rhash --base64 --tth ${CMAKE_SOURCE_DIR}/test_tmp/input.txt | grep -oE '[A-Za-z0-9+\/=]{32}' > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test tth file passed
            )
    add_custom_target(test_tth_string DEPENDS rhasher
            COMMAND echo test tth string
            COMMAND mkdir ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo -n 'tth \"data' | ${CMAKE_CURRENT_BINARY_DIR}/rhasher | grep -oE '[A-Za-z0-9+\/=]{32}' > ${CMAKE_SOURCE_DIR}/test_tmp/tmp1.txt
            COMMAND echo -n data > ${CMAKE_SOURCE_DIR}/test_tmp/input.txt
            COMMAND rhash --base64 --tth ${CMAKE_SOURCE_DIR}/test_tmp/input.txt | grep -oE '[A-Za-z0-9+\/=]{32}' > ${CMAKE_SOURCE_DIR}/test_tmp/tmp2.txt
            COMMAND rm -rf ${CMAKE_SOURCE_DIR}/test_tmp
            COMMAND echo test tth string passed
            )
else()
    message(WARNING "you have not installed rhash utility for testing tts - try sudo apt install rhash")
    add_custom_target(test_tth_file DEPENDS rhasher)
    add_custom_target(test_tth_string DEPENDS rhasher)
endif()
