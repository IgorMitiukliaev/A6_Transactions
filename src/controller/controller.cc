#include "controller.h"

#include <cstddef>
#include <exception>
#include <ios>
#include <regex>
#include <string>
#include <vector>
// foo bar "Иван Саныч" 55 "Q"
using s21::Controller;
using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

auto Controller::Command(std::string command_str) -> void {
  std::vector<std::string> c(0);
  CommandRead(command_str, c);
  try {
    if (c[0] == "SET") {
      int erase_time = -1;
      if (c[7] == "EX") erase_time = std::stoi(c[8]);
      AddElement(c[1], c[2], c[3], std::stoi(c[4]), c[5], std::stoi(c[6]),
                 erase_time);
      ShowAll();
    } else if (c[0] == "GET") {
      GetElement(c[1]);
    } else if (c[0] == "EXISTS") {
      ExistElement(c[1]);
    } else if (c[0] == "DEL") {
      DeleteElement(c[1]);
    } else if (c[0] == "UPDATE") {
      UpdateElement(
          c[1], c[2] == "-" ? std::nullopt : std::optional<std::string>(c[2]),
          c[3] == "-" ? std::nullopt : std::optional<std::string>(c[3]),
          c[4] == "-" ? std::nullopt : std::optional<int>(std::stoi(c[4])),
          c[5] == "-" ? std::nullopt : std::optional<std::string>(c[5]),
          c[6] == "-" ? std::nullopt : std::optional<int>(std::stoi(c[6])));
    } else if (c[0] == "KEYS") {
    } else if (c[0] == "RENAME") {
    } else if (c[0] == "TTL") {
    } else if (c[0] == "FIND") {
    } else if (c[0] == "SHOWALL") {
    } else if (c[0] == "UPLOAD") {
    } else if (c[0] == "EXPORT") {
    } else if (c[0] == "CLEAR") {
    } else if (c[0] == "STORAGE") {
    } else if (c[0] == "EXIT") {
    } else {
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
}

auto Controller::CommandRead(const std::string& command_str,
                             std::vector<std::string>& command) -> void {
  std::regex r_com(R"(^([[:alnum:]]+)\b)", std::regex::ECMAScript);
  std::regex r_arg(R"([\"]([^\"]+)[\"$]|[\s]([^\"\s$]+))",
                   std::regex::ECMAScript);

  std::smatch m;
  std::regex_search(command_str, m, r_com);
  command.push_back(m[1]);
  std::sregex_iterator it_begin(command_str.begin(), command_str.end(), r_arg);
  std::sregex_iterator it_end = std::sregex_iterator();
  for (std::sregex_iterator i = it_begin; i != it_end; i++) {
    std::smatch m_a = *i;
    std::string match_str =
        m_a[1].str().length() == 0 ? m_a[2].str() : m_a[1].str();
    command.push_back(match_str);
    // std::cout << m_a[0].str() << " | " << m_a[1].str() << " | " <<
    // m_a[2].str()
    //           << " | " << std::endl;
  }
  // std::for_each(command.begin(), command.end(),
  //               [](auto el) { std::cout << el << std::endl; });
};

auto Controller::Init(const BaseType type) -> void {
  if (type == HASH)
    model_ = new ::HashTable;
  else
    model_ = new ::SBT;
  // UploadData("/home/igor/School_21/A6_Transactions-0/src/test.txt");
  // std::cout << ShowKeys();
  // AddElement("key3", "Verter", "Робот, просто робот", 1975, "Smartville",
  // 100,
  //            5);
  // ShowAll();
  // ExportData("/home/igor/School_21/A6_Transactions-0/src/new_test.txt");
  // std::cout << ShowTTL("key3") << std::endl;
  // ShowAll();
};

auto Controller::UploadData(const std::string& path) -> int {
  model_->Clear();
  std::ifstream filestream(path);
  std::regex r_num(R"(\b([[:digit:]]+)\b)", std::regex::ECMAScript);
  std::regex r_key(R"(^([[:alnum:]]+)\b)", std::regex::ECMAScript);
  std::regex r_str(R"(\"(.*?)\")", std::regex::ECMAScript);
  int row = 0;
  bool read_on = true;
  if (filestream.is_open()) {
    std::string buffer = "", field = "", surname = "", key = "", name = "",
                city = "";
    int birth_year = 0, balance = 0;
    while (read_on && !filestream.eof()) {
      std::getline(filestream, buffer);

      std::sregex_iterator it_num(buffer.begin(), buffer.end(), r_num);
      birth_year = stoi((it_num++)->str());
      balance = stoi((it_num)->str());

      std::sregex_iterator it_key(buffer.begin(), buffer.end(), r_key);
      key = it_key->str();

      std::sregex_iterator it_str(buffer.begin(), buffer.end(), r_str);
      surname = (it_str)->str(1);
      ++it_str;
      name = (it_str)->str(1);
      ++it_str;
      city = (it_str)->str(1);

      Person p(surname, name, birth_year, city, balance);
      record rec(p, std::time(NULL), -1, MASK_ALL);
      record_type rec_t(key, rec);
      model_->Set(rec_t);

      std::cout << key << " | " << surname << " | " << name << " | "
                << birth_year << " | " << city << " | " << balance << std::endl;
      buffer.clear();
      row++;
    }
    filestream.close();
  } else {
    std::cout << "Could not read file\n" << std::endl;
  }
  return row;
  // std::vector<record*> data = model_->ShowAll();
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
      iter++;
    }
    filestream.close();
  }
  return rows;
};

auto Controller::AddElement(const std::string key, const std::string surname,
                            const std::string name, const int birth_year,
                            const std::string city, const int balance,
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

auto Controller::UpdateElement(const std::string key,
                               const std::optional<std::string> surname,
                               const std::optional<std::string> name,
                               const std::optional<int> birth_year,
                               const std::optional<std::string> city,
                               const std::optional<int> balance) -> bool {
  int mask = 0b0;
  if (surname) mask |= MASK_SURNAME;
  if (name) mask |= MASK_NAME;
  if (birth_year) mask |= MASK_BIRTH_YEAR;
  if (city) mask |= MASK_CITY;
  if (balance) mask |= MASK_BALANCE;

  Person p(surname.value_or("-"), name.value_or("-"), birth_year.value_or(0),
           city.value_or("-"), balance.value_or(0));
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
  std::vector<key_type> matched_keys;
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
  for (unsigned i = 0; i < recs.size(); i++) {
    buffer += recs[i]->person_.ShowData() + "\n";
  };
}

auto Controller::ClearStorage() -> void { model_->Clear(); }

auto Controller::ShowTTL(const std::string& key_ttl) -> std::string {
  std::string res = "(null)";
  record_nullable rec = model_->Get(key_ttl);
  if (rec) {
    time_t create_time = model_->Get(key_ttl)->get().create_time_;
    int erase_time_ = model_->Get(key_ttl)->get().erase_time_;
    if (erase_time_ > 0) {
      int duration = (create_time + erase_time_) - std::time(NULL);
      res = std::to_string(duration);
    }
  }
  return res;
}
