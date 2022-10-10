#include "controller.h"

using s21::Controller;
using SBT = s21::SelfBalancingBinarySearchTree;
// using HashTable = s21::HashTable;

auto Controller::Init(const BaseType type) -> void {
  if (type == HASH)
    ;
  // model_ = new ::HashTable;
  else
    model_ = new ::SBT;
  record_type new_rec("qqq", record());
  model_->Set(new_rec);
  model_->Set(new_rec);
  model_->Set(new_rec);
  new_rec.first = "www";
  model_->Set(new_rec);
  model_->Get("qqq");
  model_->Del("qqq");
  std::cout << "OK\n";
};