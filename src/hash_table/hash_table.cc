#include "hash_table.h"

using s21::hash1;
using s21::hash2;
using s21::HashTable;

HashTable::HashTable() {
  buffer_size_ = default_size_;
  size_ = 0;
  size_all_non_nullptr_ = 0;
  arr_ = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; ++i) arr_[i] = nullptr;
  // заполняем nullptr - то есть если значение отсутствует,
  // и никто раньше по этому адресу не обращался
}

HashTable::~HashTable() {
  for (int i = 0; i < buffer_size_; ++i)
    if (arr_[i]) delete arr_[i];
  delete[] arr_;
}

auto HashTable::Set(const record_type& record) -> bool {
  bool result = false;
  if (Add(record.first, record.second)) result = true;
  return result;
}

auto HashTable::Get(const key_type&) -> record& {}

int s21::HashFunctionHorner(const std::string& s, int table_size,
                            const int key) {
  int hash_result = 0;
  for (int i = 0; s[i] != s.size(); ++i)
    hash_result = (key * hash_result + s[i]) % table_size;
  hash_result = (hash_result * 2 + 1) % table_size;
  return hash_result;
}

int s21::hash1(const std::string& s, int table_size) {
  return HashFunctionHorner(s, table_size, table_size - 1);
  // ключи должны быть взаимопросты, используем числа <размер таблицы> плюс и
  // минус один.
};

int s21::hash2(const std::string& s, int table_size) {
  return HashFunctionHorner(s, table_size, table_size + 1);
};

void HashTable::Resize() {
  int past_buffer_size_ = buffer_size_;
  buffer_size_ *= 2;
  size_all_non_nullptr_ = 0;
  size_ = 0;
  Node** arr_2 = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; ++i) arr_2[i] = nullptr;
  std::swap(arr_, arr_2);
  for (int i = 0; i < past_buffer_size_; ++i) {
    if (arr_2[i] && arr_2[i]->state_)
      Add(arr_2[i]->key_,
          arr_2[i]->data_);  // добавляем элементы в новый массив
  }
  // удаление предыдущего массива
  for (int i = 0; i < past_buffer_size_; ++i)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

void HashTable::Rehash() {
  size_all_non_nullptr_ = 0;
  size_ = 0;
  Node** arr_2 = new Node*[buffer_size_];
  for (int i = 0; i < buffer_size_; ++i) arr_2[i] = nullptr;
  std::swap(arr_, arr_2);
  for (int i = 0; i < buffer_size_; ++i) {
    if (arr_2[i] && arr_2[i]->state_) Add(arr_2[i]->key_, arr_2[i]->data_);
  }
  // удаление предыдущего массива
  for (int i = 0; i < buffer_size_; ++i)
    if (arr_2[i]) delete arr_2[i];
  delete[] arr_2;
}

//
// s21::key_type HashTable::Find(const T& value) {
//   int h1 =
//       hash1(value, buffer_size_);  // значение, отвечающее за начальную
//       позицию
//   int h2 =
//       hash2(value, buffer_size_);  // значение, ответственное за "шаг" по
//       таблице
//   int i = 0;
//   while (arr_[h1] != nullptr && i < buffer_size_) {
//     if (arr_[h1]->value == value && arr_[h1]->state)
//       return true;  // такой элемент есть
//     h1 = (h1 + h2) % buffer_size_;
//     ++i;  // если у нас i >=  buffer_size_, значит мы уже обошли абсолютно
//     все
//           // ячейки, именно для этого мы считаем i, иначе мы могли бы
//           // зациклиться.
//   }
//   return false;
// }

bool HashTable::Add(const key_type& key, const record& data) {
  if (size_ + 1 > int(rehash_size_ * buffer_size_))
    Resize();
  else if (size_all_non_nullptr_ > 2 * size_)
    Rehash();  // происходит рехеш, так как слишком много deleted-элементов
  int h1 = hash1(key, buffer_size_);
  int h2 = hash2(key, buffer_size_);
  int i = 0;
  int first_deleted = -1;  // запоминаем первый подходящий (удаленный) элемент
  while (arr_[h1] != nullptr && i < buffer_size_) {
    if (arr_[h1]->key_ == key && arr_[h1]->state_)
      return false;  // такой элемент уже есть, а значит его нельзя вставлять
                     // повторно
    if (!arr_[h1]->state_ &&
        first_deleted == -1)  // находим место для нового элемента
      first_deleted = h1;
    h1 = (h1 + h2) % buffer_size_;
    ++i;
  }
  if (first_deleted ==
      -1)  // если не нашлось подходящего места, создаем новый Node
  {
    arr_[h1] = new Node(key, data);
    ++size_all_non_nullptr_;  // так как мы заполнили один пробел, не забываем
                              // записать, что это место теперь занято
  } else {
    arr_[first_deleted]->key_ = key;
    arr_[first_deleted]->data_ = data;
    arr_[first_deleted]->state_ = true;
  }
  ++size_;  // и в любом случае мы увеличили количество элементов
  return true;
}
