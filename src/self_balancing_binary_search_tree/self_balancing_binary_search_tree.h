/*
https://habr.com/ru/post/150732/
*/

#ifndef SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include <cstddef>
#include <iostream>
#include <locale>
#include <vector>

#include "../core/virtual_base.h"

namespace s21 {

class SelfBalancingBinarySearchTree : public BaseClass {
 public:
  struct Node {
    explicit Node(record_type k)
        : key_{k.first}, data_{k.second}, left_{0}, right_(0), height_{1} {};
    key_type key_;
    record data_;
    Node *left_;
    Node *right_;
    unsigned char height_;
  };
  SelfBalancingBinarySearchTree();
  ~SelfBalancingBinarySearchTree() override;
  auto Set(const record_type &) -> bool override;
  auto Get(const key_type &) -> record_nullable override;
  auto Exist(const key_type &) -> bool override;
  auto Del(const key_type &) -> bool override;
  auto Update(const record_type &) -> bool override;
  auto Keys() -> std::vector<key_type> override;
  auto Rename(const key_type &, const key_type &) -> bool override;
  auto TTL(const key_type &) -> int override;
  auto Find(const Person &, int mask) -> std::vector<key_type> override;
  auto ShowAll() -> std::vector<record *> override;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void override;
  auto Update() -> void;
  inline auto GetType() -> s21::BaseType override { return s21::SBT; };

 private:
  using func_t = std::function<void(Node *)>;
  Node *root_ = nullptr;
  inline auto Height(Node *p) -> unsigned char { return p ? p->height_ : 0; }
  inline auto Bfactor(Node *p) -> int {
    return Height(p->right_) - Height(p->left_);
  }

  auto FixHeight(Node *p) -> void;
  auto RotateRight(Node *p) -> Node *;  // правый поворот вокруг p
  auto RotateLeft(Node *p) -> Node *;  // левый поворот вокруг p
  auto Balance(Node *p) -> Node *;     // балансировка узла p
  auto Insert(Node *p, record_type k)
      -> Node *;  // вставка ключа k в дерево с корнем p
  auto FindMin(Node *p)
      -> Node *;  // поиск узла с минимальным ключом в дереве p
  auto RemoveMin(Node *p)
      -> Node *;  // удаление узла с минимальным ключом из дерева p
  auto Remove(Node *p, key_type k, bool &)
      -> Node *;  // удаление ключа k из дерева p
  auto FindRecord(Node *p, key_type k) -> Node *;
  auto preOrder(Node *p, std::vector<key_type> &res) -> void;
  auto preOrder(Node *p, std::vector<record *> &res) -> void;
  auto preOrder(Node *p, func_t f) -> void;
  auto preOrderFind(Node *p, const Person &person, int mask,
                    std::vector<key_type> &res) -> void;
  auto checkNode(Node *, const Person &, int) -> bool;
};

}  // namespace s21

#endif  // SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_
