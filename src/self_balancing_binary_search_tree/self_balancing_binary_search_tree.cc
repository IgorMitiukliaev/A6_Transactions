#include "self_balancing_binary_search_tree.h"

using Node = class s21::SelfBalancingBinarySearchTree::Node;
using SBT = s21::SelfBalancingBinarySearchTree;
using s21::record_type;

SBT::SelfBalancingBinarySearchTree() : BaseClass::BaseClass(), root_(nullptr) {}

SBT::~SelfBalancingBinarySearchTree() {
  func_t f = [this](Node *p) {
    if (p) {
      if (p->data_.erase_time_ > 0 &&
          p->data_.create_time_ + p->data_.erase_time_ < std::time(NULL))
        Del(p->key_);
    }
  };
  preOrder(root_, f);
};

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

auto SBT::Del(const key_type &k) -> bool {
  bool res = false;
  if (root_ && root_->key_ == k && root_->left_ == root_->right_ &&
      root_->right_ == nullptr) {
    delete root_;
    root_ = nullptr;
    return true;
  }
  Node *new_root = Remove(root_, k, res);
  if (new_root) {
    root_ = new_root;
  }
  return res;
}

auto SBT::Remove(Node *p, key_type k, bool &res) -> Node * {
  if (!p) return 0;
  if (k < p->key_)
    p->left_ = Remove(p->left_, k, res);
  else if (k > p->key_)
    p->right_ = Remove(p->right_, k, res);
  //  k == p->key
  else {
    Node *l = p->left_;
    Node *r = p->right_;
    delete p;
    res = true;
    if (!r) return l;
    Node *min = FindMin(r);
    min->right_ = RemoveMin(r);
    min->left_ = l;
    return Balance(min);
  }
  return Balance(p);
}

auto SBT::Set(const record_type &new_record) -> bool {
  Update();
  bool res = true;
  if (!root_) {
    root_ = new Node(new_record);
  } else if (FindRecord(root_, new_record.first)) {
    res = false;
  } else {
    root_ = Insert(root_, new_record);
  }
  return res && static_cast<bool>(root_);
}

auto SBT::FindRecord(Node *p, key_type k) -> Node * {
  if (!p) {
    return nullptr;
  } else if (p->key_ == k) {
    return p;
  } else if (p->key_ > k) {
    return FindRecord(p->left_, k);
  } else {
    return FindRecord(p->right_, k);
  }
}

auto SBT::Get(const key_type &k)
    -> std::optional<std::reference_wrapper<record>> {
  Update();
  Node *n = FindRecord(root_, k);
  return n ? std::optional<std::reference_wrapper<record>>{n->data_}
           : std::nullopt;
};

auto SBT::Exist(const key_type &k) -> bool {
  Update();
  return static_cast<bool>(FindRecord(root_, k));
};

auto SBT::Update(const record_type &rec) -> bool {
  Update();
  Node *node_for_update = FindRecord(root_, rec.first);
  bool res = static_cast<bool>(node_for_update);
  if (res) {
    Person &person_for_update = node_for_update->data_.person_;
    int mask = rec.second.mask_;
    if (mask & MASK_SURNAME)
      person_for_update.surname_ = rec.second.person_.surname_;
    if (mask & MASK_NAME) person_for_update.name_ = rec.second.person_.name_;
    if (mask & MASK_BIRTH_YEAR)
      person_for_update.balance_ = rec.second.person_.balance_;
    if (mask & MASK_CITY)
      person_for_update.birth_year_ = rec.second.person_.birth_year_;
    if (mask & MASK_BALANCE) person_for_update.city_ = rec.second.person_.city_;
  }
  return res;
};

auto SBT::Keys() -> std::vector<key_type> {
  Update();
  std::vector<key_type> res(0);
  preOrder(root_, res);
  return res;
};

auto SBT::Rename(const key_type &old_key, const key_type &new_key) -> bool {
  Update();
  Node *node_for_rename = FindRecord(root_, old_key);
  bool res = false;
  if (static_cast<bool>(node_for_rename)) {
    res = true;
    node_for_rename->key_ = new_key;
    Balance(root_);
  }
  return res;
};

auto SBT::TTL(const key_type &k) -> int {
  Update();
  Node *node = FindRecord(root_, k);
  int res = -1;
  if (node && node->data_.erase_time_ > 0) {
    time_t erase_time = node->data_.create_time_ + node->data_.erase_time_;
    res = erase_time - std::time(0);
    res = static_cast<int>(erase_time - std::time(0));
  }
  return res;
};

auto SBT::Find(const Person &person, int mask) -> std::vector<key_type> {
  Update();
  std::vector<key_type> res(0);
  preOrderFind(root_, person, mask, res);
  return res;
};

auto SBT::ShowAll() -> std::vector<record *> {
  Update();
  std::vector<record *> res(0);
  preOrder(root_, res);
  return res;
};

auto SBT::Clear() -> void {
  if (root_) {
    auto f = [](Node *p) {
      delete p;
      p = nullptr;
    };
    preOrder(root_, f);
    root_ = nullptr;
  }
};

auto SBT::preOrder(Node *p, std::vector<key_type> &res) -> void {
  if (p == nullptr) return;
  res.push_back(p->key_);
  preOrder(p->left_, res);
  preOrder(p->right_, res);
}

auto SBT::preOrder(Node *p, std::vector<record *> &res) -> void {
  if (p == nullptr) return;
  res.push_back(&p->data_);
  preOrder(p->left_, res);
  preOrder(p->right_, res);
}

auto SBT::preOrder(Node *p, func_t f) -> void {
  if (p) {
    preOrder(p->left_, f);
    preOrder(p->right_, f);
    f(p);
  }
}

auto SBT::preOrderFind(Node *p, const Person &person, int mask,
                       std::vector<key_type> &res) -> void {
  if (p == nullptr) return;
  if (checkNode(p, person, mask)) res.push_back(p->key_);
  preOrder(p->left_, res);
  preOrder(p->right_, res);
}

auto SBT::checkNode(Node *p, const Person &person, int mask) -> bool {
  bool res = true;
  Person &person_for_check = p->data_.person_;
  if (res && mask & MASK_SURNAME &&
      person_for_check.surname_ != person.surname_)
    res = false;
  if (res && mask & MASK_NAME && person_for_check.name_ != person.name_)
    res = false;
  if (res && mask & MASK_BIRTH_YEAR &&
      person_for_check.balance_ != person.balance_)
    res = false;
  if (res && mask & MASK_CITY &&
      person_for_check.birth_year_ != person.birth_year_)
    res = false;
  if (res && mask & MASK_BALANCE && person_for_check.city_ != person.city_)
    res = false;
  return res;
}

auto SBT::Update() -> void {
  func_t f = [this](Node *p) {
    if (p) {
      if (p->data_.erase_time_ > 0 &&
          p->data_.create_time_ + p->data_.erase_time_ < std::time(NULL))
        Del(p->key_);
    }
  };
  preOrder(root_, f);
};
