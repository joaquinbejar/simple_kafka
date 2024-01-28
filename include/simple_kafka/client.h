//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_CLIENT_H
#define SIMPLE_KAFKA_CLIENT_H

#include <simple_kafka/config.h>
#include <simple_kafka/common.h>

#include <utility>

namespace simple_kafka::client {

    class KafkaClientException : public std::exception {
    public:
        explicit KafkaClientException(const std::shared_ptr<simple_logger::Logger> &logger, std::string message);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        std::string m_message;
    };

    class KafkaClientConsumer {

    public:
        KafkaClientConsumer(config::KafkaConfig &config, common::MetaProducerConsumer &mpc);

        ~KafkaClientConsumer();

        void subscribe();

        void unsubscribe();

        void consume();

        void stop();


    private:
        config::KafkaConfig m_config;
        std::shared_ptr<RdKafka::KafkaConsumer> m_consumer;
        common::MetaProducerConsumer exConsumer;
        std::atomic<bool> m_run_consume = false;
        std::thread m_consumeThread;
        std::mutex m_Mutex;

        void m_consume();

    };

    class KafkaClientProducer {

    public:
        KafkaClientProducer(config::KafkaConfig &config, common::MetaProducerConsumer &mpc);

        ~KafkaClientProducer();

        void produce();

        void stop();

    private:
        config::KafkaConfig m_config;
        std::shared_ptr<RdKafka::Producer> m_producer;
        common::MetaProducerConsumer exProducer;
        std::atomic<bool> m_run_produce = false;
        std::thread m_produceThread;
        std::mutex m_Mutex;

        void m_produce();

    };

}
#endif //SIMPLE_KAFKA_CLIENT_H
