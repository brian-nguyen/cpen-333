#ifndef AMAZOOM_MESSAGE_H
#define AMAZOOM_MESSAGE_H

#include <string>

#include "Order.h"
#include "Product.h"

#define ORDER_MSG 0
#define QUERY_MSG 1
#define ORDER_RESPONSE 2
#define QUERY_RESPONSE 3

class Message {
 public:
  virtual int type() const = 0;
};

class ResponseMessage : public Message {
 public:
  std::string info;
  std::string status;
  ResponseMessage(const std::string& info, const std::string& status) :
    info(info), status(status) {}
};

class OrderMessage : public Message {
 public:
  Order order;
  OrderMessage(Order& o) : order(o) { }

  int type() const {
    return ORDER_MSG;
  }
};

class OrderResponse : public ResponseMessage {
 public:
  const OrderMessage omsg;

  OrderResponse(const OrderMessage& omsg, std::string info, std::string status) :
    ResponseMessage(info, status), omsg(omsg) { }

  int type() const {
    return ORDER_RESPONSE;
  }
};

class QueryMessage : public Message {
 public:
  Product product;
  QueryMessage(Product& p) : product(p) { }

  int type() const {
    return QUERY_MSG;
  }
};

class QueryResponse : public ResponseMessage {
 public:
  const QueryMessage qmsg;
  int quantity;

  QueryResponse(const QueryMessage& qmsg, std::string info, std::string status, int quantity) :
    ResponseMessage(info, status), qmsg(qmsg), quantity(quantity) { }

  int type() const {
    return QUERY_RESPONSE;
  }
};

#endif