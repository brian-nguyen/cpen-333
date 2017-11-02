#ifndef LAB6_ORDER_H
#define LAB6_ORDER_H

/**
 * Basic order information containing a customer id and item id
 */
struct Order {
  int customer_id;
  int item_id;

  bool operator==(const Order& other) const {
    return ((customer_id == other.customer_id)
        && (item_id == other.item_id));
  }

  bool operator!=(const Order& other) const {
    return !(*this == other);
  }
};

#endif //LAB6_ORDER_H
