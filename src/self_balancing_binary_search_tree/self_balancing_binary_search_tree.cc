#include "self_balancing_binary_search_tree.h"

#include <cstddef>

using Node = class s21::SelfBalancingBinarySearchTree::Node;
using SBT = s21::SelfBalancingBinarySearchTree;
using s21::record_type;

SBT::SelfBalancingBinarySearchTree(s21::record_type &record) {
  root_ = new Node(record);
}

auto SBT::FixHeight(Node *p) -> void {
  unsigned char h_left = Height(p->left_);
  unsigned char h_right = Height(p->right_);
  p->height_ = (h_left > h_right ? h_left : h_right) + 1;
}

auto SBT::RotateRight(Node *p) -> Node * {
  Node *q = p->left_;
  p->left_ = q->right_;
  q->right_ = p;
  FixHeight(p);
  FixHeight(q);
  return q;
}

auto SBT::RotateLeft(Node *q) -> Node * {
  Node *p = q->right_;
  q->right_ = p->left_;
  p->left_ = q;
  FixHeight(q);
  FixHeight(p);
  return p;
}

auto SBT::Balance(Node *p) -> Node * {
  FixHeight(p);
  if (Bfactor(p) == 2) {
    if (Bfactor(p->right_) < 0) p->right_ = RotateRight(p->right_);
    return RotateLeft(p);
  }
  if (Bfactor(p) == -2) {
    if (Bfactor(p->left_) > 0) p->left_ = RotateLeft(p->left_);
    return RotateRight(p);
  }
  return p;
}

auto SBT::Insert(Node *p, record_type k) -> Node * {
  if (!p) return new Node(k);
  if (k.first < p->key_)
    p->left_ = Insert(p->left_, k);
  else
    p->right_ = Insert(p->right_, k);
  return Balance(p);
}

auto SBT::FindMin(Node *p) -> Node * {
  return p->left_ ? FindMin(p->left_) : p;
}

auto SBT::RemoveMin(Node *p) -> Node * {
  if (p->left_ == 0) return p->right_;
  p->left_ = RemoveMin(p->left_);
  return Balance(p);
}

auto SBT::Remove(Node *p, key_type k) -> Node * {
  if (!p) return 0;
  if (k < p->key_)
    p->left_ = Remove(p->left_, k);
  else if (k > p->key_)
    p->right_ = Remove(p->right_, k);
  else  //  k == p->key
  {
    Node *q = p->left_;
    Node *r = p->right_;
    delete p;
    if (!r) return q;
    Node *min = FindMin(r);
    min->right_ = RemoveMin(r);
    min->left_ = q;
    return Balance(min);
  }
  return Balance(p);
}

auto SBT::Set(const record_type &new_record) -> bool {
  if (!root_)
    root_ = new Node(new_record);
  else
    root_ = Insert(root_, new_record);
  return static_cast<bool>(root_);
}

auto SBT::Get(const key_type &k) -> record & {
  record *a = new record();
  return *a;
};
auto SBT::Exist(const key_type &) -> bool { return true; };
auto SBT::Del(const key_type &) -> bool { return true; };
auto SBT::Update(const record_type &) -> bool { return true; };
auto SBT::Keys() -> void{};
auto SBT::Rename(const key_type &, const key_type &) -> bool { return true; };
auto SBT::TTL(const key_type &) -> int { return 0; };
auto SBT::Find(const Person &) -> std::vector<key_type> {
  return std::vector<key_type>(0);
};
auto SBT::ShowAll() -> void{};
auto SBT::Upload(const std::string &) -> size_t {
  size_t res;
  return res;
};
auto SBT::Export(const std::string &) -> size_t {
  size_t res;
  return res;
};
auto SBT::Clear() -> void{};
