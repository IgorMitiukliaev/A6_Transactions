#ifndef SRC_CORE_VIRTUAL_BASE_H_
#define SRC_CORE_VIRTUAL_BASE_H_

#include <cstddef>
#include <cstring>
#include <ctime>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace s21 {

class BaseClass;
using key_type = std::string;

enum PersonMask {
  MASK_ALL = 0b11111,
  MASK_SURNAME = 0b10000,
  MASK_NAME = 0b01000,
  MASK_BIRTH_YEAR = 0b00100,
  MASK_CITY = 0b00010,
  MASK_BALANCE = 0b00001
};

class Person {
 public:
  Person() : surname_(""), name_(""), birth_year_(-1), city_(""), balance_(0){};
  Person(std::string surname, std::string name, int birth_year,
         std::string city, int balance)
      : surname_(surname),
        name_(name),
        birth_year_(birth_year),
        city_(city),
        balance_(balance){};
  ~Person() = default;
  auto ShowData() -> std::string {
    std::string res;
    res = surname_ + "; " + name_ + "; " + std::to_string(birth_year_) + "; " +
          city_ + "; " + std::to_string(balance_);
    return res;
  }
  std::string surname_;
  std::string name_;
  int birth_year_;
  std::string city_;
  int balance_;
};

struct record {
  record() : person_(){};
  record(Person person, time_t create_time = 0, int erase_time = -1,
         int mask = MASK_ALL)
      : person_(person),
        create_time_(create_time),
        erase_time_(erase_time),
        mask_(mask){};
  Person person_;
  time_t create_time_ = 0;
  int erase_time_ = -1;
  int mask_;
};

typedef std::optional<std::reference_wrapper<record>> record_nullable;
typedef std::pair<s21::key_type, s21::record> record_type;

class BaseClass {
 public:
  virtual auto Set(const record_type &) -> bool = 0;
  virtual auto Get(const key_type &) -> record_nullable = 0;
  virtual auto Exist(const key_type &) -> bool = 0;
  virtual auto Del(const key_type &) -> bool = 0;
  virtual auto Update(const record_type &) -> bool = 0;
  virtual auto Keys() -> std::vector<key_type> = 0;
  virtual auto Rename(const key_type &, const key_type &) -> bool = 0;
  // возвращаем -1 если не нашли ключ или erase_time < 0
  virtual auto TTL(const key_type &) -> int = 0;
  virtual auto Find(const Person &, int) -> std::vector<key_type> = 0;
  virtual auto ShowAll() -> std::vector<record *> = 0;
  virtual auto Upload(const std::string &path) -> size_t = 0;
  virtual auto Export(const std::string &path) -> size_t = 0;
  virtual auto Clear() -> void = 0;
};

}  // namespace s21

#endif  // SRC_CORE_VIRTUAL_BASE_H_
