#include <gtest/gtest.h>

#include <memory>

#include "../hash_table/hash_table.h"
#include "../self_balancing_binary_search_tree/self_balancing_binary_search_tree.h"

using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

// TEST(SET_HashTable, test) {
//   std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
//   s21::Person p("Surname", "Name Name", 1970, "City", 100);
//   s21::record r(p);
//   bool res = model_->Set(s21::record_type("key", r));
//   ASSERT_TRUE(res);

//   std::optional<s21::Person> p_res =
//   model_->Get("key").value().get().person_; ASSERT_TRUE(p_res);
//   ASSERT_TRUE(p_res->ShowData() == p.ShowData());
// }

TEST(SET_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::SBT>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  bool res = model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(res);

  std::optional<s21::Person> p_res = model_->Get("key").value().get().person_;
  ASSERT_TRUE(p_res);
  ASSERT_TRUE(p_res->ShowData() == p.ShowData());
}

TEST(EXIST_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::SBT>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  bool res = model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(model_->Exist("key"));
  ASSERT_FALSE(model_->Exist("not_key"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}