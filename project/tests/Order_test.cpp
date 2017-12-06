#include <iostream>
#include <map>

#include "TestException.h"
#include "Order.h"
#include "Product.h"

void testAddProduct(Order& o, Product& p) {
  o.add(p, 1);
  auto it = o.products().find(p);

  if (it != o.products().end()) return;

  throw TestException(std::string("Product not added properly"));
}

void testAddDuplicateProduct(Order& o, Product& p) {
  o.add(p, 1);
  std::map<Product, int> products = o.products();

  auto it = products.find(p);  
  if (it != products.end() && (it->second == 2)) return;

  throw TestException(std::string("Product not added properly"));
}

void testRemoveProduct(Order& o, Product& p) {
  o.remove(p);

  auto it = o.products().find(p);
  if (it == o.products().end()) return;

  throw TestException(std::string("Product not removed properly"));
}

int main() {
  Order o(0, 0);

  try {
    Product p("fork", 4.2);
    Product p2("spoon", 3.3);
    testAddProduct(o, p);
    testAddProduct(o, p2);    
    testAddDuplicateProduct(o, p);
    testRemoveProduct(o, p);
    testRemoveProduct(o, p);
    
    std::cout << "All tests passed" << std::endl;
  } catch (TestException& exc) {
    std::cout << exc.what() << std::endl;
  }
  
  return 0;
}