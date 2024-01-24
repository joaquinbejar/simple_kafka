//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/common.h>

namespace simple_kafka::common {

    void MetaConsumer::event_cb(RdKafka::Event &event) {
        // Manejar eventos, como desconexiones, estadísticas, etc.
    }

//    void MetaConsumer::rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err,
//                                    std::vector<RdKafka::TopicPartition *> &partitions) {
//        // Manejar el reequilibrio de particiones
//    }
    void MetaConsumer::rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err, std::vector<RdKafka::TopicPartition *> &partitions) {
        if (err == RdKafka::ERR__ASSIGN_PARTITIONS) {
            // Asignación de particiones: se llama cuando el grupo ha sido asignado sus particiones
            std::cout << "Assigning partitions: ";
            for (auto &partition : partitions) {
                std::cout << partition->topic() << "[" << partition->partition() << "], ";
            }
            std::cout << std::endl;
            consumer->assign(partitions);
        } else if (err == RdKafka::ERR__REVOKE_PARTITIONS) {
            // Desasignación de particiones: se llama antes de que el consumidor abandone el grupo
            std::cout << "Revoking partitions: ";
            for (auto &partition : partitions) {
                std::cout << partition->topic() << "[" << partition->partition() << "], ";
            }
            std::cout << std::endl;
            consumer->unassign();
        } else {
            std::cerr << "Rebalance failed with error code: " << RdKafka::err2str(err) << std::endl;
        }
    }


    void MetaConsumer::consume_cb(RdKafka::Message &msg, void *opaque) {
            if (msg.err() == RdKafka::ERR_NO_ERROR) {
                // Mensaje recibido correctamente
                std::cout << "Message received: " << static_cast<const char*>(msg.payload()) << std::endl;
            } else if (msg.err() == RdKafka::ERR__TIMED_OUT) {
                // El consumo ha excedido el tiempo límite
                std::cout << "Consumption timed out." << std::endl;
            } else {
                // Manejar otros errores
                std::cerr << "Error in consume: " << msg.errstr() << std::endl;
            }
        }


}