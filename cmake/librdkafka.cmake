include(FetchContent)
FetchContent_Declare(librdkafka
        GIT_REPOSITORY https://github.com/joaquinbejar/librdkafka.git
        GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(librdkafka)

set(KAFKA_INCLUDE ${librdkafka_SOURCE_DIR}/src-cpp CACHE INTERNAL "")
if (CMAKE_DEBUG)
    message(STATUS "simple_mariadb/cmake librdkafka_SOURCE_DIR ${librdkafka_SOURCE_DIR}")
    message(STATUS "simple_mariadb/cmake KAFKA_INCLUDE ${KAFKA_INCLUDE}")
endif ()