cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(simple_kafka)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake;${CMAKE_MODULE_PATH})

include(cmake/simple_logger.cmake)
include(cmake/librdkafka.cmake)


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
endif()

set(SIMPLE_KAFKA_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SIMPLE_KAFKA_SOURCE_FILES
        include/simple_kafka/common.h
        include/simple_kafka/client.h
        include/simple_kafka/config.h
        include/simple_kafka/exceptions.h
        src/common.cpp
        src/config.cpp
        src/client.cpp
        src/exceptions.cpp
)

add_library(simple_kafka STATIC ${SIMPLE_KAFKA_SOURCE_FILES})
set(SIMPLE_KAFKA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if (CMAKE_DEBUG)
    message(STATUS "simple_kafka SIMPLE_COLOR_INCLUDE: ${SIMPLE_COLOR_INCLUDE}")
    message(STATUS "simple_kafka SIMPLE_CONFIG_INCLUDE: ${SIMPLE_CONFIG_INCLUDE}")
    message(STATUS "simple_kafka SIMPLE_LOGGER_INCLUDE: ${SIMPLE_LOGGER_INCLUDE}")
    message(STATUS "simple_kafka NLOHMANN_JSON_INCLUDE: ${NLOHMANN_JSON_INCLUDE}")
    message(STATUS "simple_kafka COMMON_INCLUDE: ${COMMON_INCLUDE}")
endif ()

target_include_directories(simple_kafka PUBLIC
        ${SIMPLE_KAFKA_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${KAFKA_INCLUDE}
)

target_link_libraries(simple_kafka
        PUBLIC
        pthread
        simple_config
        simple_logger
        common
)

set(SIMPLE_KAFKA_TESTS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
option(NO_KAFKA_TESTS "simple_kafka Disable simple_kafka tests" OFF)
if (NOT NO_KAFKA_TESTS)
    FetchContent_Declare(Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.4.0
    )
    FETCHCONTENT_GETPROPERTIES(Catch2)
    FETCHCONTENT_MAKEAVAILABLE(Catch2)
    set(CURL_DISABLE_TESTS ON)
    add_subdirectory(tests)
endif ()
