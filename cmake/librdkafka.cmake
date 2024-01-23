include(FetchContent)
FetchContent_Declare(librdkafka
        GIT_REPOSITORY https://github.com/joaquinbejar/librdkafka.git
        GIT_BRANCH master
)
FetchContent_MakeAvailable(librdkafka)

set(CURL_INCLUDE ${librdkafka_SOURCE_DIR}/include CACHE INTERNAL "")
if (CMAKE_DEBUG)
    message(STATUS "simple_mariadb/cmake librdkafka_SOURCE_DIR ${librdkafka_SOURCE_DIR}")
    message(STATUS "simple_mariadb/cmake CURL_INCLUDE ${CURL_INCLUDE}")
endif ()