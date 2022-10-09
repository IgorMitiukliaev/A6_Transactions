#ifndef SRC_CONTROLLER_CONTROLLER_H_
#define SRC_CONTROLLER_CONTROLLER_H_

#include <string>

#include "../hash_table/hash_table.h"
#include "../self_balancing_binary_search_tree/self_balancing_binary_search_tree.h"

namespace s21 {
class Controller {
 public:
  enum BaseType { HASH, SBT };
  auto Init(const BaseType type) -> void;
  auto AddElement(const std::string& input_str) -> void;
  auto GetElement(const std::string& input_str) -> void;
  auto ExistElement(const std::string& input_str) -> void;
  auto DeleteElement(const std::string& input_str) -> void;
  auto UpdateElement(const std::string& input_str) -> void;
  auto ShowKeys(const std::string& input_str) -> void;
  auto RenameKey(const std::string& input_str) -> void;
  auto FindElement(const std::string& input_str) -> void;
  auto ShowTTL(const std::string& input_str) -> void;
  auto ShowAll(const std::string& input_str) -> void;
  auto UploadData(const std::string& input_str) -> void;
  auto ExportData(const std::string& input_str) -> void;
  auto ClearStorage() -> void;
  auto Research() -> void;

 private:
  s21::BaseClass* model_ = nullptr;
};

};  // namespace s21

#endif  // SRC_CONTROLLER_CONTROLLER_H_