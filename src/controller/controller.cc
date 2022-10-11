#include "controller.h"

#include <vector>

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
                                      std::time(0), 20, MASK_ALL)));
  model_->Set(record_type("3", record(Person("SN3", "N3", 1950, "Q", 10),
                                      std::time(0), 0, MASK_ALL)));
  model_->Set(record_type("1", record(Person("SN1", "N1", 1980, "C", 10),
                                      std::time(0), 0, MASK_ALL)));
  std::optional<std::reference_wrapper<record>> a = model_->Get("1");
  std::cout << a.value().get().person_.surname_ << std::endl;
  std::cout << model_->Get("2").value().get().person_.ShowData() << std::endl;
  std::cout << model_->TTL("2") << std::endl;
  model_->Del("2");
  model_->Clear();
  std::cout << "OK\n";
};

auto Controller::UploadData(const std::string& input_str) -> void {
  model_->Clear();
  std::cout << input_str << std::endl;
  std::vector<record*> data = model_->ShowAll();
};

auto Controller::ExportData(const std::string& input_str) -> void {
  std::cout << input_str << std::endl;

  std::vector<record*> data = model_->ShowAll();
};