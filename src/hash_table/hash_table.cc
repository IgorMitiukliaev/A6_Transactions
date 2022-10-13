#include "hash_table.h"

// using s21::hash1;
// using s21::hash2;
using s21::HashTable;

using std::cout;

// int s21::HashFunctionHorner(const std::string& s, int table_size,
//                             const int key) {
//   int hash_result = 0;
//   for (int i = 0; s[i] != s.size(); ++i) {
//     hash_result = (key * hash_result + s[i]) % table_size;
//     std::cout << "hash_result " << hash_result << std::endl;
//   }
//   hash_result = (hash_result * 2 + 1) % table_size;
//   return hash_result;
// }

// int s21::hash1(const std::string& s, int table_size) {
//   return HashFunctionHorner(s, table_size, table_size - 1);
//   // ключи должны быть взаимопросты, используем числа <размер таблицы> плюс и
//   // минус один.
// }

// int s21::hash2(const std::string& s, int table_size) {
//   return HashFunctionHorner(s, table_size, table_size + 1);
// }

HashTable::HashTable() {
  buffer_size_ = default_size_;
  size_ = 0;
  size_all_non_nullptr_ = 0;
  arr_ = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; i++) arr_[i] = nullptr;
  // заполняем nullptr - то есть если значение отсутствует,
  // и никто раньше по этому адресу не обращался
}

HashTable::~HashTable() {
  for (int i = 0; i < buffer_size_; i++)
    if (arr_[i]) delete arr_[i];
  delete[] arr_;
}

auto HashTable::Set(const record_type& record) -> bool {
  bool result = false;
  if (Add(record.first, record.second)) result = true;
  return result;
}

// auto HashTable::Get(const key_type&) -> record& {}

auto HashTable::Get(const key_type& key)
    -> std::optional<std::reference_wrapper<record>> {
  Node* get_node = FindRecord(key);
  return get_node
             ? std::optional<std::reference_wrapper<record>>{get_node->data_}
             : std::nullopt;
}

auto HashTable::Exist(const key_type& key) -> bool {
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
  std::vector<key_type> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_) result.push_back(arr_[i]->key_);
  }
  return result;
}

auto HashTable::Rename(const key_type& old_key, const key_type& new_key)
    -> bool {
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
  Node* node = FindRecord(key);
  int result = -1;
  if (node && node->data_.erase_time_ > 0) {
    time_t erase_time = node->data_.create_time_ + node->data_.erase_time_;
    result = erase_time - std::time(0);
    result = static_cast<int>(erase_time - std::time(0));
  }
  return result;
}

auto HashTable::Find(const Person& person, int mask) -> std::vector<key_type> {
  std::vector<key_type> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_ && CheckNode(arr_[i]->key_, person, mask))
      result.push_back(arr_[i]->key_);
  }
  return result;
}

auto HashTable::ShowAll() -> std::vector<record*> {
  std::vector<record*> result(0);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_[i] && arr_[i]->state_) result.push_back(&arr_[i]->data_);
  }
  return result;
}

auto HashTable::Upload(const std::string& path) -> size_t {
  size_t result;
  return result;
}

auto HashTable::Export(const std::string& path) -> size_t {
  size_t result;
  return result;
}

auto HashTable::Clear() -> void {
  for (int i = 0; i < buffer_size_; i++)
    if (arr_[i]) delete arr_[i];
  delete[] arr_;
};

