#pragma once

#include <cstddef>
#include <string>

namespace s21 {

class BaseClass;
class SelfBalancingBinarySearchTree;

using key_type = std::string;
using SBT = SelfBalancingBinarySearchTree;

class Person {
 private:
  std::string surname;
  std::string name;
  int birth_year;
  std::string city;
  int balance;
};

struct record_type {
  key_type key;
  Person person;
  time_t create_time = 0;
  int erase_time = -1;
};

class BaseClass {
 public:
  virtual auto Set(const record_type &) -> bool;
  virtual auto Get(const key_type &) -> record_type &;
  virtual auto Exist(const key_type &) -> bool;
  virtual auto Del(const key_type &) -> bool;
  virtual auto Update(const record_type &) -> bool;
  virtual auto Keys() -> void;
  virtual auto Rename(const key_type &, const key_type &) -> bool;
  virtual auto TTL(const key_type &) -> int;
  virtual auto Find(const Person &) -> void;
  virtual auto ShowAll() -> void;
  virtual auto Upload(const std::string &) -> size_t;
  virtual auto Export(const std::string &) -> size_t;
  virtual auto Clear() -> void;
};

}  // namespace s21