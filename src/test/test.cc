#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>

#include "../hash_table/hash_table.h"
#include "../self_balancing_binary_search_tree/self_balancing_binary_search_tree.h"

using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

// SET key1 NoSurname NoName 1990 Nsk 50 EX 3
// SET key2 Surname Name 1990 Nsk 50 EX 10
// SHOWALL

TEST(SET_GET_SBT, test) {
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
  ASSERT_TRUE(res);
  ASSERT_TRUE(model_->Exist("key"));
  ASSERT_FALSE(model_->Exist("not_key"));
}

TEST(DEL_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::SBT>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key", r));
  ASSERT_FALSE(model_->Del("not_key"));
  ASSERT_TRUE(model_->Del("key"));
  ASSERT_FALSE(model_->Del("key"));

  model_->Set(s21::record_type("1", r));
  model_->Set(s21::record_type("2", r));
  model_->Set(s21::record_type("3", r));
  model_->Set(s21::record_type("4", r));
  model_->Set(s21::record_type("5", r));
  model_->Set(s21::record_type("6", r));
  ASSERT_FALSE(model_->Del("not_key"));
  ASSERT_TRUE(model_->Del("2"));
  ASSERT_TRUE(model_->Del("1"));
  ASSERT_FALSE(model_->Del("2"));
}

TEST(UPDATE_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::SBT>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_new("Noname", "Noname", 2000, "No", 0);
  s21::record r(p);
  s21::record r_new(p_new);
  model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(model_->Update(s21::record_type("key", r_new)));
  ASSERT_FALSE(model_->Update(s21::record_type("no", r_new)));
  r = model_->Get("key").value();
  ASSERT_TRUE(r.person_.ShowData() == r_new.person_.ShowData());
}

TEST(KEYS_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::SBT>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_new("Noname", "Noname", 2000, "No", 0);
  s21::record r(p);
  s21::record r_new(p_new);
  model_->Set(s21::record_type("key1", r));
  model_->Set(s21::record_type("key2", r_new));
  model_->Set(s21::record_type("key3", r_new));
  std::vector<s21::key_type> res = model_->Keys();
  ASSERT_TRUE(res.size() == 3);
  ASSERT_TRUE((res[0] == "key1" && res[1] == "key2") ||
              (res[0] == "key2" && res[1] == "key1"));
}

TEST(RENAME_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::SBT());
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key1", r));
  ASSERT_TRUE(model_->Rename("key1", "key2"));
  ASSERT_TRUE(model_->Get("key2").value().get().person_.ShowData() ==
              p.ShowData());
  model_->Clear();
}

TEST(TTL_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::SBT());
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p, std::time(NULL), 1, s21::MASK_ALL);
  model_->Set(s21::record_type("key1", r));

  ASSERT_TRUE(model_->TTL("key1") > 0);
  ASSERT_TRUE(model_->Exist("key1"));
  ASSERT_TRUE(model_->Get("key1").value().get().person_.ShowData() ==
              p.ShowData());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_FALSE(model_->Exist("key2"));
}

TEST(FIND_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::SBT());
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_find("NoName", "Name NoName", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key1", r));
  std::vector<std::string> res = model_->Find(p_find, 0b11111);
  ASSERT_TRUE(res.size() == 0);
  res = model_->Find(p_find, 0b00111);
  ASSERT_TRUE(res.size() == 1);
}

TEST(SHOWALL_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::SBT());
  s21::Person p1("Surname1", "Name1", 1970, "City1", 100);
  s21::Person p2("Surname2", "Name2", 1990, "City2", 0);
  s21::Person p3("Surname3", "Name3", 1993, "City3", 0);
  s21::Person p4("Surname4", "Name4", 1994, "City4", 0);
  s21::Person p5("Surname5", "Name5", 1995, "City5", 0);
  s21::record r(p1);
  model_->Set(s21::record_type("d", r));
  r = s21::record(p2);
  model_->Set(s21::record_type("e", r));
  r = s21::record(p3);
  model_->Set(s21::record_type("f", r));
  r = s21::record(p4);
  model_->Set(s21::record_type("g", r));
  r = s21::record(p5);
  model_->Set(s21::record_type("c", r));
  model_->Set(s21::record_type("b", r));
  model_->Set(s21::record_type("a", r));
  model_->Set(s21::record_type("k", r));
  std::vector<s21::record *> res = model_->ShowAll();
  ASSERT_TRUE(res.size() == 8);
}

TEST(CLEAR_SBT, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::SBT());
  s21::Person p1("Surname1", "Name1", 1970, "City1", 100);
  s21::record r(p1);
  model_->Set(s21::record_type("key1", r));
  model_->Clear();
  std::vector<s21::record *> res = model_->ShowAll();
  ASSERT_TRUE(res.size() == 0);
}

TEST(SET_GET_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  bool res = model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(res);

  std::optional<s21::Person> p_res = model_->Get("key").value().get().person_;
  ASSERT_TRUE(p_res);
  ASSERT_TRUE(p_res->ShowData() == p.ShowData());
}

