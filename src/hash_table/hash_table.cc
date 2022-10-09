#include "hash_table.h"

using s21::hash1;
using s21::hash2;
using s21::HashTable;

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

template <typename T>
HashTable<T>::HashTable() {
  buffer_size = default_size;
  size = 0;
  size_all_non_nullptr = 0;
  arr = new Node*[buffer_size];
  for (int i = 0; i < buffer_size; ++i)
    arr[i] = nullptr;  // заполняем nullptr - то есть если значение отсутствует,
                       // и никто раньше по этому адресу не обращался
}

template <typename T>
HashTable<T>::~HashTable() {
  for (int i = 0; i < buffer_size; ++i)
    if (arr[i]) delete arr[i];
  delete[] arr;
}

template <typename T>
void HashTable<T>::Resize() {
  int past_buffer_size = buffer_size;
  buffer_size *= 2;
  size_all_non_nullptr = 0;
  size = 0;
  Node** arr2 = new Node*[buffer_size];
  for (int i = 0; i < buffer_size; ++i) arr2[i] = nullptr;
  std::swap(arr, arr2);
  for (int i = 0; i < past_buffer_size; ++i) {
    if (arr2[i] && arr2[i]->state)
      Add(arr2[i]->value);  // добавляем элементы в новый массив
  }
  // удаление предыдущего массива
  for (int i = 0; i < past_buffer_size; ++i)
    if (arr2[i]) delete arr2[i];
  delete[] arr2;
}

template <typename T>
void HashTable<T>::Rehash() {
  size_all_non_nullptr = 0;
  size = 0;
  Node** arr2 = new Node*[buffer_size];
  for (int i = 0; i < buffer_size; ++i) arr2[i] = nullptr;
  std::swap(arr, arr2);
  for (int i = 0; i < buffer_size; ++i) {
    if (arr2[i] && arr2[i]->state) Add(arr2[i]->value);
  }
  // удаление предыдущего массива
  for (int i = 0; i < buffer_size; ++i)
    if (arr2[i]) delete arr2[i];
  delete[] arr2;
}

template <typename T>
s21::key_type HashTable<T>::Find(const T& value) {
  int h1 =
      hash1(value, buffer_size);  // значение, отвечающее за начальную позицию
  int h2 =
      hash2(value, buffer_size);  // значение, ответственное за "шаг" по таблице
  int i = 0;
  while (arr[h1] != nullptr && i < buffer_size) {
    if (arr[h1]->value == value && arr[h1]->state)
      return true;  // такой элемент есть
    h1 = (h1 + h2) % buffer_size;
    ++i;  // если у нас i >=  buffer_size, значит мы уже обошли абсолютно все
          // ячейки, именно для этого мы считаем i, иначе мы могли бы
          // зациклиться.
  }
  return false;
}

template <typename T>
bool HashTable<T>::Add(const T& value) {
  if (size + 1 > int(rehash_size * buffer_size))
    Resize();
  else if (size_all_non_nullptr > 2 * size)
    Rehash();  // происходит рехеш, так как слишком много deleted-элементов
  int h1 = hash1(value, buffer_size);
  int h2 = hash2(value, buffer_size);
  int i = 0;
  int first_deleted = -1;  // запоминаем первый подходящий (удаленный) элемент
  while (arr[h1] != nullptr && i < buffer_size) {
    if (arr[h1]->value == value && arr[h1]->state)
      return false;  // такой элемент уже есть, а значит его нельзя вставлять
                     // повторно
    if (!arr[h1]->state &&
        first_deleted == -1)  // находим место для нового элемента
      first_deleted = h1;
    h1 = (h1 + h2) % buffer_size;
    ++i;
  }
  if (first_deleted ==
      -1)  // если не нашлось подходящего места, создаем новый Node
  {
    arr[h1] = new Node(value);
    ++size_all_non_nullptr;  // так как мы заполнили один пробел, не забываем
                             // записать, что это место теперь занято
  } else {
    arr[first_deleted]->value = value;
    arr[first_deleted]->state = true;
  }
  ++size;  // и в любом случае мы увеличили количество элементов
  return true;
}