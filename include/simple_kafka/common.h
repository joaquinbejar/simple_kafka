//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_COMMON_H
#define SIMPLE_KAFKA_COMMON_H

#include <rdkafkacpp.h>
#include <iostream>
#include <simple_logger/logger.h>

namespace simple_kafka::common {

    class MetaConsumer {

    private:
        std::shared_ptr<simple_logger::Logger> logger;

    public:
        explicit MetaConsumer(std::shared_ptr<simple_logger::Logger> logger);
        virtual void consume_cb(RdKafka::Message &msg) ;
    };


}
#endif //SIMPLE_KAFKA_COMMON_H
