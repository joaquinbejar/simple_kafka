//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_COMMON_H
#define SIMPLE_KAFKA_COMMON_H

#include <rdkafkacpp.h>
#include <iostream>
#include <simple_logger/logger.h>
#include <common/dates.h>

namespace simple_kafka::common {

    class MetaProducerConsumer {

    private:
        std::shared_ptr<simple_logger::Logger> logger;

    public:
        explicit MetaProducerConsumer(std::shared_ptr<simple_logger::Logger> logger);
        virtual void consume(RdKafka::Message &msg) ;
        virtual std::string get_msg_to_produce() ;
    };


}
#endif //SIMPLE_KAFKA_COMMON_H
