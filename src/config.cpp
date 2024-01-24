//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/config.h>

namespace simple_kafka::config {
    
    KafkaConfig::KafkaConfig() : simple_config::Config() {
        this->m_set_kafka_conf();
    }

    void KafkaConfig::m_set_kafka_conf() {
        if (this->validate()) {
            m_conf->set("bootstrap.servers", m_kafka_brokers, m_errstr);
            m_conf->set("group.id", m_kafka_group_id, m_errstr);
        } else {
            logger->send<simple_logger::LogLevel::ERROR>("Error setting Kafka configuration");
        }
    }

    /**
     * Validates the Kafka configuration.
     *
     * This method checks if the Polygon API URL and API key are not empty.
     * It logs an error and returns false if either of them is empty.
     *
     * @return true if both the API URL and key are non-empty, false otherwise.
     */
    bool KafkaConfig::validate() {
        if (m_kafka_brokers.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("KAFKA_BROKERS is empty");
            return false;
        }
        if (m_kafka_topic.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("KAFKA_TOPIC is empty");
            return false;
        }
        if (m_kafka_group_id.empty()) {
            logger->send<simple_logger::LogLevel::WARNING>("KAFKA_GROUP_ID is empty");
        }
        return true;
    }

    /**
     * Converts the configuration to JSON format.
     *
     * This method serializes the Kafka configuration settings into a JSON object.
     *
     * @return A JSON object containing the configuration settings.
     */
    json KafkaConfig::to_json() const {
        json j;
        j["kafka_brokers"] = m_kafka_brokers;
        j["kafka_topic"] = m_kafka_topic;
        j["kafka_group_id"] = m_kafka_group_id;

        return j;
    }

    /**
     * Populates the configuration from a JSON object.
     *
     * This method deserializes the Kafka configuration settings from a given JSON object.
     *
     * @param j The JSON object containing the configuration settings.
     */
    void KafkaConfig::from_json(const json &j) {
        try {
            m_kafka_topic = j.at("kafka_topic").get<std::string>();
            m_kafka_brokers = j.at("kafka_brokers").get<std::string>();
            m_kafka_group_id = j.at("kafka_group_id").get<std::string>();
            this->m_set_kafka_conf();
        } catch (json::exception &e) {
            logger->send<simple_logger::LogLevel::ERROR>("Error parsing KafkaConfig: " + std::string(e.what()));
            throw e;
        }
    }

    /**
     * Converts the configuration to a string representation.
     *
     * This method provides a textual representation of the Kafka configuration settings,
     * which includes the Brokers, the Topic and the Group ID.
     *
     * @return A string representing the configuration settings.
     */
    std::string KafkaConfig::to_string() const {
        return (std::string) R"("KafkaConfig":)" + this->to_json().dump();
    }

    std::string KafkaConfig::get_kafka_brokers() const {
        return m_kafka_brokers;
    }

    std::string KafkaConfig::get_kafka_topic() const {
        return m_kafka_topic;
    }

    std::string KafkaConfig::get_kafka_group_id() const {
        return m_kafka_group_id;
    }

    RdKafka::Conf *KafkaConfig::get_kafka_conf()  {
        return m_conf.get();
    }

}