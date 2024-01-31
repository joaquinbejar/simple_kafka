//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_CONFIG_H
#define SIMPLE_KAFKA_CONFIG_H

#include <simple_config/config.h>
#include <simple_logger/logger.h>
#include <common/common.h>
#include <simple_kafka/common.h>

namespace simple_kafka::config {

    using ::common::get_env_variable_string;
    using ::common::get_env_variable_vector_string;
    using ::common::get_env_variable_int;

    class KafkaConfig : public simple_config::Config {
    public:

        KafkaConfig();

        bool validate() override;

        [[nodiscard]] json to_json() const override;

        void from_json(const json &j) override;

        [[nodiscard]] std::string to_string() const override;


    private:
        void m_set_kafka_conf();

    protected:
        std::string m_kafka_brokers = get_env_variable_string("KAFKA_BROKERS", "localhost:9092");
        std::vector<std::string> m_kafka_topics = get_env_variable_vector_string("KAFKA_TOPICS", "");
        std::string m_kafka_group_id = get_env_variable_string("KAFKA_GROUP_ID", "");
        int m_kafka_msg_timeout = get_env_variable_int("KAFKA_MSG_TIMEOUT", 3000);
        std::shared_ptr<RdKafka::Conf> m_conf = std::shared_ptr<RdKafka::Conf>(
                RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
        std::string m_errstr;
        int m_kafka_flush_timeout = get_env_variable_int("KAFKA_FLUSH_TIMEOUT", 1000);


    public:
        [[nodiscard]] std::string get_kafka_brokers() const;

        [[nodiscard]] std::vector<std::string> get_kafka_topics() const;

        [[nodiscard]] std::string get_kafka_group_id() const;

        [[nodiscard]] RdKafka::Conf *get_kafka_conf();

        [[nodiscard]] RdKafka::Conf *get_kafka_producer_conf();

        [[nodiscard]] int get_kafka_msg_timeout() const;

        [[nodiscard]] int get_kafka_flush_timeout() const;

        // add a logger to the config
        std::shared_ptr<simple_logger::Logger> logger = std::make_shared<simple_logger::Logger>(loglevel);
    };

}
#endif //SIMPLE_KAFKA_CONFIG_H
