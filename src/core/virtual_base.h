#ifndef SRC_CORE_VIRTUAL_BASE_H_
#define SRC_CORE_VIRTUAL_BASE_H_

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace s21 {

class BaseClass;
using key_type = std::string;

class Person {
 private:
  std::string surname;
  std::string name;
  int birth_year;
  std::string city;
  int balance;
};

struct record {
  Person person;
  time_t create_time = 0;
  int erase_time = -1;
};

typedef std::pair<s21::key_type, s21::record> record_type;

class BaseClass {
 public:
  virtual auto Set(const record_type &) -> bool = 0;
  virtual auto Get(const key_type &) -> record_type & = 0;
  virtual auto Exist(const key_type &) -> bool = 0;
  virtual auto Del(const key_type &) -> bool = 0;
  virtual auto Update(const record_type &) -> bool = 0;
  virtual auto Keys() -> void = 0;
  virtual auto Rename(const key_type &, const key_type &) -> bool = 0;
  virtual auto TTL(const key_type &) -> int = 0;
  virtual auto Find(const Person &) -> std::vector<key_type> = 0;
  virtual auto ShowAll() -> void = 0;
  virtual auto Upload(const std::string &) -> size_t = 0;
  virtual auto Export(const std::string &) -> size_t = 0;
  virtual auto Clear() -> void = 0;
};

}  // namespace s21

#endif  // SRC_CORE_VIRTUAL_BASE_H_
