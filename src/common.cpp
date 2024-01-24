//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/common.h>

namespace simple_kafka::common {

    void MetaConsumer::event_cb(RdKafka::Event &event) {
        // Manejar eventos, como desconexiones, estadísticas, etc.
    }

    void MetaConsumer::rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err,
                                    std::vector<RdKafka::TopicPartition *> &partitions) {
        // Manejar el reequilibrio de particiones
    }

    void MetaConsumer::consume_cb(RdKafka::Message &msg, void *opaque) {
        // Manejar mensajes recibidos

        std::cout << "Message received: " << msg.payload() << std::endl;
    }

}