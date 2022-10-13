#include "controller.h"

using s21::Controller;
using SBT = s21::SelfBalancingBinarySearchTree;
using HashTable = s21::HashTable;

auto Controller::Command(std::string command) -> std::string {
  std::string answer;
  answer = command;
  return answer;
}

auto Controller::Init(const BaseType type) -> void {
  if (type == HASH)
    model_ = new ::HashTable;
  else
    model_ = new ::SBT;

  UploadData(
      "/Users/pfidelia/Schule_21/Meine_Projekte/A6_Transactions-0/src/"
      "test.txt");

  AddElement("key3", "Verter", "Робот, просто робот", 1975, "Smartville", 100,
             5);
  // // // ShowAll();
  AddElement("key4", "Verter2", "Робот, просто робот", 1976, "Smartville", 100,
             5);
  AddElement("key5", "Verter3", "Робот, просто робот", 1977, "Smartville", 100,
             5);

  ShowAll();
  std::cout << ShowKeys();
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
};