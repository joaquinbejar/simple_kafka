if (CMAKE_DEBUG)
    message(STATUS "simple_kafka/tests SIMPLE_COLOR_INCLUDE: ${SIMPLE_COLOR_INCLUDE}")
    message(STATUS "simple_kafka/tests SIMPLE_CONFIG_INCLUDE: ${SIMPLE_CONFIG_INCLUDE}")
    message(STATUS "simple_kafka/tests SIMPLE_LOGGER_INCLUDE: ${SIMPLE_LOGGER_INCLUDE}")
    message(STATUS "simple_kafka/tests NLOHMANN_JSON_INCLUDE: ${NLOHMANN_JSON_INCLUDE}")
    message(STATUS "simple_kafka/tests COMMON_INCLUDE: ${COMMON_INCLUDE}")
endif ()

add_executable(test_simple_kafka_common test_common.cpp)
target_include_directories(test_simple_kafka_common
        PRIVATE
        ${SIMPLE_KAFKA_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${KAFKA_INCLUDE}
)
target_link_libraries(test_simple_kafka_common PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_kafka_common PRIVATE
        simple_logger
        simple_config
        common
        simple_kafka
)

add_executable(test_simple_kafka_client test_client.cpp)
target_include_directories(test_simple_kafka_client
        PRIVATE
        ${SIMPLE_KAFKA_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${KAFKA_INCLUDE}
)
target_link_libraries(test_simple_kafka_client PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_kafka_client PRIVATE
        simple_logger
        simple_config
        common
        simple_kafka
        rdkafka++
)


add_executable(test_simple_kafka_config test_config.cpp)
target_include_directories(test_simple_kafka_config
        PRIVATE
        ${SIMPLE_KAFKA_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
)

target_link_libraries(test_simple_kafka_config PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_kafka_config PRIVATE
        simple_logger
        simple_config
        common
        simple_kafka
        rdkafka++
)


add_executable(test_simple_kafka_exceptions test_exceptions.cpp)
target_include_directories(test_simple_kafka_exceptions
        PRIVATE
        ${SIMPLE_KAFKA_INCLUDE}
        ${SIMPLE_COLOR_INCLUDE}
        ${SIMPLE_CONFIG_INCLUDE}
        ${SIMPLE_LOGGER_INCLUDE}
        ${NLOHMANN_JSON_INCLUDE}
        ${COMMON_INCLUDE}
        ${KAFKA_INCLUDE}
)

target_link_libraries(test_simple_kafka_exceptions PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_simple_kafka_exceptions PRIVATE
        simple_logger
        simple_config
        common
        simple_kafka
)


