#include "hash_table.h"

using s21::HashTable;
using std::cout;

HashTable::HashTable() {
  buffer_size_ = default_size_;
  size_ = 0;
  size_all_non_nullptr_ = 0;
  arr_ = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; i++) {
    arr_[i] = new Node();
  }
}

HashTable::~HashTable() {
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] != nullptr) delete arr_[i];
  }
  delete[] arr_;
}

auto HashTable::Set(const record_type& record) -> bool {
  Update();
  bool result = false;
  Node* temp = FindRecord(record.first);
  if (temp == nullptr) {
    if (Add(record.first, record.second)) result = true;
  }
  return result;
}

auto HashTable::Get(const key_type& key)
    -> std::optional<std::reference_wrapper<record>> {
  Update();
  Node* get_node = FindRecord(key);
  return get_node
             ? std::optional<std::reference_wrapper<record>>{get_node->data_}
             : std::nullopt;
}

auto HashTable::Exist(const key_type& key) -> bool {
  Update();
  bool result = false;
  if (FindRecord(key) != nullptr) result = true;
  return result;
}

auto HashTable::Del(const key_type& key) -> bool {
  bool result = false;
  Node* del = FindRecord(key);
  if (del != nullptr) {
    del->state_ = false;
    result = true;
  }
  return result;
}

auto HashTable::Update(const record_type& record) -> bool {
  Update();
  Node* node_for_update = FindRecord(record.first);
  bool result = static_cast<bool>(node_for_update);
  if (result) {
    Person& person_for_update = node_for_update->data_.person_;
    int mask = record.second.mask_;
    if (mask & MASK_SURNAME)
      person_for_update.surname_ = record.second.person_.surname_;
    if (mask & MASK_NAME) person_for_update.name_ = record.second.person_.name_;
    if (mask & MASK_BIRTH_YEAR)
      person_for_update.balance_ = record.second.person_.balance_;
    if (mask & MASK_CITY)
      person_for_update.birth_year_ = record.second.person_.birth_year_;
    if (mask & MASK_BALANCE)
      person_for_update.city_ = record.second.person_.city_;
  }
  return result;
}

auto HashTable::Keys() -> std::vector<key_type> {
  Update();
  std::vector<key_type> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_ && !arr_[i]->empty_)
      result.push_back(arr_[i]->key_);
  }
  return result;
}

auto HashTable::Rename(const key_type& old_key, const key_type& new_key)
    -> bool {
  Update();
  bool result = false;
  Node* node_for_rename = FindRecord(old_key);
  if (static_cast<bool>(node_for_rename) && FindRecord(new_key) == nullptr) {
    record record = node_for_rename->data_;
    Del(old_key);
    Add(new_key, record);
    result = true;
  }
  return result;
}

auto HashTable::TTL(const key_type& key) -> int {
  Update();
  Node* node = FindRecord(key);
  int result = -1;
  if (node && node->data_.erase_time_ > 0) {
    time_t erase_time = node->data_.create_time_ + node->data_.erase_time_;
    result = static_cast<int>(erase_time - std::time(0));
  }
  return result;
}

auto HashTable::Find(const Person& person, int mask) -> std::vector<key_type> {
  Update();
  std::vector<key_type> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_ && CheckNode(arr_[i]->key_, person, mask))
      result.push_back(arr_[i]->key_);
  }
  return result;
}

auto HashTable::ShowAll() -> std::vector<record*> {
  Update();
  std::vector<record*> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_ && !arr_[i]->empty_) {
      result.push_back(&arr_[i]->data_);
    }
  }
  return result;
}

auto HashTable::Clear() -> void {
  for (int i = 0; i < buffer_size_; i++) {
    if (!arr_[i]->empty_) {
      arr_[i]->key_ = "";
      arr_[i]->empty_ = true;
    }
  }
  size_ = 0;
}

