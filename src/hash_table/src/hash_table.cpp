#include "hash_table/hash_table.hpp"

#include <optional>

namespace s21 {

int HashTable::hash(std::string key) {  // хеш-функция
  return (std::hash<std::string>{}(key) % _mod);
}

void HashTable::bazeInsert(std::string key,
                           t_data value)  // добавление элемента в таблицу
{
  if (_LoadFactor < ((float)_realSize / _mod)) {
    _mod *= _alpha;
    auto NewbucketsArray = BucketVector(_mod);

    for (int i = 0; i < _mod / _alpha; i++) {
      NewbucketsArray[i] = _buckets[i];
    }
    _buckets = NewbucketsArray;
  }
  int index = hash(key);

  for (auto& node : _buckets[index]) {
    if (node.key == key) {
      return;
    }
  }

  _buckets[index].push_front({key, value});
  _keys_list.push_back(key);
  _realSize++;
}

std::optional<HashNodeRef> HashTable::bazeFind(
    std::string key)  // поиск элемента по таблице
{
  int index = hash(key);

  auto& bucket = _buckets[index];

  auto e = std::find_if(bucket.begin(), bucket.end(),
                        [key](auto e) { return e.key == key; });

  if (e == bucket.end()) {
    return {};
  }

  return e;
}

bool HashTable::bazeErase(std::string key)  // удаление элемента из таблицы
{
  auto index = hash(key);

  auto& bucket = _buckets[index];

  auto item = std::find_if(bucket.begin(), bucket.end(),
                           [key](auto e) { return e.key == key; });

  if (item != bucket.end()) {
    _keys_list.erase(std::find(_keys_list.begin(), _keys_list.end(), key));
    bucket.erase(item);
    _realSize--;

    return true;
  }

  return false;
}

void HashTable::checkTimers() {
  std::stack<std::size_t> gotcha;

  if (_temporary_list.size() > 0) {
    std::size_t temporary = _temporary_list.size();
    for (std::size_t i = 0; i < temporary; i++) {
      if (auto node = bazeFind(_temporary_list[i])) {
        if (node.value()->data.ex != 0) {
          auto diff = std::chrono::duration_cast<std::chrono::seconds>(
              std::chrono::system_clock::now() - node.value()->data.time_start);
          if (node.value()->data.ex - diff.count() <= 0) {
            bazeErase(node.value()->key);
            gotcha.push(i);
          }
        }
      }
    }
    while (gotcha.size() > 0) {
      _temporary_list.erase(_temporary_list.begin() + gotcha.top());
      gotcha.pop();
    }
  }
}

//Создание записи
std::string HashTable::set(std::string str) {
  std::stringstream ss(str);
  std::vector<std::string> words;
  t_data data;

  checkTimers();

  std::string word;
  while (ss >> word) {
    words.push_back(word);
  }
  if (words.size() < 6) return ("ERROR: not enough data");

  std::string key = words[0];
  data.last_name = words[1];
  data.first_name = words[2];

  try {
    data.year_of_birth = stoi(words[3]);
  } catch (std::invalid_argument&) {
    std::string ret =
        "ERROR: unable to cast value \\\"" + words[3] + "\\\" to type int";
    return ret;
  }
  if (data.year_of_birth < 0)
    return ("ERROR: year of birth cannot be negative");

  data.city = words[4];

  try {
    data.coins = stoi(words[5]);
  } catch (std::invalid_argument&) {
    std::string ret =
        "ERROR: unable to cast value \\\"" + words[5] + "\\\" to type int";
    return ret;
  }

  data.ex = 0;
  if (words.size() > 7) {
    if (words[6] == "EX") {
      try {
        data.ex = stoi(words[7]);
      } catch (std::invalid_argument&) {
        std::string ret =
            "ERROR: unable to cast value \\\"" + words[7] + "\\\" to type int";
        return ret;
      }
      if (data.ex <= 0) return ("OK");
      _temporary_list.push_back(key);
      data.time_start = std::chrono::system_clock::now();
    }
  }

  bazeInsert(key, data);
  return ("OK");
}

// Получить запись
std::optional<t_data> HashTable::get(std::string key) {
  checkTimers();

  if (auto node = bazeFind(key)) {
    return node.value()->data;
  }

  return {};
}

// Существует ли запись
bool HashTable::exists(std::string key) {
  checkTimers();

  auto it = bazeFind(key);
  if (it) return true;

  return false;
}

// Удаление записи
bool HashTable::del(std::string key) {
  checkTimers();

  auto it = bazeFind(key);
  if (it) {
    bazeErase(key);
    return true;
  }

  return false;
}

// Изменение данных
std::string HashTable::update(std::string str) {
  std::stringstream ss(str);
  std::vector<std::string> words;

  checkTimers();

  std::string word;
  while (ss >> word) {
    words.push_back(word);
  }
  if (words.size() < 6) return ("ERROR: not enough data");

  std::string key = words[0];
  auto it = bazeFind(key);
  if (!it) return ("ERROR: update value not found");
  if (words[1] != "-") it.value()->data.last_name = words[1];
  if (words[2] != "-") it.value()->data.first_name = words[2];

  try {
    if (words[3] != "-") it.value()->data.year_of_birth = stoi(words[3]);
  } catch (std::invalid_argument&) {
    std::string ret =
        "ERROR: unable to cast value \\\"" + words[3] + "\\\" to type int";
    return ret;
  }
  if (it.value()->data.year_of_birth < 0)
    return ("ERROR: year of birth cannot be negative");

  if (words[4] != "-") it.value()->data.city = words[4];

  try {
    if (words[5] != "-") it.value()->data.coins = stoi(words[5]);
  } catch (std::invalid_argument&) {
    std::string ret =
        "ERROR: unable to cast value \\\"" + words[5] + "\\\" to type int";
    return ret;
  }

  return ("OK");
}

// Возврат списка имеющихся ключей
std::vector<std::string> HashTable::keys() {
  std::vector<std::string> ret;

  if (_realSize == 0) return (ret);

  checkTimers();

  return _keys_list;
}

// Переименование ключа
std::string HashTable::rename(std::string old_key, std::string new_key) {
  checkTimers();

  auto it = bazeFind(old_key);
  if (it) {
    t_data data = it.value()->data;
    bazeErase(it.value()->key);
    bazeInsert(new_key, data);
    return ("OK");
  }

  return ("ERROR: renamemy value not found");
}

// Просмотр оставшегося времени жизни записи
int HashTable::ttl(std::string key) {
  checkTimers();

  auto it = bazeFind(key);
  if (it) {
    if (it.value()->data.ex != 0) {
      auto diff = std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now() - it.value()->data.time_start);
      return static_cast<int>(it.value()->data.ex - diff.count());
    } else {
      return (std::numeric_limits<int>::infinity());
    }
  }

