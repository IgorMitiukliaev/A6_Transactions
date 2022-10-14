/*
https://habr.com/ru/post/509220/
*/

#ifndef SRC_HASH_TABLE_HASH_TABLE_H_
#define SRC_HASH_TABLE_HASH_TABLE_H_

#include <iostream>

#include "../core/virtual_base.h"

namespace s21 {

class HashTable : public BaseClass {
 public:
  HashTable();
  HashTable(const HashTable &);
  ~HashTable() override;
  auto operator=(const HashTable &) -> HashTable &;
  auto Set(const record_type &) -> bool override;
  auto Get(const key_type &) -> record_nullable override;
  auto Exist(const key_type &) -> bool override;
  auto Del(const key_type &) -> bool override;
  auto Update(const record_type &) -> bool override;
  auto Keys() -> std::vector<key_type> override;
  auto Rename(const key_type &, const key_type &) -> bool override;
  auto TTL(const key_type &) -> int override;
  auto Find(const Person &, int) -> std::vector<key_type> override;
  auto ShowAll() -> std::vector<record *> override;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void override;
  auto Update() -> void;
  inline auto GetType() -> s21::BaseType override { return s21::HASH; };

 private:
  static const int default_size_ = 8;
  constexpr static const double rehash_size_ = 0.75;

  struct Node {
    key_type key_;
    record data_;
    bool state_;
    bool empty_;

    explicit Node(const record_type record)
        : key_(record.first),
          data_(record.second),
          state_(true),
          empty_(false) {}

    Node(const key_type &key, const record &data)
        : key_(key), data_(data), state_(true), empty_(false) {}
    Node() : key_(""), data_(), state_(true), empty_(true) {}
    Node(const Node &n)
        : key_(n.key_), data_(n.data_), state_(true), empty_(true){};
  };
  Node **arr_ = nullptr;
  int size_ = 0;
  int buffer_size_ = 0;
  int size_all_non_nullptr_ = 0;

  auto Hash1(const std::string &s) -> int;
  auto Hash2(const std::string &s) -> int;
  auto Resize() -> void;
  auto Rehash() -> void;
  auto Add(const key_type &key, const record &data) -> bool;
  auto FindRecord(const key_type &key) -> Node *;
  auto CheckNode(const key_type &key, const Person &person, int mask) -> bool;
};

}  // namespace s21

#endif  // SRC_HASH_TABLE_HASH_TABLE_H_
