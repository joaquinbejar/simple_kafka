//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/config.h>
#include <simple_kafka/client.h>
#include <catch2/catch_test_macros.hpp>


using namespace simple_kafka::config;
using namespace simple_kafka::client;
using namespace simple_kafka::common;


TEST_CASE("KafkaClientConsumer", "[KafkaClientConsumer]") {
    KafkaConfig config;
    MetaProducerConsumer exConsumer = MetaProducerConsumer(config.logger);
    KafkaClientConsumer<MetaProducerConsumer> consumer(config, exConsumer);
    consumer.subscribe();
    consumer.consume();
    //sleep for 30 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));
    consumer.unsubscribe();
    consumer.stop();
}


TEST_CASE("KafkaClientProducer", "[KafkaClientProducer]") {
    KafkaConfig config;
    MetaProducerConsumer exProducer = MetaProducerConsumer(config.logger);
    KafkaClientProducer producer(config, exProducer);
    producer.produce();
    //sleep for 30 seconds
    std::this_thread::sleep_for(std::chrono::seconds(30));
    producer.stop();
}