  return 0;
}

// Поиск ключей по данным
std::vector<std::string> HashTable::find(std::string str) {
  std::stringstream ss(str);
  std::vector<std::string> words;
  t_data data;
  std::vector<std::string> ret;
  std::vector<bool> args(5, true);

  if (_realSize == 0) return (ret);

  checkTimers();

  std::string word;
  while ((ss >> word) && (words.size() < 5)) {
    words.push_back(word);
    if (word == "-") args[words.size() - 1] = false;
  }
  while (words.size() < 5) {
    words.push_back("-");
    args[words.size() - 1] = false;
  }

  data.last_name = words[0];
  if (words[0] == "-") args[0] = false;
  data.first_name = words[1];
  if (words[1] == "-") args[1] = false;

  if (words[2] != "-") {
    try {
      data.year_of_birth = stoi(words[2]);
    } catch (std::invalid_argument&) {
      return ret;
    }
    if (data.year_of_birth < 0) {
      return ret;
    }
  } else {
    args[2] = false;
  }

  data.city = words[3];
  if (words[3] == "-") args[3] = false;

  if (words[4] != "-") {
    try {
      data.coins = stoi(words[4]);
    } catch (std::invalid_argument&) {
      return ret;
    }
  } else {
    args[4] = false;
  }

  std::size_t j = 0;
  for (auto i : args) {
    if (i == false) j++;
  }
  if (j == args.size()) return ret;

  std::vector<std::string> keys;
  keys = _keys_list;

  while (keys.size() > 0) {
    auto it = bazeFind(keys.back());
    keys.pop_back();
    bool yes = true;

    if ((args[0]) && (it.value()->data.last_name != data.last_name))
      yes = false;
    if ((args[1]) && (it.value()->data.first_name != data.first_name))
      yes = false;
    if ((args[2]) && (it.value()->data.year_of_birth != data.year_of_birth))
      yes = false;
    if ((args[3]) && (it.value()->data.city != data.city)) yes = false;
    if ((args[4]) && (it.value()->data.coins != data.coins)) yes = false;

    if (yes) ret.push_back(it.value()->key);
  }

  return ret;
}

