#include <iostream>

#include "TestException.h"

#include "Shelf.h"
#include "Product.h"

void testAddProduct(Shelf& s, Product& p) {
  s.add(p);
  
  for (const auto& product : s.products()) {
    if (product == p) return;
  }

  throw TestException(std::string("Product not added properly"));
}

void testAddDuplicateProduct(Shelf& s, Product& p) {
  s.add(p);
  
  for (const auto& product : s.products()) {
    if (product == p && product.quantity_ == 4) return;
  }

  throw TestException(std::string("Product not added properly"));
}

void testRemoveProduct(Shelf& s, Product& p) {
  s.remove(p);

  for (const auto& product : s.products()) {
    if (product == p) throw TestException(std::string("Product not removed"));
  }
}

int main() {
  std::pair<int, int> loc(1, 1);
  Shelf s(999, loc);

  try {
    Product p("fork", 4.2, 2);
    testAddProduct(s, p);
    testAddDuplicateProduct(s, p);
    testRemoveProduct(s, p);
    testRemoveProduct(s, p);
    
    std::cout << "All tests passed" << std::endl;
  } catch (TestException& exc) {
    std::cout << exc.what() << std::endl;
  }
  
  return 0;
}