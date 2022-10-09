#include "self_balancing_binary_search_tree.h"


using Node = class  s21::SelfBalancingBinarySearchTree::Node;


using SBT = s21::SelfBalancingBinarySearchTree;


SBT::SelfBalancingBinarySearchTree(s21::record_type &record) {
  root = new Node(record);
}

// auto SBT::Set(const record_type &) -> bool {}

auto SBT::Height(Node *p) -> unsigned char {
  return p ? p->height_ : 0;
};


auto SBT::Bfactor(Node *p) -> int {
  return Height(p->right_) - Height(p->left_);
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