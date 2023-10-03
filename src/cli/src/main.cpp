#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <string_view>

#include "hash_table.hpp"
#include "kvstore.hpp"
#include "self_balancing_binary_tree.hpp"
#include "self_balancing_binary_tree/self_balancing_binary_search_tree.hpp"

auto getStore(int argc, char** argv) -> std::unique_ptr<s21::Store> {
  if (argc < 2) {
    return std::make_unique<s21::HashTable>();
  }

  auto storeType = std::string(argv[1]);

  if (storeType == "btree") {
    return std::make_unique<s21::SelfBalancingBinarySearchTree>();
  }

  return std::make_unique<s21::HashTable>();
}

auto handleSet(s21::Store& store, std::istringstream&, std::string rest) {
  auto response = store.set(rest);

  std::cout << "> " << response << std::endl;
}

auto handleGet(s21::Store& store, std::istringstream& command, std::string) {
  std::string key;

  command >> key;

  auto data = store.get(key);

  if (!data) {
    std::cout << "> (null)" << std::endl;
  } else {
    std::cout << "> " << data->last_name << " " << data->first_name << " "
              << data->year_of_birth << " " << data->city << " " << data->coins
              << std::endl;
  }
}

auto handleExists(s21::Store& store, std::istringstream& command, std::string) {
  std::string key;

  command >> key;

  auto result = store.exists(key);

  std::cout << "> " << std::boolalpha << result << std::endl;
}

auto handleDel(s21::Store& store, std::istringstream& command, std::string) {
  std::string key;

  command >> key;

  auto result = store.del(key);

  std::cout << "> " << std::boolalpha << result << std::endl;
}

auto handleUpdate(s21::Store& store, std::istringstream&, std::string rest) {
  auto response = store.update(rest);

  std::cout << "> " << response << std::endl;
}

auto handleKeys(s21::Store& store, std::istringstream&, std::string) {
  auto keys = store.keys();

  for (auto i = 0ul; i < keys.size(); i++) {
    std::cout << i + 1 << ") " << keys[i] << "\n";
  }

  std::cout << std::flush;
}

auto handleRename(s21::Store& store, std::istringstream& command, std::string) {
  std::string oldKey, newKey;

  command >> oldKey >> newKey;

  auto response = store.rename(oldKey, newKey);

  std::cout << "> " << response << std::endl;
}

auto handleTtl(s21::Store& store, std::istringstream& command, std::string) {
  std::string key;

  command >> key;

  auto result = store.ttl(key);

  if (result == 0) {
    std::cout << "> (null)" << std::endl;
  } else {
    std::cout << "> " << result << std::endl;
  }
}

auto handleFind(s21::Store& store, std::istringstream&, std::string rest) {
  auto records = store.find(rest);

  for (auto i = 0ul; i < records.size(); i++) {
    std::cout << "> " << i + 1 << ") " << records[i] << "\n";
  }

  std::cout << std::flush;
}

auto handleShowAll(s21::Store& store, std::istringstream&, std::string) {
  auto records = store.showall();

  std::cout << std::left << "> " << std::setw(2) << "№ "
            << "| " << std::setw(15) << "Last name"
            << "| " << std::setw(15) << "First name"
            << "| " << std::setw(5) << "Year"
            << "| " << std::setw(10) << "City"
            << "| " << std::setw(15) << "Number of coins"
            << " |\n";

  for (auto i = 0ul; i < records.size(); i++) {
    std::cout << std::left << "> " << std::setw(2) << i << "| " << std::setw(15)
              << records[i].last_name << "| " << std::setw(15)
              << records[i].first_name << "| " << std::setw(5)
              << records[i].year_of_birth << "| " << std::setw(10)
              << records[i].city << "| " << std::setw(15) << records[i].coins
              << " |\n";
  }

  std::cout << std::flush;
}

auto handleUpload(s21::Store& store, std::istringstream& command, std::string) {
  std::string path;

  command >> path;

  auto response = store.load(path);

  std::cout << "> " << response << std::endl;
}

auto handleExport(s21::Store& store, std::istringstream& command, std::string) {
  std::string path;

  command >> path;

  auto response = store.fexport(path);

  std::cout << "> " << response << std::endl;
}

void cli(int argc, char** argv) {
  auto store = getStore(argc, argv);

  while (true) {
    std::string line;

    std::getline(std::cin, line);

    std::istringstream lin(line);

    std::string command;

    lin >> command;

    auto rest = line.substr(command.length());

    if (command == "EXIT") {
      return;
    }

    if (command == "SET") {
      handleSet(*store, lin, rest);
    } else if (command == "GET") {
      handleGet(*store, lin, rest);
    } else if (command == "EXISTS") {
      handleExists(*store, lin, rest);
    } else if (command == "DEL") {
      handleDel(*store, lin, rest);
    } else if (command == "UPDATE") {
      handleUpdate(*store, lin, rest);
    } else if (command == "KEYS") {
      handleKeys(*store, lin, rest);
    } else if (command == "RENAME") {
      handleRename(*store, lin, rest);
    } else if (command == "TTL") {
      handleTtl(*store, lin, rest);
    } else if (command == "FIND") {
      handleFind(*store, lin, rest);
    } else if (command == "SHOWALL") {
      handleShowAll(*store, lin, rest);
    } else if (command == "UPLOAD") {
      handleUpload(*store, lin, rest);
    } else if (command == "EXPORT") {
      handleExport(*store, lin, rest);
    } else {
      std::cout << "> unknown command" << std::endl;
    }
  }
}

int main(int argc, char** argv) {
  try {
    cli(argc, argv);
  } catch (std::exception& e) {
    std::cout << "Fatal error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
