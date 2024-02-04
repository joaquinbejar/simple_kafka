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
    concept Consumable = requires(T t, std::string &msg) {
        { t.from_string(msg) } -> std::same_as<void>;
    };

    template<typename T> requires Consumable<T>
    class KafkaClientConsumer {

    public:
        T caster;

        explicit KafkaClientConsumer(config::KafkaConfig &config) : m_config(std::move(config)) {
            std::string errstr;
            m_consumer = std::unique_ptr<RdKafka::KafkaConsumer>(
                    RdKafka::KafkaConsumer::create(m_config.get_kafka_conf(), errstr));
            if (!m_consumer) {
                m_config.logger->send<simple_logger::LogLevel::CRITICAL>("Error creating consumer: " + errstr);
                exit(1);
            }
        }

        ~KafkaClientConsumer() {
            if (m_run_consume)
                stop();
        }

        void subscribe() {
            RdKafka::ErrorCode resp = m_consumer->subscribe(m_config.get_kafka_topics());
            if (resp != RdKafka::ERR_NO_ERROR) {
                m_config.logger->send<simple_logger::LogLevel::ERROR>(
                        "Error creating consumer: " + RdKafka::err2str(resp));
            }
        }

        void unsubscribe() {
            m_config.logger->send<simple_logger::LogLevel::DEBUG>("Unsubscribing consumer");
            m_consumer->unsubscribe();
        }

        void consume() {
            m_run_consume = true;
            m_consumeThread = std::thread(&KafkaClientConsumer::m_consume, this);
        }

        void stop() {
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

        std::atomic<bool> m_run_consume = false;
        std::thread m_consumeThread;
        std::mutex m_Mutex;

        std::string msg_to_string(std::unique_ptr<RdKafka::Message> &msg) {
            std::string msg_str;
            switch (msg->err()) {
                case RdKafka::ERR__TIMED_OUT: {
                    if (m_config.get_kafka_warning_partition_eof())
                        m_config.logger->send<simple_logger::LogLevel::WARNING>(
                                "KafkaClientConsumer Consumption timed out.");
                    break;
                }
                case RdKafka::ERR_NO_ERROR: {
                    msg_str = static_cast<const char *>(msg->payload());
                    m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                            "KafkaClientConsumer Message received: " + msg_str);
                    break;
                }
                case RdKafka::ERR__PARTITION_EOF:
                    m_config.logger->send<simple_logger::LogLevel::DEBUG>(
                            "KafkaClientConsumer End of partition reached.");
                    break;
                case RdKafka::ERR__UNKNOWN_TOPIC:
                case RdKafka::ERR__UNKNOWN_PARTITION:
                    m_config.logger->send<simple_logger::LogLevel::ERROR>(
                            "KafkaClientConsumer Topic or partition not found.");
                    break;
                default:
                    m_config.logger->send<simple_logger::LogLevel::ERROR>(
                            "KafkaClientConsumer Error in consume: " + msg->errstr());
                    break;
            }
            return msg_str;
        }

        void m_consume() {
            m_config.logger->send<simple_logger::LogLevel::DEBUG>("Starting consumer");
            while (m_run_consume) {
                try {
                    auto msg = std::unique_ptr<RdKafka::Message>(m_consumer->consume(m_config.get_kafka_msg_timeout()));
                    std::string msg_str = msg_to_string(msg);
                    caster.from_string(msg_str);
                } catch (std::exception &e) {
                    m_config.logger->send<simple_logger::LogLevel::ERROR>(
                            "KafkaClientConsumer Error in m_consume: " + std::string(e.what()));
                }
            }
            m_consumer->close();
        }

    };

//    class KafkaClientProducer {
//
//    public:
//        KafkaClientProducer(config::KafkaConfig &config, common::MetaProducerConsumer &mpc);
//
//        ~KafkaClientProducer();
//
//        void produce();
//
//        void stop();
//
//    private:
//        config::KafkaConfig m_config;
//        std::shared_ptr<RdKafka::Producer> m_producer;
//        common::MetaProducerConsumer exProducer;
//        std::atomic<bool> m_run_produce = false;
//        std::thread m_produceThread;
//        std::mutex m_Mutex;
//
//        void m_produce();
//
//    };

}
#endif //SIMPLE_KAFKA_CLIENT_H
