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
        KafkaClientConsumer(config::KafkaConfig config);

        ~KafkaClientConsumer();

        void subscribe();

        void unsubscribe();

        void consume();

        void stop();


    private:
        config::KafkaConfig m_config;
        std::shared_ptr<RdKafka::KafkaConsumer> m_consumer;
//        common::MetaConsumer exConsumer;
        std::atomic<bool> m_run_consume = false;
        std::thread m_consumeThread;
        std::mutex m_Mutex;

        void m_consume();

    };

}
#endif //SIMPLE_KAFKA_CLIENT_H
