//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/client.h>

namespace simple_kafka::client {

    KafkaClientException::KafkaClientException(const std::shared_ptr<simple_logger::Logger> &logger,
                                               std::string message) : m_message(std::move(message)) {
        logger->send<simple_logger::LogLevel::CRITICAL>("KafkaClientException: " + message);
    }

    const char *KafkaClientException::what() const noexcept {
        return m_message.c_str();
    }

    KafkaClientConsumer::KafkaClientConsumer(config::KafkaConfig &config, common::MetaProducerConsumer &mpc) : m_config(
            std::move(config)), exConsumer(mpc) {
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
            exConsumer.consume(*msg);
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

    KafkaClientProducer::KafkaClientProducer(config::KafkaConfig &config, common::MetaProducerConsumer &mpc) : m_config(
            std::move(config)), exProducer(mpc) {
        std::string errstr;
        m_producer = std::unique_ptr<RdKafka::Producer>(
                RdKafka::Producer::create(m_config.get_kafka_producer_conf(), errstr));
        if (!m_producer) {
            m_config.logger->send<simple_logger::LogLevel::CRITICAL>("Error creating producer: " + errstr);
            exit(1);
        }
    }

    KafkaClientProducer::~KafkaClientProducer() {
        if (m_run_produce)
            stop();
    }

    void KafkaClientProducer::produce() {
        m_run_produce = true;
        m_produceThread = std::thread(&KafkaClientProducer::m_produce, this);
    }

    void KafkaClientProducer::stop() {
        m_config.logger->send<simple_logger::LogLevel::DEBUG>("Stopping producer");
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_run_produce = false;
        if (m_produceThread.joinable()) {
            m_produceThread.join();
        }
    }

    void KafkaClientProducer::m_produce() {
        m_config.logger->send<simple_logger::LogLevel::DEBUG>("Starting producer");
        std::string msg;
        while (m_run_produce) {
            msg = exProducer.get_msg_to_produce();
            // sleep for 1 second
            std::this_thread::sleep_for(std::chrono::seconds(1));

            if (!msg.empty()) {
                // iter m_kafka_topics
                auto topics = m_config.get_kafka_topics();
                std::for_each(topics.begin(), topics.end(),
                              [&](const std::string &topic) {
                                  try {
                                      RdKafka::ErrorCode resp = m_producer->produce(
                                              topic, // const std::string topic_name
                                              RdKafka::Topic::PARTITION_UA, // int32_t partition
                                              RdKafka::Producer::RK_MSG_COPY, // int msgflags
                                              const_cast<char *>(msg.c_str()), // const void *payload
                                              msg.size(), // size_t len
                                              nullptr, // const void *key
                                              0, // size_t key_len
                                              (int64_t) ::common::dates::get_unix_timestamp(), //int64_t timestamp
                                              nullptr);  // void *msg_opaque
                                      switch (resp) {
                                          case RdKafka::ERR_NO_ERROR: {
                                              std::string info = "Topic: " + topic;
                                              info += " Message: " + msg;
                                              m_config.logger->send<simple_logger::LogLevel::DEBUG>(info);
                                              break;
                                          }
                                          case RdKafka::ERR__QUEUE_FULL:
                                              m_config.logger->send<simple_logger::LogLevel::ERROR>(
                                                      "KafkaClientProducer Queue full: " + msg);
                                              break;
                                          case RdKafka::ERR_MSG_SIZE_TOO_LARGE:
                                              m_config.logger->send<simple_logger::LogLevel::ERROR>(
                                                      "KafkaClientProducer Message too large: " + msg);
                                              break;
                                          default:
                                              m_config.logger->send<simple_logger::LogLevel::ERROR>(
                                                      "KafkaClientProducer Error producing message: " +
                                                      RdKafka::err2str(resp));
                                              break;

                                      }
                                  } catch (std::exception &e) {
                                      throw KafkaClientException(m_config.logger, e.what());
                                  }
                              });

            }
        }
        m_producer->flush(m_config.get_kafka_flush_timeout());
        m_producer->poll(0);
    }

}