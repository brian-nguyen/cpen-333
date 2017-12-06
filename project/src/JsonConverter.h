#ifndef AMAZOOM_JSON_H
#define AMAZOOM_JSON_H

#include <string>
#include <vector>
#include <map>

#include <json.hpp>
using JSON = nlohmann::json;

#include "safe_printf.h"
#include "Message.h"
#include "Product.h"
#include "Order.h"

class JsonConverter {
 public:

  static JSON toJSON(const Product& p) {
    JSON j;
    j["name"] = p.name_;
    j["weight"] = 0;
    return j;
  }

  static JSON toJSON(const std::map<Product, int>& products) {
    JSON j;
    for (const auto& pair : products) {
      JSON p;
      p["quantity"] = pair.second;
      p["product"] = toJSON(pair.first);
      j.push_back(p);
    }
    return j;
  }

  static JSON toJSON(const Order& o) {
    JSON j;
    j["id"] = o.id_;
    j["status"] = o.status_;
    j["products"] = toJSON(o.products_);
    return j;
  }

  static JSON toJSON(std::vector<Order>& orders) {
    JSON j;
    for (auto& o : orders) {
      j.push_back(toJSON(o));
    }
    return j;
  }

  static JSON toJSON(const OrderMessage& omsg) {
    JSON j;
    j["type"] = ORDER_MSG;
    j["order"] = toJSON(omsg.order);
    return j;
  }

  static JSON toJSON(const OrderResponse& omsg_response) {
    JSON j;
    j["type"] = ORDER_RESPONSE;
    j["info"] = omsg_response.info;
    j["status"] = omsg_response.status;
    j["original"] = toJSON(omsg_response.omsg);
    return j;
  }

  static JSON toJSON(const QueryMessage& qmsg) {
    JSON j;
    j["type"] = QUERY_MSG;
    j["product"] = toJSON(qmsg.product);
    return j;
  }

  static JSON toJSON(const QueryResponse& qmsg_response) {
    JSON j;
    j["type"] = QUERY_RESPONSE;
    j["info"] = qmsg_response.info;
    j["status"] = qmsg_response.status;
    j["quantity"] = qmsg_response.quantity;
    j["original"] = toJSON(qmsg_response.qmsg);
    return j;
  }

  static JSON toJSON(const Message& msg) {
    switch (msg.type()) {
      case ORDER_MSG: {
        return toJSON((OrderMessage&)msg);
      }
      case ORDER_RESPONSE: {
        return toJSON((OrderResponse&)msg);
      }
      case QUERY_MSG: {
        return toJSON((QueryMessage&)msg);
      }
      case QUERY_RESPONSE: {
        return toJSON((QueryResponse&)msg);
      }
      default: {

      }
    }

    // unknown message type
    JSON err;
    err["status"] = -1;
    err["info"] = std::string("Unknown message type");
    return err;
  }

  static Product parseProduct(const JSON& j) {
    return Product(j["name"], (double)j["weight"]);
  }

  static std::map<Product, int> parseProducts(const JSON& j) {
    std::map<Product, int> out;
    for (const auto& p : j) {
      Product product = parseProduct(p["product"]);
      out.insert({product, p["quantity"]});
    }
    return out;
  }

  static Order parseOrder(const JSON& j) {
    return Order(j["id"], j["status"], parseProducts(j["products"]));
  }

  static OrderMessage parseOrderMessage(const JSON& j) {
    Order o = parseOrder(j["order"]);
    return OrderMessage(o);
  }

  static OrderResponse parseOrderResponse(const JSON& j) {
    OrderMessage omsg = parseOrderMessage(j["original"]);
    std::string info = j["info"];
    std::string status = j["status"];
    return OrderResponse(omsg, info, status);
  }

  static QueryMessage parseQueryMessage(const JSON& j) {
    Product o = parseProduct(j["product"]);
    return QueryMessage(o);
  }

  static QueryResponse parseQueryResponse(const JSON& j) {
    QueryMessage qmsg = parseQueryMessage(j["original"]);
    std::string info = j["info"];
    std::string status = j["status"];
    int quantity = j["quantity"];
    return QueryResponse(qmsg, info, status, quantity);
  }

  static std::unique_ptr<Message> parseMessage(const JSON &jmsg) {
    int type = jmsg["type"];
    switch(type) {
      case ORDER_MSG: {
        return std::unique_ptr<Message>(new OrderMessage(parseOrderMessage(jmsg)));
      }
      case ORDER_RESPONSE: {
        return std::unique_ptr<Message>(new OrderResponse(parseOrderResponse(jmsg)));
      }
      case QUERY_MSG: {
        return std::unique_ptr<Message>(new QueryMessage(parseQueryMessage(jmsg)));
      }
      case QUERY_RESPONSE: {
        return std::unique_ptr<Message>(new QueryResponse(parseQueryResponse(jmsg)));        
      }
      default: {
        
      }
    }

    return std::unique_ptr<Message>(nullptr);
  }

};

#endif