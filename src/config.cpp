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
        if (m_kafka_topics.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("KAFKA_TOPIC is empty");
            return false;
        }
        if (m_kafka_group_id.empty()) {
            logger->send<simple_logger::LogLevel::ERROR>("KAFKA_GROUP_ID is empty");
            return false;
        }
        if (m_kafka_msg_timeout < 0) {
            logger->send<simple_logger::LogLevel::ERROR>("KAFKA_MSG_TIMEOUT is negative");
            return false;
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
        try {
            json j;
            j["kafka_brokers"] = m_kafka_brokers;
            j["kafka_topic"] = m_kafka_topics;
            j["kafka_group_id"] = m_kafka_group_id;
            j["kafka_msg_timeout"] = m_kafka_msg_timeout;
            j["kafka_flush_timeout"] = m_kafka_flush_timeout;
            j["kafka_warning_partition_eof"] = m_kafka_warning_partition_eof;
            j["kafka_consumer_name"] = m_kafka_consumer_name;
            return j;
        } catch (json::exception &e) {
            logger->send<simple_logger::LogLevel::ERROR>("Error parsing KafkaConfig to_json: " + std::string(e.what()));
            throw e;
        }
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
            m_kafka_topics = j.at("kafka_topic").get<std::vector<std::string>>();
            m_kafka_brokers = j.at("kafka_brokers").get<std::string>();
            m_kafka_group_id = j.at("kafka_group_id").get<std::string>();
            m_kafka_msg_timeout = j.at("kafka_msg_timeout").get<int>();
            m_kafka_flush_timeout = j.at("kafka_flush_timeout").get<int>();
            m_kafka_warning_partition_eof = j.at("kafka_warning_partition_eof").get<bool>();
            m_kafka_consumer_name = j.at("kafka_consumer_name").get<std::string>();
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

    std::vector<std::string> KafkaConfig::get_kafka_topics() const {
        return m_kafka_topics;
    }

    std::string KafkaConfig::get_kafka_group_id() const {
        return m_kafka_group_id;
    }

    std::string KafkaConfig::get_kafka_consumer_name() const {
        return m_kafka_consumer_name;
    }

    void KafkaConfig::set_kafka_consumer_name(const std::string &name) {
        m_kafka_consumer_name = name;
    }

    bool KafkaConfig::get_kafka_warning_partition_eof() const {
        return m_kafka_warning_partition_eof;
    }

    RdKafka::Conf *KafkaConfig::get_kafka_conf() {
        m_conf->set("group.id", m_kafka_group_id, m_errstr);
        if (m_kafka_warning_partition_eof)
            m_conf->set("enable.partition.eof", "true", m_errstr);
        else
            m_conf->set("enable.partition.eof", "false", m_errstr);
        m_conf->set("client.id", m_kafka_consumer_name, m_errstr);  // set the unique client id
        return m_conf.get();
    }

    RdKafka::Conf *KafkaConfig::get_kafka_producer_conf() {
        return m_conf.get();
    }

    int KafkaConfig::get_kafka_msg_timeout() const {
        return m_kafka_msg_timeout;
    }

    int KafkaConfig::get_kafka_flush_timeout() const {
        return m_kafka_flush_timeout;
    }

}