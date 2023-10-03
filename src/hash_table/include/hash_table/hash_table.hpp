#pragma once

#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <optional>
#include <vector>

#include "kvstore.hpp"
#include "kvstore/KVS.hpp"

namespace s21 {

struct HashNode {  //узел, необходимый для реализации односвязного списка
  std::string key;
  t_data data;
};

using Bucket = std::list<HashNode>;
using BucketVector = std::vector<Bucket>;

using HashNodeRef = Bucket::iterator;

class HashTable : public Store {
 public:
  std::string set(std::string str) override;  //Создание записи
  std::optional<t_data> get(std::string key) override;  // Получить запись
  bool exists(std::string key) override;  // Существует ли запись
  bool del(std::string key) override;     // Удаление записи
  std::string update(std::string str) override;  // Изменение данных
  std::vector<std::string> keys() override;  // Возврат списка имеющихся ключей
  std::string rename(std::string old_key,
                     std::string new_key) override;  // Переименование ключа
  int ttl(
      std::string key) override;  // Просмотр оставшегося времени жизни записи
  std::vector<std::string> find(
      std::string str) override;  // Поиск ключей по данным
  std::vector<t_data> showall() override;  // Показывает список всех значений
  std::string load(
      std::string file_name) override;  // Загружает таблицу из файла
  std::string fexport(
      std::string filename) override;  // Выгружает таблицу в файл

 private:
  int _mod = 8;
  BucketVector _buckets = BucketVector(_mod);
  const int _alpha = 2;
  int _realSize = 0;
  float _LoadFactor = 3;
  std::vector<std::string> _temporary_list;
  std::vector<std::string> _keys_list;

  int hash(std::string key);
  void bazeInsert(std::string key, t_data value);
  std::optional<HashNodeRef> bazeFind(std::string key);
  bool bazeErase(std::string key);
  void checkTimers();
};

}  // namespace s21
