//
// Created by Joaquin Bejar Garcia on 23/1/24.
//
#include <simple_kafka/common.h>

namespace simple_kafka::common {

//    void MetaProducerConsumer::consume(RdKafka::Message &msg, void *opaque) {
//        switch (msg.err()) {
//            case RdKafka::ERR__TIMED_OUT:
//                logger->send<simple_logger::LogLevel::WARNING>("MetaProducerConsumer Consumption timed out.");
//                break;
//            case RdKafka::ERR_NO_ERROR: {
//                std::string message_payload = static_cast<const char *>(msg.payload());
//                logger->send<simple_logger::LogLevel::INFORMATIONAL>("MetaProducerConsumer Message received: " + *message_payload);
//                opaque = message_payload;
//                break;
//            }
//            case RdKafka::ERR__PARTITION_EOF:
//                logger->send<simple_logger::LogLevel::DEBUG>("MetaProducerConsumer End of partition reached.");
//                break;
//            case RdKafka::ERR__UNKNOWN_TOPIC:
//            case RdKafka::ERR__UNKNOWN_PARTITION:
//                logger->send<simple_logger::LogLevel::ERROR>("MetaProducerConsumer Topic or partition not found.");
//                break;
//            default:
//                logger->send<simple_logger::LogLevel::ERROR>("MetaProducerConsumer Error in consume: " + msg.errstr());
//                break;
//
//        }
//    }
//
//    std::string MetaProducerConsumer::get_msg_to_produce() {
//        return "MetaProducerConsumer Message produced: " + std::to_string(::common::dates::get_unix_timestamp());
//    }
//
//    MetaProducerConsumer::MetaProducerConsumer(std::shared_ptr<simple_logger::Logger> logger) {
//        this->logger = std::move(logger);
//    }
}