auto HashTable::CheckNode(const key_type& key, const Person& person, int mask)
    -> bool {
  bool result = true;
  Node* node = FindRecord(key);
  Person& person_for_check = node->data_.person_;
  // Person& person_for_update = node_for_update->data_.person_
  if (result && mask & MASK_SURNAME &&
      person_for_check.surname_ != person.surname_)
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
  for (int i = 0; i < buffer_size_; i++) arr_2[i] = nullptr;
  std::swap(arr_, arr_2);
  for (int i = 0; i < past_buffer_size_; i++) {
    if (arr_2[i] && arr_2[i]->state_)
      Add(arr_2[i]->key_,
          arr_2[i]->data_);  // добавляем элементы в новый массив
  }
  // удаление предыдущего массива
  for (int i = 0; i < past_buffer_size_; i++)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

auto HashTable::Rehash() -> void {
  size_all_non_nullptr_ = 0;
  size_ = 0;
  Node** arr_2 = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; i++) arr_2[i] = nullptr;
  std::swap(arr_, arr_2);
  for (int i = 0; i < buffer_size_; i++) {
    if (arr_2[i] && arr_2[i]->state_) Add(arr_2[i]->key_, arr_2[i]->data_);
  }
  // удаление предыдущего массива
  for (int i = 0; i < buffer_size_; i++)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

auto HashTable::FindRecord(const key_type& key) -> Node* {
  int h1 =
      Hash1(key, buffer_size_);  // значение, отвечающее за начальную позицию
  int h2 =
      Hash2(key, buffer_size_);  // значение, ответственное за "шаг" по таблице
  int i = 0;
  while (arr_[h1] != nullptr && i < buffer_size_) {
    if (arr_[h1]->key_ == key && arr_[h1]->state_)
      return arr_[h1];  // такой элемент есть
    h1 = (h1 + h2) % buffer_size_;
    i++;  // если у нас i >=  buffer_size_, значит мы уже обошли абсолютно все
    // ячейки, именно для этого мы считаем i, иначе мы могли бы
    // зациклиться.
  }
  return nullptr;
}

auto HashTable::Add(const key_type& key, const record& data) -> bool {
  if (size_ + 1 > int(rehash_size_ * buffer_size_)) {
    std::cout << "Resize!!!" << std::endl;
    Resize();
  } else if (size_all_non_nullptr_ > 2 * size_) {
    std::cout << "Rehash!!!" << std::endl;
    Rehash();  // происходит рехеш, так как слишком много deleted-элементов
  }
  int h1 = Hash1(key, buffer_size_);
  int h2 = Hash2(key, buffer_size_);
  int i = 0;

  std::cout << "h1 " << h1 << std::endl;
  // std::cout << "h2 " << h2 << std::endl;

  int first_deleted = -1;  // запоминаем первый подходящий (удаленный) элемент

  // for (int i = 0; i < buffer_size_; i++) {
  //   // std::cout << "While()!!!" << std::endl;
  //   // такой элемент уже есть, а значит его нельзя вставлять
  //   // повторно
  //   if (arr_[i]->key_ == key && arr_[h1]->state_) {
  //     return false;
  //   }
  //   if (arr_[i] != nullptr && !arr_[h1]->state_) {
  //     // нашли место для нового элемента
  //     first_deleted = i;
  //     break;
  //   }
  // }

  while (arr_[h1] != nullptr && i < buffer_size_) {
    std::cout << "While()!!!" << std::endl;
    if (arr_[h1]->key_ == key && arr_[h1]->state_)
      return false;  // такой элемент уже есть, а значит его нельзя вставлять
                     // повторно
    if (arr_[h1] && !arr_[h1]->state_) {
      std::cout << "Find place!" << std::endl;
      // нашли место для нового элемента
      first_deleted = h1;
      break;
    }
    h1 = (h1 + h2) % buffer_size_;
    std::cout << "new h1 " << h1 << std::endl;
    i++;
  }
  if (first_deleted == -1)
  // если не нашлось подходящего места, создаем новый Node
  {
    std::cout << "=======new Node #" << h1 << std::endl;
    arr_[h1] = new Node(key, data);
    ++size_all_non_nullptr_;  // так как мы заполнили один пробел, не забываем
    // записать, что это место теперь занято
  } else {
    std::cout << "=======fill Node #" << first_deleted << std::endl;
    if (arr_[first_deleted] != nullptr) {
      arr_[first_deleted]->key_ = key;
      arr_[first_deleted]->data_ = data;
      arr_[first_deleted]->state_ = true;
    }
  }
  ++size_;  // и в любом случае мы увеличили количество элементов
  std::cout << "size_ " << size_ << std::endl;
  return true;
}

auto HashTable::HashFunctionHorner(const std::string& s, int table_size,
                                   const int key) -> int {
  int hash_result = 0;
  for (int i = 0; s[i] != s.size(); i++) {
    hash_result = (key * hash_result + s[i]) % table_size;
    // std::cout << "hash_result " << hash_result << std::endl;
  }
  hash_result = (hash_result * 2 + 1) % table_size;
  if (hash_result < 0) hash_result *= -1;
  // std::cout << "hash_result result: " << hash_result << std::endl;
  return hash_result;
}

auto HashTable::Hash1(const std::string& s, int table_size) -> int {
  return HashFunctionHorner(s, table_size, table_size - 1);
  // ключи должны быть взаимопросты, используем числа <размер таблицы> плюс и
  // минус один.
}

auto HashTable::Hash2(const std::string& s, int table_size) -> int {
  return HashFunctionHorner(s, table_size, table_size + 1);
}
