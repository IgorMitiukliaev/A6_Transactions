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

  model_->Set(record_type("5", record(Person("SN5", "N5", 1980, "C", 10),
                                      std::time(0), 0, MASK_ALL)));
  model_->Set(record_type("2", record(Person("SN2", "N2", 1990, "C", 10),
                                      std::time(0), 0, MASK_ALL)));
  model_->Set(record_type("3", record(Person("SN3", "N3", 1950, "Q", 10),
                                      std::time(0), 0, MASK_ALL)));
  model_->Set(record_type("1", record(Person("SN1", "N1", 1980, "C", 10),
                                      std::time(0), 0, MASK_ALL)));
  std::optional<std::reference_wrapper<record>> a = model_->Get("1");
  std::cout << a.value().get().person_.surname_ << std::endl;
  model_->Del("2");
  std::cout << "OK\n";
};