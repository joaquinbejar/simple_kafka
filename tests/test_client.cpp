//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/config.h>
#include <simple_kafka/client.h>
#include <catch2/catch_test_macros.hpp>


using namespace simple_kafka::config;
using namespace simple_kafka::client;


TEST_CASE("KafkaClientConsumer", "[KafkaClientConsumer]") {
    KafkaConfig config;
    KafkaClientConsumer consumer(config);
    consumer.subscribe();
    consumer.consume();
    // sleep(10);
    consumer.unsubscribe();
}