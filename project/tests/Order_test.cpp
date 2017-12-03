#include <iostream>

#include "TestException.h"
#include "Order.h"
#include "Product.h"

void testAddProduct(Order& o, Product& p) {
  o.add(p);
  
  for (const auto& product : o.products()) {
    if (product == p) return;
  }

  throw TestException(std::string("Product not added properly"));
}

void testAddDuplicateProduct(Order& o, Product& p) {
  o.add(p);
  
  for (const auto& product : o.products()) {
    if (product == p && product.quantity_ == 4) return;
  }

  throw TestException(std::string("Product not added properly"));
}

void testRemoveProduct(Order& o, Product& p) {
  o.remove(p);

  for (const auto& product : o.products()) {
    if (product == p) throw TestException(std::string("Product not removed"));
  }
}

int main() {
  Order o(0, 0);

  try {
    Product p("fork", 4.2, 2);
    testAddProduct(o, p);
    testAddDuplicateProduct(o, p);
    testRemoveProduct(o, p);
    testRemoveProduct(o, p);
    
    std::cout << "All tests passed" << std::endl;
  } catch (TestException& exc) {
    std::cout << exc.what() << std::endl;
  }
  
  return 0;
}