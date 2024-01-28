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

    template<typename T>
    concept Consumable = requires(T t, RdKafka::Message &msg) {
        { t.consume(msg) } -> std::same_as<void>;
    };

    template<typename T> requires Consumable<T>
    class KafkaClientConsumer {

    public:
        KafkaClientConsumer(config::KafkaConfig &config, T &mpc) : m_config(std::move(config)), exConsumer(mpc) {
            std::string errstr;
            m_consumer = std::unique_ptr<RdKafka::KafkaConsumer>(
                    RdKafka::KafkaConsumer::create(m_config.get_kafka_conf(), errstr));
            if (!m_consumer) {
                m_config.logger->send<simple_logger::LogLevel::CRITICAL>("Error creating consumer: " + errstr);
                exit(1);
            }
        }

        ~KafkaClientConsumer(){
            if (m_run_consume)
                stop();
        }

        void subscribe(){
            RdKafka::ErrorCode resp = m_consumer->subscribe(m_config.get_kafka_topics());
            if (resp != RdKafka::ERR_NO_ERROR) {
                m_config.logger->send<simple_logger::LogLevel::ERROR>("Error creating consumer: " + RdKafka::err2str(resp));
            }
        }

        void unsubscribe(){
            m_config.logger->send<simple_logger::LogLevel::DEBUG>("Unsubscribing consumer");
            m_consumer->unsubscribe();
        }

        void consume(){
            m_run_consume = true;
            m_consumeThread = std::thread(&KafkaClientConsumer::m_consume, this);
        }

        void stop(){
            m_config.logger->send<simple_logger::LogLevel::DEBUG>("Stopping consumer");
            std::lock_guard<std::mutex> lock(m_Mutex);
            m_run_consume = false;
            if (m_consumeThread.joinable()) {
                m_consumeThread.join();
            }
        }


    private:
        config::KafkaConfig m_config;
        std::shared_ptr<RdKafka::KafkaConsumer> m_consumer;
        common::MetaProducerConsumer exConsumer;
        std::atomic<bool> m_run_consume = false;
        std::thread m_consumeThread;
        std::mutex m_Mutex;

        void m_consume(){
            m_config.logger->send<simple_logger::LogLevel::DEBUG>("Starting consumer");
            while (m_run_consume) {
                std::unique_ptr<RdKafka::Message> msg = std::unique_ptr<RdKafka::Message>(
                        m_consumer->consume(m_config.get_kafka_msg_timeout()));
                exConsumer.consume(*msg);
            }
            m_consumer->close();
        }

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
