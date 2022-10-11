/*
https://habr.com/ru/post/509220/
*/

#ifndef SRC_HASH_TABLE_HASH_TABLE_H_
#define SRC_HASH_TABLE_HASH_TABLE_H_

#include "../core/virtual_base.h"

namespace s21 {
int HashFunctionHorner(const std::string &s, int table_size, const int key);
int hash1(const std::string &s, int table_size);
int hash2(const std::string &s, int table_size);

class HashTable : public BaseClass {
 public:
  HashTable();
  ~HashTable();
  auto Set(const record_type &) -> bool;
  auto Get(const key_type &) -> record &;
  auto Exist(const key_type &) -> bool;
  auto Del(const key_type &) -> bool;
  auto Update(const record_type &) -> bool;
  auto Keys() -> std::vector<key_type> = 0;
  auto Rename(const key_type &, const key_type &) -> bool;
  auto TTL(const key_type &) -> int;
  auto Find(const Person &) -> std::vector<key_type>;
  auto ShowAll() -> void;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void;

 private:
  static const int default_size_ = 8;  // начальный размер нашей таблицы
  constexpr static const double rehash_size_ = 0.75;
  // коэффициент, при котором произойдет увеличение таблицы

  struct Node {
    key_type key_;
    record data_;
    bool state_;  // если значение флага state = false, значит элемент массива
                  // был удален (deleted)
    Node(const record_type record)
        : key_(record.first), data_(record.second), state_(true) {}

    Node(const key_type &key, const record &data)
        : key_(key), data_(data), state_(true) {}
  };
  Node **arr_;  // соответственно в массиве будут хранится структуры Node*
  int size_;  // сколько элементов у нас сейчас в массиве (без учета deleted)
  int buffer_size_;  // размер самого массива, сколько памяти выделено под
                     // хранение нашей таблицы
  int size_all_non_nullptr_;  // сколько элементов у нас сейчас в массиве (с
                              // учетом deleted)

  // bool Add(const Node &unit);

  bool Add(const key_type &key, const record &data);

  auto Resize() -> void;
  auto Rehash() -> void;
};

}  // namespace s21

#endif  // SRC_HASH_TABLE_HASH_TABLE_H_
