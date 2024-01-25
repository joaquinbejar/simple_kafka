//
// Created by Joaquin Bejar Garcia on 20/10/23.
//

#include "simple_kafka/config.h"
#include <catch2/catch_test_macros.hpp>
#include "simple_logger/logger.h"

using namespace simple_kafka::config;

TEST_CASE("KafkaConfig Tests", "[KafkaConfig]") {
//    setenv("KAFKA_TOPICS", "", 1);
    KafkaConfig config;
    // set env variables
//    setenv("KAFKA_TOPICS", "topic1,topic2", 1);

    SECTION("Default Configuration") {
        REQUIRE(config.get_kafka_brokers() == "localhost:9092");
        REQUIRE(config.get_kafka_topics().empty());
    }

    SECTION("from_json method") {
        json j;
        j["kafka_brokers"] = "fromjson:9092";
        j["kafka_topic"] = {"topic1","topic2"};
        j["kafka_group_id"] = "g_id_123";
        j["kafka_msg_timeout"] = 5000;

        config.from_json(j);

        REQUIRE(config.get_kafka_brokers() == "fromjson:9092");
        for (auto &topic : config.get_kafka_topics()) {
            // topic1 in {"topic1", "topic2"}
            REQUIRE((topic == "topic1" || topic == "topic2"));
        }

        REQUIRE(config.get_kafka_group_id() == "g_id_123");
        std::string errstr;
        REQUIRE(RdKafka::Conf::CONF_OK == config.get_kafka_conf()->get("bootstrap.servers", errstr));
        REQUIRE(errstr == "fromjson:9092");
        REQUIRE(RdKafka::Conf::CONF_OK == config.get_kafka_conf()->get("group.id", errstr));
        REQUIRE(errstr == "g_id_123");
        REQUIRE(RdKafka::Conf::CONF_OK == config.get_kafka_conf()->get("enable.auto.commit", errstr));
        REQUIRE(errstr == "true");
        REQUIRE(RdKafka::Conf::CONF_OK == config.get_kafka_conf()->get("auto.commit.interval.ms", errstr));
        REQUIRE(errstr == "5000");
        REQUIRE(RdKafka::Conf::CONF_OK == config.get_kafka_conf()->get("session.timeout.ms", errstr));
        REQUIRE(errstr == "45000");
    }

    SECTION("to_string method") {
        std::string expected_str = R"("KafkaConfig":{"kafka_brokers":"localhost:9092","kafka_group_id":"","kafka_msg_timeout":3000,"kafka_topic":[]})";
        REQUIRE(config.to_string() == expected_str);
    }

    SECTION("from_json method and key as param") {
        json j;
        j["kafka_brokers"] = "fromjson:9092";
        j["kafka_topic"] = {"topic123"};
        j["kafka_group_id"] = "g_id_123";
        j["kafka_msg_timeout"] = 5000;
        config.from_json(j);

        REQUIRE(config.get_kafka_brokers() == "fromjson:9092");
        std::string expected_str = R"("KafkaConfig":{"kafka_brokers":"fromjson:9092","kafka_group_id":"g_id_123","kafka_msg_timeout":5000,"kafka_topic":["topic123"]})";
        REQUIRE(config.to_string() == expected_str);
    }
}
