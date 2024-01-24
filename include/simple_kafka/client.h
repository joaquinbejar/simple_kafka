//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_CLIENT_H
#define SIMPLE_KAFKA_CLIENT_H

#include <simple_kafka/config.h>
#include <simple_kafka/common.h>

namespace simple_kafka::client {



    class KafkaClientConsumer {

    public:
        KafkaClientConsumer(std::unique_ptr< config::KafkaConfig> config);

        void subscribe();


    private:
        std::unique_ptr< config::KafkaConfig> m_config;
        std::unique_ptr<RdKafka::KafkaConsumer> m_consumer;

    };

}
#endif //SIMPLE_KAFKA_CLIENT_H