auto HashTable::Update() -> void {
  for (int i = 0; i < buffer_size_; i++) {
    if (!arr_[i]->empty_ && arr_[i]->state_ && arr_[i]->data_.erase_time_ > 0) {
      Node* node = FindRecord(arr_[i]->key_);
      if (node &&
          node->data_.create_time_ + node->data_.erase_time_ < std::time(0)) {
        Del(node->key_);
      }
    }
  }
}

auto HashTable::CheckNode(const key_type& key, const Person& person, int mask)
    -> bool {
  bool result = true;
  Node* node = FindRecord(key);
  Person& person_for_check = node->data_.person_;
  if (mask & MASK_SURNAME && person_for_check.surname_ != person.surname_)
    result = false;
  if (result && mask & MASK_NAME && person.name_ != person.name_)
    result = false;
  if (result && mask & MASK_BIRTH_YEAR &&
      person_for_check.balance_ != person.balance_)
    result = false;
  if (result && mask & MASK_CITY &&
      person_for_check.birth_year_ != person.birth_year_)
    result = false;
  if (result && mask & MASK_BALANCE && person_for_check.city_ != person.city_)
    result = false;
  return result;
}

auto HashTable::Resize() -> void {
  int past_buffer_size_ = buffer_size_;
  buffer_size_ *= 2;
  size_all_non_nullptr_ = 0;
  size_ = 0;
  Node** arr_2 = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; i++) arr_2[i] = new Node();
  std::swap(arr_, arr_2);
  for (int i = 0; i < past_buffer_size_; i++) {
    if (arr_2[i] && !arr_2[i]->empty_ && arr_2[i]->state_)
      Add(arr_2[i]->key_, arr_2[i]->data_);
  }
  for (int i = 0; i < past_buffer_size_; i++)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

auto HashTable::Rehash() -> void {
  size_all_non_nullptr_ = 0;
  size_ = 0;
  Node** arr_2 = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; i++) arr_2[i] = new Node();
  std::swap(arr_, arr_2);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_2[i] && arr_2[i]->state_ && !arr_2[i]->empty_)
      Add(arr_2[i]->key_, arr_2[i]->data_);
  }
  for (int i = 0; i < buffer_size_; i++)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

auto HashTable::FindRecord(const key_type& key) -> Node* {
  int h1 = Hash1(key);
  int h2 = Hash2(key);
  int i = 0;
  while (!arr_[h1]->empty_ && i < buffer_size_) {
    if (arr_[h1]->key_ == key && arr_[h1]->state_) return arr_[h1];
    i++;
    h1 = (h1 + i * h2) % buffer_size_;
  }
  return nullptr;
}

auto HashTable::Add(const key_type& key, const record& data) -> bool {
  if (size_ + 1 > int(rehash_size_ * buffer_size_)) {
    Resize();
  } else if (size_all_non_nullptr_ > 2 * size_) {
    Rehash();
  }
  int h1 = Hash1(key);
  int h2 = Hash2(key);
  int i = 0;
  int temp_pos = -1;

  while (arr_[h1] && !arr_[h1]->empty_ && i < buffer_size_) {
    if (arr_[h1]->key_ == key && arr_[h1]->state_) return false;
    if (arr_[h1]->key_ != "" && !arr_[h1]->state_) {
      temp_pos = h1;
      break;
    }
    i++;
    h1 = (h1 + i * h2) % buffer_size_;
  }

  if (temp_pos == -1) temp_pos = h1;
  arr_[temp_pos]->key_ = key;
  arr_[temp_pos]->data_ = data;
  arr_[temp_pos]->state_ = true;
  arr_[temp_pos]->empty_ = false;

  size_++;
  return true;
}

auto HashTable::Hash1(const std::string& key) -> int {
  return (std::hash<std::string>{}(key)) % buffer_size_;
}

auto HashTable::Hash2(const std::string& key) -> int {
  return ((std::hash<std::string>{}(key)) - 1) % buffer_size_;
}
