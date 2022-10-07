#pragma once

#include "../core/virtual_base.h"

namespace s21 {

class HashTable : public BaseClass {
 public:
  HashTable(const HashTable &) = default;
  HashTable(HashTable &&) = default;
  HashTable &operator=(const HashTable &) = default;
  HashTable &operator=(HashTable &&) = default;

  auto Set(const record_type &) -> bool;
  auto Get(const key_type &) -> record_type &;
  auto Exist(const key_type &) -> bool;
  auto Del(const key_type &) -> bool;
  auto Update(const record_type &) -> bool;
  auto Keys() -> void;
  auto Rename(const key_type &, const key_type &) -> bool;
  auto TTL(const key_type &) -> int;
  auto Find(const Person &) -> void;
  auto ShowAll() -> void;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void;
};

}  // namespace s21