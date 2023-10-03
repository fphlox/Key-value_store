#pragma once

#include <iostream>
#include <optional>

#include "kvstore.hpp"
#include "self_balancing_binary_tree/avl_node.hpp"

namespace s21 {
class SelfBalancingBinarySearchTree : public Store {
 public:
  SelfBalancingBinarySearchTree();
  ~SelfBalancingBinarySearchTree();

  std::string set(std::string str);            //Создание записи
  std::optional<t_data> get(std::string key);  // Получить запись
  bool exists(std::string key);  // Существует ли запись
  bool del(std::string key);     // Удаление записи
  std::string update(std::string str);  // Изменение данных
  std::vector<std::string> keys();  // Возврат списка имеющихся ключей
  std::string rename(std::string old_key,
                     std::string new_key);  // Переименование ключа
  int ttl(std::string key);  // Просмотр оставшегося времени жизни записи
  std::vector<std::string> find(std::string str);  // Поиск ключей по данным
  std::vector<t_data> showall();  // Показывает список всех значений
  std::string load(std::string file_name);  // Загружает таблицу из файла
  std::string fexport(std::string filename);  // Выгружает таблицу в файл

 private:
  AVLNode* _entry;
  std::vector<std::string> _temporary_list;

  void bazeInsert(std::string key, t_data data);
  void bazeRemove(std::string delkey);
  AVLNode* bazeFind(std::string key);

  void checkTimers();
};
}  // namespace s21
