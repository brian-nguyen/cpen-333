#ifndef LAB6_MENU_H
#define LAB6_MENU_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <json.hpp>
using JSON = nlohmann::json;

/**
 * Menu items
 */
struct MenuItem {
  std::string item;
  std::string description;
  std::string meal;
  std::string course;
  float price;
  int id;
};

/**
 * Menu, containing list of items that can
 * be ordered at the restaurant
 */
class Menu {
  std::map<int,MenuItem> menu_;
  std::vector<MenuItem> appetizers_;
  std::vector<MenuItem> mains_;
  std::vector<MenuItem> desserts_;

 public:
  Menu(): menu_(), appetizers_(), mains_(), desserts_() {}

  /**
   * Loads items from a JSON file
   * @param filename file to load items from
   * @return number of new items added to the menu
   */
  size_t load(const std::string& filename) {
    std::ifstream fin(filename);

    size_t nadded = 0;
    if (fin.is_open()) {
      JSON jmenu;
      fin >> jmenu;

      for (const auto& jitem : jmenu) {
        MenuItem item;
        item.item = jitem["item"];
        item.description = jitem["description"];
        item.meal = jitem["meal"];
        item.course = jitem["course"];
        item.price = (float)jitem["price"];
        item.id = jitem["item_id"];

        // add new item
        auto it = menu_.insert({item.id, item});
        if (it.second) {
          ++nadded;

          if (item.course == "appetizer") {
            appetizers_.push_back(item);
          } else if (item.course == "main") {
            mains_.push_back(item);
          } else if (item.course == "dessert") {
            desserts_.push_back(item);
          }
        }
      }
    }
    return nadded;
  }

  /**
   * Appetizers
   * @return list of appetizers
   */
  const std::vector<MenuItem>& appetizers() const {
    return appetizers_;
  }

  /**
   * Main Courses
   * @return list of mains
   */
  const std::vector<MenuItem>& mains() const {
    return mains_;
  }

  /**
   * Desserts
   * @return list of desserts
   */
  const std::vector<MenuItem>& desserts() const {
    return desserts_;
  }

};

#endif //LAB6_MENU_H
