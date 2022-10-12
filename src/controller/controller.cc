#include "controller.h"


#include <string>
#include <vector>

using s21::Controller;
using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

auto Controller::Command(std::string command) -> void {
  std::cout << "command =" << command << std::endl;
}

auto Controller::Init(const BaseType type) -> void {
  if (type == HASH)
    model_ = new ::HashTable;
  else
    model_ = new ::SBT;
  // UploadData("/home/igor/School_21/A6_Transactions-0/src/test.txt");
  // std::cout << ShowKeys();
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
  std::vector<record*> data = model_->ShowAll();
  int rows = 0;
  if (data.size() > 0) {
    std::ofstream filestream(path);

    
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

auto Controller::RenameKey(const std::string&) -> void{};