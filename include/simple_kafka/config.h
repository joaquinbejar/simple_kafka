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
        std::string m_kafka_topic = get_env_variable_string("KAFKA_TOPIC", "");
        std::string m_kafka_group_id = get_env_variable_string("KAFKA_GROUP_ID", "");
        std::unique_ptr<RdKafka::Conf> m_conf = std::unique_ptr<RdKafka::Conf>(
                RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL));
        std::string m_errstr;


    public:
        [[nodiscard]] std::string get_kafka_brokers() const;

        [[nodiscard]] std::string get_kafka_topic() const;

        [[nodiscard]] std::string get_kafka_group_id() const;

        [[nodiscard]] RdKafka::Conf *get_kafka_conf();

        // add a logger to the config
        std::shared_ptr<simple_logger::Logger> logger = std::make_shared<simple_logger::Logger>(loglevel);
    };

}
#endif //SIMPLE_KAFKA_CONFIG_H
