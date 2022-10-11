#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include <algorithm>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

// #include "../hash_table/hash_table.h"
#include "../self_balancing_binary_search_tree/self_balancing_binary_search_tree.h"

namespace s21 {
class Controller {
 public:
  enum BaseType { HASH, SBT };
  auto Init(const BaseType type) -> void;
  auto AddElement(const std::string key, const std::string surname,
                  const std::string name, const int birth_year,
                  const std::string city, const int balance, int erase_time)
      -> bool;
  auto GetElement(const std::string& key) -> const std::string;
  auto ExistElement(const std::string& key) -> bool;
  auto DeleteElement(const std::string& key) -> bool;
  auto UpdateElement(const std::string key,
                     const std::optional<std::string> surname,
                     const std::optional<std::string> name,
                     const std::optional<int> birth_year,
                     const std::optional<std::string> city,
                     const std::optional<int> balance) -> bool;
  auto ShowKeys() -> std::string;
  auto RenameKey(const std::string&) -> void;
  auto FindElement(const std::string&) -> void;
  auto ShowTTL(const std::string&) -> void;
  auto ShowAll(const std::string&) -> void;
  auto UploadData(const std::string&) -> bool;
  auto ExportData(const std::string&) -> bool;
  auto ClearStorage() -> void;

 private:
  s21::BaseClass* model_ = nullptr;
};

};  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLER_H_