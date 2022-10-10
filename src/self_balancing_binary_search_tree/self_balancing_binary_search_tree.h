/*
https://habr.com/ru/post/150732/
*/

#ifndef SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_
#define SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_

#include "../core/virtual_base.h"

namespace s21 {

class SelfBalancingBinarySearchTree : public BaseClass {
 public:
  struct Node {
    Node() = delete;
    key_type key_;
    record data_;
    Node *left_;
    Node *right_;
    unsigned char height_;
    Node(record_type k)
        : key_{k.first}, data_{k.second}, left_{0}, right_(0), height_{1} {}
  };
  SelfBalancingBinarySearchTree() = default;
  explicit SelfBalancingBinarySearchTree(record_type &record);
  ~SelfBalancingBinarySearchTree();
  auto Set(const record_type &) -> bool;
  auto Get(const key_type &) -> record &;
  auto Exist(const key_type &) -> bool;
  auto Del(const key_type &) -> bool;
  auto Update(const record_type &) -> bool;
  auto Keys() -> std::vector<key_type>;
  auto Rename(const key_type &, const key_type &) -> bool;
  auto TTL(const key_type &) -> int;
  auto Find(const Person &) -> std::vector<key_type>;
  auto ShowAll() -> void;
  auto Upload(const std::string &) -> size_t;
  auto Export(const std::string &) -> size_t;
  auto Clear() -> void;

 private:
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
  auto Remove(Node *p, key_type k) -> Node *;  // удаление ключа k из дерева p
  auto FindRecord(Node *p, key_type k) -> Node *;
  auto preOrder(Node *p, std::vector<key_type> &res) -> void;
};

}  // namespace s21

#endif  // SRC_SELF_BALANCING_BINARY_SEARCH_TREE_SELF_BALANCING_BINARY_SEARCH_TREE_H_
