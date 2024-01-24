//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/client.h>

namespace simple_kafka::client {

    KafkaClientConsumer::KafkaClientConsumer(std::unique_ptr<config::KafkaConfig> config) {
        m_config = std::move(config);
        std::string errstr;
        m_consumer =  std::unique_ptr<RdKafka::KafkaConsumer>(
                RdKafka::KafkaConsumer::create(m_config->get_kafka_conf(), errstr));
//                (RdKafka::KafkaConsumer::create(m_config->get_kafka_conf(), errstr));
        if (!m_consumer) {
            m_config->logger->send<simple_logger::LogLevel::CRITICAL>("Error creating consumer: " + errstr);
            exit(1);
        }
    }

    void KafkaClientConsumer::subscribe() {
        RdKafka::ErrorCode resp = m_consumer->subscribe(m_config->get_kafka_topics());
        if (resp != RdKafka::ERR_NO_ERROR) {
            m_config->logger->send<simple_logger::LogLevel::ERROR>("Error creating consumer: " + RdKafka::err2str(resp));
        }
    }


}