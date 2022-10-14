#include "controller.h"

#include <cstddef>
#include <exception>
#include <ios>
#include <regex>
#include <string>
#include <vector>

using s21::Controller;
using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

auto Controller::Command(std::string command_str) -> std::string {
  std::vector<std::string> c(0), res_v;
  CommandRead(command_str, c);
  std::string res;

  try {
    bool res_b;
    if (c[0] == "set" && (c.size() == 7 || c.size() == 9)) {
      int erase_time = -1;
      if (c[7] == "EX") erase_time = std::stoi(c[8]);
      res_b = AddElement(c[1], c[2], c[3], std::stoi(c[4]), c[5],
                         std::stoi(c[6]), erase_time);
      res = res_b ? "OK" : "Unable to perform " + c[0];
    } else if (c[0] == "get" && c.size() == 2) {
      res = GetElement(c[1]);
    } else if (c[0] == "exists" && c.size() == 2) {
      res_b = ExistElement(c[1]);
      res = res_b ? "OK" : "Key " + c[1] + " doesn't exist";
    } else if (c[0] == "del" && c.size() == 2) {
      res_b = DeleteElement(c[1]);
      res = res_b ? "Deleted"
                  : "Key " + c[1] + " doesn't exist, unable to delete";
    } else if (c[0] == "update" && c.size() == 7) {
      res_b = UpdateElement(
          c[1], c[2] == "-" ? std::nullopt : std::optional<std::string>(c[2]),
          c[3] == "-" ? std::nullopt : std::optional<std::string>(c[3]),
          c[4] == "-" ? std::nullopt : std::optional<std::string>(c[4]),
          c[5] == "-" ? std::nullopt : std::optional<std::string>(c[5]),
          c[6] == "-" ? std::nullopt : std::optional<std::string>(c[6]));
      res = res_b ? "OK" : "Key " + c[1] + " doesn't exist, unable to update";
    } else if (c[0] == "keys" && c.size() == 1) {
      res = ShowKeys();
    } else if (c[0] == "rename" && c.size() == 3) {
      res_b = RenameKey(c[1], c[2]);
      res = res_b ? "Key " + c[1] + " set to " + c[2]
                  : "Key " + c[1] + " doesn't exist, unable to delete";
    } else if (c[0] == "ttl" && c.size() == 2) {
      res = ShowTTL(c[1]);
    } else if (c[0] == "find" && c.size() == 6) {
      res = FindElement(
          c[1] == "-" ? std::nullopt : std::optional<std::string>(c[1]),
          c[2] == "-" ? std::nullopt : std::optional<std::string>(c[2]),
          c[3] == "-" ? std::nullopt : std::optional<int>(std::stoi(c[3])),
          c[4] == "-" ? std::nullopt : std::optional<std::string>(c[4]),
          c[5] == "-" ? std::nullopt : std::optional<int>(std::stoi(c[5])));
    } else if (c[0] == "showall" && c.size() == 1) {
      ShowAll(res);
    } else if (c[0] == "upload" && c.size() == 2) {
      res = std::to_string(UploadData(c[1]));
    } else if (c[0] == "export" && c.size() == 2) {
      res = std::to_string(ExportData(c[1]));
    } else if (c[0] == "clear" && c.size() == 1) {
      ClearStorage();
      res = "Storage is empty";
    } else if (c[0] == "storage") {
      model_->Clear();
      if (model_->GetType() == s21::HASH) {
        model_ = new s21::SelfBalancingBinarySearchTree;
        res = "Storage type set to AVL Tree";
      } else {
        model_ = new s21::HashTable;
        res = "Storage type set to HashTable";
      }
    } else if (c[0] == "exit") {
      res = "Goodbye!";
    } else {
      res = "Incorrect command";
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return res;
}

auto Controller::CommandRead(const std::string& command_str,
                             std::vector<std::string>& command) -> void {
  std::regex r_com(R"(^([[:alnum:]]+)\b)",
                   std::regex::ECMAScript | std::regex::icase);
  std::regex r_arg(R"([\"]([^\"]+)[\"$]|[\s]([^\"\s$]+))",
                   std::regex::ECMAScript);

  std::smatch m;
  std::regex_search(command_str, m, r_com);
  command.push_back(m[1]);

  std::for_each(command[0].begin(), command[0].end(),
                [](char& c) { c = ::tolower(c); });

  std::sregex_iterator it_begin(command_str.begin(), command_str.end(), r_arg);
  std::sregex_iterator it_end = std::sregex_iterator();
  for (std::sregex_iterator i = it_begin; i != it_end; i++) {
    std::smatch m_a = *i;
    std::string match_str =
        m_a[1].str().length() == 0 ? m_a[2].str() : m_a[1].str();
    command.push_back(match_str);
  }
};

auto Controller::Init(const BaseType type) -> void {
  if (type == HASH)
    model_ = new ::HashTable;
  else
    model_ = new ::SBT;
};

auto Controller::UploadData(const std::string& path) -> int {
  model_->Clear();
  std::ifstream filestream(path);
  std::regex r_all(
      R"(^([[:alnum:]]+)\s+"(.*?)\"\s+"(.*?)\"\s+([[:digit:]]+)\s+"(.*?)\"\s+([[:digit:]]+)$)",
      std::regex::ECMAScript);
  std::regex r_num(R"(\b([[:digit:]]+)\b)", std::regex::ECMAScript);
  std::regex r_key(R"(^([[:alnum:]]+)\b)", std::regex::ECMAScript);
  std::regex r_str(R"(\"(.*?)\")", std::regex::ECMAScript);
  int rows = 0;
  if (filestream.is_open()) {
    std::string buffer = "";
    std::getline(filestream, buffer);
    while (!filestream.eof()) {
      std::string field = "", surname = "", key = "", name = "", city = "";

      int birth_year, balance;
      std::smatch m;
      std::regex_match(buffer, m, r_all);
      key = m[1];
      surname = m[2];
      name = m[3];
      birth_year = std::stoi(m[4]);
      city = m[5];
      balance = std::stoi(m[6]);
      Person p(surname, name, birth_year, city, balance);
      std::cout << p.ShowData() << std::endl;
      record rec(p, std::time(NULL), -1, MASK_ALL);
      record_type rec_t(key, rec);
      if (model_->Set(rec_t)) rows++;
      buffer.clear();
      std::getline(filestream, buffer);
    }
    filestream.close();
  } else {
    std::cout << "Could not read file\n" << std::endl;
  }
  return rows;
};

auto Controller::ExportData(const std::string& path) -> int {
  std::ofstream filestream(path, std::ios::out | std::ios::trunc);
  int rows = 0;
  if (filestream.is_open()) {
    std::vector<key_type> data = model_->Keys();
    std::vector<key_type>::iterator iter = data.begin();
    Person p;
    while (iter != data.end()) {
      filestream << *iter << " ";
      p = model_->Get(*iter).value().get().person_;
      filestream << " \"" << p.surname_ << "\" ";
      filestream << " \"" << p.name_ << "\" ";
      filestream << p.birth_year_ << " ";
      filestream << " \"" << p.city_ << "\" ";
      filestream << p.balance_ << std::endl;
      ++iter;
      ++rows;
    }
    filestream.close();
  }
  return rows;
};

auto Controller::AddElement(const std::string& key, const std::string& surname,
                            const std::string& name, const int birth_year,
                            const std::string& city, const int balance,
                            int erase_time) -> bool {
  Person p(surname, name, birth_year, city, balance);
  record rec(p, std::time(NULL), erase_time, MASK_ALL);
  record_type rec_t(key, rec);
  return model_->Set(rec_t);
}

auto Controller::GetElement(const std::string& key) -> const std::string {
  record_nullable element = model_->Get(key);
  if (element == std::nullopt)
    return "(null)";
  else
    return element->get().person_.ShowData();
}

auto Controller::ExistElement(const std::string& key) -> bool {
  return model_->Get(key) != std::nullopt;
}

auto Controller::DeleteElement(const std::string& key) -> bool {
  return model_->Del(key);
}

auto Controller::UpdateElement(const std::string& key,
                               const std::optional<std::string> surname,
                               const std::optional<std::string> name,
                               const std::optional<std::string> birth_year,
                               const std::optional<std::string> city,
                               const std::optional<std::string> balance)
    -> bool {
  int mask = 0b0;
  if (surname) mask |= MASK_SURNAME;
  if (name) mask |= MASK_NAME;
  if (birth_year) mask |= MASK_BIRTH_YEAR;
  if (city) mask |= MASK_CITY;
  if (balance) mask |= MASK_BALANCE;

  Person p(surname.value_or("-"), name.value_or("-"),
           std::stoi(birth_year.value_or("0")), city.value_or("-"),
           std::stoi(balance.value_or("0")));
  record rec(p, std::time(NULL), -1, mask);
  record_type rec_t(key, rec);
  return model_->Update(rec_t);
}

auto Controller::ShowKeys() -> std::string {
  std::vector<key_type> keys_vector = model_->Keys();
  std::string res;
  for (unsigned i = 0; i < keys_vector.size(); i++) {
    res = res + keys_vector[i] + "\n";
  }
  if (keys_vector.size() == 0) res = "(null)";
  return res;
}

auto Controller::RenameKey(const std::string& old_key,
                           const std::string& new_key) -> bool {
  return model_->Rename(old_key, new_key);
}

auto Controller::FindElement(const std::optional<std::string> surname,
                             const std::optional<std::string> name,
                             const std::optional<int> birth_year,
                             const std::optional<std::string> city,
                             const std::optional<int> balance) -> std::string {
  std::vector<key_type> matched_keys(0);
  std::string res;
  int mask = 0b0;
  if (surname) mask |= MASK_SURNAME;
  if (name) mask |= MASK_NAME;
  if (birth_year) mask |= MASK_BIRTH_YEAR;
  if (city) mask |= MASK_CITY;
  if (balance) mask |= MASK_BALANCE;

  Person p(surname.value_or("-"), name.value_or("-"), birth_year.value_or(0),
           city.value_or("-"), balance.value_or(0));
  matched_keys = model_->Find(p, mask);
  for (unsigned i = 0; i < matched_keys.size(); i++) {
    res = res + matched_keys[i] + "\n";
  }
  return res;
}

auto Controller::ShowAll() -> void {
  auto f = [this]() {
    std::vector<record*> recs = model_->ShowAll();
    for (unsigned i = 0; i < recs.size(); i++) {
      std::cout << recs[i]->person_.ShowData() << std::endl;
    }
  };
  f();
}

auto Controller::ShowAll(std::string& buffer) -> void {
  std::vector<record*> recs = model_->ShowAll();
  if (recs.size() == 0) buffer = "(null)";
  for (unsigned i = 0; i < recs.size(); i++) {
    buffer += recs[i]->person_.ShowData() + "\n";
  };
}

auto Controller::ClearStorage() -> void { model_->Clear(); }

auto Controller::ShowTTL(const std::string& key_ttl) -> std::string {
  std::string res = "(null)";
  record_nullable rec = model_->Get(key_ttl);
  if (rec) {
    int erase_time_ = model_->Get(key_ttl)->get().erase_time_;
    if (erase_time_ > 0) {
      time_t create_time = model_->Get(key_ttl)->get().create_time_;
      int duration = (create_time + erase_time_) - std::time(NULL);
      res = std::to_string(duration);
    }
  }
  return res;
}
