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
  auto Get(const key_type &) -> record_type &;
  auto Exist(const key_type &) -> bool;
  auto Del(const key_type &) -> bool;
  auto Update(const record_type &) -> bool;
  auto Keys() -> void;
  auto Rename(const key_type &, const key_type &) -> bool;
  auto TTL(const key_type &) -> int;
  auto Find(const Person &) -> std::vector<key_type>;
  auto ShowAll() -> void;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void;



 private:
  static const int default_size = 8;  // начальный размер нашей таблицы
  constexpr static const double rehash_size =
      0.75;  // коэффициент, при котором произойдет увеличение таблицы
  struct Node {
    key_type key_;
    Person data_;
    bool state_;  // если значение флага state = false, значит элемент массива
                 // был удален (deleted)
    Node(const record_type &rec) : key(rec.), state(true) {}
  };
  Node **arr;  // соответственно в массиве будут хранится структуры Node*
  int size;  // сколько элементов у нас сейчас в массиве (без учета deleted)
  int buffer_size;  // размер самого массива, сколько памяти выделено под
                    // хранение нашей таблицы
  int size_all_non_nullptr;  // сколько элементов у нас сейчас в массиве (с
                             // учетом deleted)

  bool Add(const key_type &key);
  auto Resize() -> void;
  auto Rehash() -> void;
};

}  // namespace s21

#endif  // SRC_HASH_TABLE_HASH_TABLE_H_