// Показывает список всех значений
std::vector<t_data> HashTable::showall() {
  std::vector<t_data> ret;
  std::vector<std::string> keys;

  if (_realSize == 0) return (ret);

  checkTimers();

  keys = _keys_list;

  while (keys.size() > 0) {
    auto it = bazeFind(keys.back());

    ret.push_back(it.value()->data);
    keys.pop_back();
  }

  return ret;
}

static bool getWord(std::stringstream& ss, std::string& data) {
  std::string word;

  ss >> word;
  if (ss.good() == false) return false;

  if (word == "\\\"") {
    ss >> word;
    if (ss.good() == false) return false;
    if ((word.size() >= 2) && (word.substr(word.size() - 2) == "\\\"")) {
      data = word.substr(0, word.size() - 2);
    } else {
      data = word;
      ss >> word;
    }
  } else {
    word = word.substr(2, word.size());
    if ((word.size() >= 2) && (word.substr(word.size() - 2) == "\\\"")) {
      data = word.substr(0, word.size() - 2);
    } else {
      data = word;
      ss >> word;
    }
  }

  return true;
}

// Загружает таблицу из файла
std::string HashTable::load(std::string filename) {
  std::ifstream reader(filename);
  std::string str;
  std::string key;
  t_data data;
  int count = 0;

  checkTimers();

  if (reader.fail()) {
    return ("ERROR: Could not open file");
  }

  while (std::getline(reader, str)) {
    std::stringstream ss(str);
    std::string word;

    ss >> key;
    if (ss.good() == false) continue;

    if (getWord(std::ref(ss), std::ref(data.last_name)) == false) continue;
    if (getWord(std::ref(ss), std::ref(data.first_name)) == false) continue;

    ss >> word;
    if (ss.good() == false) continue;
    try {
      data.year_of_birth = stoi(word);
    } catch (std::invalid_argument&) {
      continue;
    }
    if (data.year_of_birth < 0) continue;

    if (getWord(std::ref(ss), std::ref(data.city)) == false) continue;

    ss >> word;
    try {
      data.coins = stoi(word);
    } catch (std::invalid_argument&) {
      continue;
    }

    bazeInsert(key, data);
    count++;
  }

  return ("OK " + std::to_string(count));
}

// Выгружает таблицу в файл
std::string HashTable::fexport(std::string filename) {
  if (_realSize == 0) return ("Table is empty");

  std::ofstream writer(filename);
  std::vector<std::string> keys;

  checkTimers();
  if (writer.fail()) {
    return ("ERROR: Could not open file");
  }

  keys = _keys_list;

  std::size_t size = keys.size();
  while (keys.size() > 0) {
    auto it = bazeFind(keys.back());
    keys.pop_back();
    writer << it.value()->key << " \\\"";
    writer << it.value()->data.last_name << "\\\" \\\"";
    writer << it.value()->data.first_name << "\\\" ";
    writer << it.value()->data.year_of_birth << " \\\"";
    writer << it.value()->data.city << "\\\" ";
    writer << it.value()->data.coins << std::endl;
  }

  return ("OK " + std::to_string(size));
}

}  // namespace s21
