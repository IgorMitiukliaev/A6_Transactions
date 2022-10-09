#pragma once

#include "../core/virtual_base.h"

namespace s21 {
int HashFunctionHorner(const std::string &s, int table_size, const int key);
int hash1(const std::string& s, int table_size);
int hash2(const std::string& s, int table_size);

template <class T>
class HashTable : public BaseClass {
 public:
  HashTable();
  ~HashTable();
  auto Set(const record_type &) -> bool;
  auto Get(const key_type &) -> record_type &;
  auto Exist(const key_type &) -> bool;
  auto Del(const key_type &) -> bool;
  auto Update(const record_type &) -> bool;
  auto Keys() -> void;
  auto Rename(const key_type &, const key_type &) -> bool;
  auto TTL(const key_type &) -> int;
  auto Find(const T &) -> key_type;
  auto ShowAll() -> void;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void;
  bool Add(const T &value);

 private:
  static const int default_size = 8;  // начальный размер нашей таблицы
  constexpr static const double rehash_size =
      0.75;  // коэффициент, при котором произойдет увеличение таблицы
  struct Node {
    T value;
    bool state;  // если значение флага state = false, значит элемент массива
                 // был удален (deleted)
    Node(const T &value_) : value(value_), state(true) {}
  };
  Node **arr;  // соответственно в массиве будут хранится структуры Node*
  int size;  // сколько элементов у нас сейчас в массиве (без учета deleted)
  int buffer_size;  // размер самого массива, сколько памяти выделено под
                    // хранение нашей таблицы
  int size_all_non_nullptr;  // сколько элементов у нас сейчас в массиве (с
                             // учетом deleted)

  auto Resize() -> void;
  auto Rehash() -> void;
};

}  // namespace s21