//
// Created by Joaquin Bejar Garcia on 23/1/24.
//

#ifndef SIMPLE_KAFKA_COMMON_H
#define SIMPLE_KAFKA_COMMON_H

#include <rdkafkacpp.h>

namespace simple_kafka::common {

    class MetaConsumer : public RdKafka::EventCb, public RdKafka::RebalanceCb, public RdKafka::ConsumeCb {
    public:
        void event_cb(RdKafka::Event &event) override;

        void rebalance_cb(RdKafka::KafkaConsumer *consumer, RdKafka::ErrorCode err,
                          std::vector<RdKafka::TopicPartition *> &partitions) override;

        void consume_cb(RdKafka::Message &msg, void *opaque) override;
    };

}
#endif //SIMPLE_KAFKA_COMMON_H
