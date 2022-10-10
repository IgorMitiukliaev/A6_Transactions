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
 public:
  Person() : surname_(""), name_(""), birth_year_(-1), city_(""), balance_(0){};

 private:
  std::string surname_;
  std::string name_;
  int birth_year_;
  std::string city_;
  int balance_;
};

struct record {
  Person person_;
  time_t create_time_ = 0;
  int erase_time_ = -1;
  record() : person_(){};
};

typedef std::pair<s21::key_type, s21::record> record_type;

class BaseClass {
 public:
  virtual auto Set(const record_type &) -> bool = 0;
  virtual auto Get(const key_type &) -> record & = 0;
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
