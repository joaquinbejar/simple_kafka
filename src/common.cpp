//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/common.h>

namespace simple_kafka::common {

    void MetaConsumer::consume_cb(RdKafka::Message &msg, void *opaque) {
        switch (msg.err()) {
            case RdKafka::ERR__TIMED_OUT:
                logger->send<simple_logger::LogLevel::WARNING>("MetaConsumer Consumption timed out.");
                break;
            case RdKafka::ERR_NO_ERROR: {
                std::string message_payload = static_cast<const char *>(msg.payload());
                logger->send<simple_logger::LogLevel::INFORMATIONAL>("MetaConsumer Message received: " + message_payload);
                break;
            }
            case RdKafka::ERR__PARTITION_EOF:
                logger->send<simple_logger::LogLevel::DEBUG>("MetaConsumer End of partition reached.");
                break;
            case RdKafka::ERR__UNKNOWN_TOPIC:
            case RdKafka::ERR__UNKNOWN_PARTITION:
                logger->send<simple_logger::LogLevel::ERROR>("MetaConsumer Topic or partition not found.");
                break;
            default:
                logger->send<simple_logger::LogLevel::ERROR>("MetaConsumer Error in consume: " + msg.errstr());
                break;

        }
    }

    MetaConsumer::MetaConsumer(std::shared_ptr<simple_logger::Logger> logger) {
        this->logger = std::move(logger);
    }
}