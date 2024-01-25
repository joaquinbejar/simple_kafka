//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/client.h>

namespace simple_kafka::client {

    KafkaClientConsumer::KafkaClientConsumer(config::KafkaConfig &config, common::MetaConsumer &ec) : m_config(
            std::move(config)), exConsumer(ec) {
        std::string errstr;
        m_consumer = std::unique_ptr<RdKafka::KafkaConsumer>(
                RdKafka::KafkaConsumer::create(m_config.get_kafka_conf(), errstr));
        if (!m_consumer) {
            m_config.logger->send<simple_logger::LogLevel::CRITICAL>("Error creating consumer: " + errstr);
            exit(1);
        }
    }

    KafkaClientConsumer::~KafkaClientConsumer() {
        if (m_run_consume)
            stop();
    }

    void KafkaClientConsumer::subscribe() {
        RdKafka::ErrorCode resp = m_consumer->subscribe(m_config.get_kafka_topics());
        if (resp != RdKafka::ERR_NO_ERROR) {
            m_config.logger->send<simple_logger::LogLevel::ERROR>("Error creating consumer: " + RdKafka::err2str(resp));
        }
    }

    void KafkaClientConsumer::unsubscribe() {
        m_config.logger->send<simple_logger::LogLevel::DEBUG>("Unsubscribing consumer");
        m_consumer->unsubscribe();
    }


    void KafkaClientConsumer::m_consume() {
        m_config.logger->send<simple_logger::LogLevel::DEBUG>("Starting consumer");
        while (m_run_consume) {
            std::unique_ptr<RdKafka::Message> msg = std::unique_ptr<RdKafka::Message>(
                    m_consumer->consume(m_config.get_kafka_msg_timeout()));
            exConsumer.consume_cb(*msg);
        }
        m_consumer->close();
    }

    void KafkaClientConsumer::consume() {
        m_run_consume = true;
        m_consumeThread = std::thread(&KafkaClientConsumer::m_consume, this);
    }

    void KafkaClientConsumer::stop() {
        m_config.logger->send<simple_logger::LogLevel::DEBUG>("Stopping consumer");
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_run_consume = false;
        if (m_consumeThread.joinable()) {
            m_consumeThread.join();
        }
    }

}