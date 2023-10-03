#pragma once

#include <chrono>
#include <fstream>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

namespace s21 {

struct t_data {
  std::string last_name;
  std::string first_name;
  int year_of_birth;
  std::string city;
  int coins;
  int ex;
  std::chrono::system_clock::time_point time_start;
};

class Store {
 public:
  virtual std::string set(std::string str) = 0;  // Создание записи
  virtual std::optional<t_data> get(std::string key) = 0;  // Получить запись
  virtual bool exists(std::string key) = 0;  // Существует ли запись
  virtual bool del(std::string key) = 0;  // Удаление записи
  virtual std::string update(std::string str) = 0;  // Изменение данных
  virtual std::vector<std::string> keys() = 0;  //Возврат списка ключей
  virtual std::string rename(std::string old_key,
                             std::string new_key) = 0;  // Переименование ключа
  virtual int ttl(std::string key) = 0;  //Просмотр времени жизни записи
  virtual std::vector<std::string> find(
      std::string str) = 0;  //Поиск ключей по данным
  virtual std::vector<t_data> showall() = 0;  // Показывает список всех значений
  virtual std::string load(
      std::string file_name) = 0;  //Загружает таблицу из файла
  virtual std::string fexport(
      std::string filename) = 0;  //Выгружает таблицу в файл
};
}  // namespace s21