TEST(EXIST_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  bool res = model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(res);
  ASSERT_TRUE(model_->Exist("key"));
  ASSERT_FALSE(model_->Exist("not_key"));
}

TEST(DEL_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key", r));
  ASSERT_FALSE(model_->Del("not_key"));
  ASSERT_TRUE(model_->Del("key"));
  ASSERT_FALSE(model_->Del("key"));

  model_->Set(s21::record_type("1", r));
  model_->Set(s21::record_type("2", r));
  model_->Set(s21::record_type("3", r));
  model_->Set(s21::record_type("4", r));
  model_->Set(s21::record_type("5", r));
  model_->Set(s21::record_type("6", r));
  ASSERT_FALSE(model_->Del("not_key"));
  ASSERT_TRUE(model_->Del("2"));
  ASSERT_TRUE(model_->Del("1"));
  ASSERT_FALSE(model_->Del("2"));
}

TEST(UPDATE_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_new("Noname", "Noname", 2000, "No", 0);
  s21::record r(p);
  s21::record r_new(p_new);
  model_->Set(s21::record_type("key", r));
  ASSERT_TRUE(model_->Update(s21::record_type("key", r_new)));
  ASSERT_FALSE(model_->Update(s21::record_type("no", r_new)));
  r = model_->Get("key").value();
  ASSERT_TRUE(r.person_.ShowData() == r_new.person_.ShowData());
}

TEST(KEYS_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_new("Noname", "Noname", 2000, "No", 0);
  s21::record r(p);
  s21::record r_new(p_new);
  model_->Set(s21::record_type("key1", r));
  model_->Set(s21::record_type("key2", r_new));
  std::vector<s21::key_type> res = model_->Keys();
  ASSERT_TRUE(res.size() == 2);
  ASSERT_TRUE((res[0] == "key1" && res[1] == "key2") ||
              (res[0] == "key2" && res[1] == "key1"));
}

TEST(RENAME_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key1", r));
  ASSERT_TRUE(model_->Rename("key1", "key2"));
  ASSERT_TRUE(model_->Get("key2").value().get().person_.ShowData() ==
              p.ShowData());
}

TEST(TTL_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::record r(p, std::time(NULL), 1, s21::MASK_ALL);
  model_->Set(s21::record_type("key1", r));

  ASSERT_TRUE(model_->TTL("key1") > 0);
  ASSERT_TRUE(model_->Exist("key1"));
  ASSERT_TRUE(model_->Get("key1").value().get().person_.ShowData() ==
              p.ShowData());
  std::this_thread::sleep_for(std::chrono::seconds(2));
  ASSERT_FALSE(model_->Exist("key2"));
}

TEST(FIND_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::HashTable());
  s21::Person p("Surname", "Name Name", 1970, "City", 100);
  s21::Person p_find("NoName", "Name NoName", 1970, "City", 100);
  s21::record r(p);
  model_->Set(s21::record_type("key1", r));
  std::vector<std::string> res = model_->Find(p_find, 0b11111);
  ASSERT_TRUE(res.size() == 0);
  res = model_->Find(p_find, 0b00111);
  ASSERT_TRUE(res.size() == 1);
}

TEST(SHOWALL_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_(new ::HashTable());
  s21::Person p1("Surname1", "Name1", 1970, "City1", 100);
  s21::Person p2("Surname2", "Name2", 1990, "City2", 0);
  s21::Person p3("Surname3", "Name3", 1993, "City3", 0);
  s21::Person p4("Surname4", "Name4", 1994, "City4", 0);
  s21::Person p5("Surname5", "Name5", 1995, "City5", 0);
  s21::record r(p1);
  model_->Set(s21::record_type("dasdfgghhj", r));
  r = s21::record(p2);
  model_->Set(s21::record_type("easdfgghhj", r));
  r = s21::record(p3);
  model_->Set(s21::record_type("fasdfgghhj", r));
  r = s21::record(p4);
  model_->Set(s21::record_type("gasdfgghhj", r));
  r = s21::record(p5);
  model_->Set(s21::record_type("casdfgghhj", r));
  model_->Set(s21::record_type("basdfgghhj", r));
  model_->Set(s21::record_type("aasdfgghhj", r));
  model_->Set(s21::record_type("kasdfgghhj", r));
  std::vector<s21::record *> res = model_->ShowAll();
  ASSERT_TRUE(res.size() == 8);
}

TEST(CLEAR_HashTable, test) {
  std::unique_ptr<s21::BaseClass> model_ = std::make_unique<::HashTable>();
  s21::Person p1("Surname1", "Name1", 1970, "City1", 100);
  s21::record r(p1);
  model_->Set(s21::record_type("key1", r));
  model_->Clear();
  std::vector<s21::record *> res = model_->ShowAll();
  ASSERT_TRUE(res.size() == 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}