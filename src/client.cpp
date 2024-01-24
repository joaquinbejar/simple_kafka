//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/client.h>

namespace simple_kafka::client {

    KafkaClientConsumer::KafkaClientConsumer(config::KafkaConfig config) : m_config(std::move(config)) {
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
        m_consumer->unsubscribe();
    }


    void KafkaClientConsumer::m_consume() {
        while (m_run_consume) {
            std::unique_ptr<RdKafka::Message> msg = std::unique_ptr<RdKafka::Message>(m_consumer->consume(1000));
            if (msg->err() == RdKafka::ERR_NO_ERROR) {
                // Mensaje recibido correctamente
                std::cout << "Message received: " << static_cast<const char*>(msg->payload()) << std::endl;
            } else if (msg->err() == RdKafka::ERR__TIMED_OUT) {
                // El consumo ha excedido el tiempo límite
                std::cout << "Consumption timed out." << std::endl;
            } else {
                // Manejar otros errores
                std::cerr << "Error in consume: " << msg->errstr() << std::endl;
            }
        }
        m_consumer->close();
    }

    void KafkaClientConsumer::consume() {
        m_run_consume = true;
        m_consumeThread = std::thread(&KafkaClientConsumer::m_consume, this);
    }

    void KafkaClientConsumer::stop() {
        std::lock_guard<std::mutex> lock(m_Mutex);
        m_run_consume = false;
        if (m_consumeThread.joinable()) {
            m_consumeThread.join();
        }
    }

}